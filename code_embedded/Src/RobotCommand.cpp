/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotCommand.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 304 $
 * Last Updated: $Date: 2008-02-19 14:18:31 -0500 (Tue, 19 Feb 2008) $
 * 
 * 
 * Copyright (c) 2005-2007, Matthew G Bolitho
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of The Johns Hopkins University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***************************************************************************************************/

#include "Robot.hpp"

#include "RobotCommand.hpp"
#include "RobotServer.hpp"

#include "System/Configuration.hpp"
using Bolitho::System::Configuration;

#include "Debug.h"

//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,RobotCommandServer,Runnable);
ImplementRuntimeType(NASA,RobotCommandConnection,Runnable);
//----------------------------------------------------------------------------------------------------------------------------
RobotCommandServer::RobotCommandServer(RobotServer& Server) : m_Server(Server)
{
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL RobotCommandServer::Initialize()
{
#if !defined(SPACECUBE)
  try
  {
    m_pServerSocket = new Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);

    m_pServerSocket->Bind(IPEndPoint(IPAddress::Any(), Configuration::GetValue<UINT>("RobotCommandServer.Port") ));
    m_pServerSocket->Listen();
    TraceInfo(this, "Listening for command connections on %s", m_pServerSocket->EndPoint().ToString().ConstStr());
  }
  catch(SocketException& E)
  {
    TraceError(this, "Failed to create server socket: %s", E.ToString().ConstStr());
    return FALSE;
  }
#else

  /* Might need to change this, but it will be a starting point */
  /* Also see Line #112 */
  LPCTSTR DeviceName = Configuration::GetValue<String>("RobotCommandServer.DeviceName");
  DWORD BaudRate = Configuration::GetValue<DWORD>("RobotCommandServer.BaudRate");

  m_pServerPort = SerialPort::Open(DeviceName);
  m_pServerPort->SetParity(Parity::NONE);
  m_pServerPort->SetStopBits(StopBits::ONE);
  m_pServerPort->SetBaudRate(BaudRate);
  m_pServerPort->Purge();

#endif

  return Subsystem::Initialize();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotCommandServer::Destroy()
{
  Subsystem::Destroy();
}
//----------------------------------------------------------------------------------------------------------------------------
INT RobotCommandServer::ThreadMain()
{
  Trace0Enter("");

  #if !defined(SPACECUBE)
  while (TRUE)
  {
    Ptr<Socket> Client;
    try
    {
      Client = m_pServerSocket->Accept();
      TraceInfo(this, "Accepted command connection from %s", Client->EndPoint().Address().ToString().ConstStr());

      new RobotCommandConnection(new SocketStream(Client), m_Server, TRUE);
    }
    catch(SocketException& E)
    {
      TraceError(this, "Failed to accept connection: %s", E.ToString().ConstStr());
    }
  }
  #else

  /* Might need to change this, but it will be a starting point */
  RobotCommandConnection CommandConnection(m_pServerPort, m_Server, FALSE);
  CommandConnection.ThreadMain();

  #endif
}
//----------------------------------------------------------------------------------------------------------------------------
RobotCommandConnection::RobotCommandConnection(Ptr<Stream> pClient, RobotServer& Server, BOOL RunOnThread) : m_pClient(pClient), m_Server(Server)
{
  m_LastTransmittedImage = 0;
  m_LastTransmittedRangeImage = 0;

  if (RunOnThread)
    m_pThread = Thread::Create(this);
}
//----------------------------------------------------------------------------------------------------------------------------
INT RobotCommandConnection::ThreadMain()
{
  Trace0Enter("");

  try
  {
    while (TRUE)
    {
      BinaryStream S(m_pClient);

      /* Read the request packet */
      PACKETHEADER Request;
      S >> Request.MessageId;
      S >> Request.VariableLengthData;

      Buffer<BYTE> Packet(Request.VariableLengthData);
      if (Request.VariableLengthData > 0)
        m_pClient->Read(&Packet[0], Request.VariableLengthData);

      //Trace("Packet %d %d", Request.MessageId, Request.VariableLengthData);
      /* Process the packet */
      switch (Request.MessageId)
      {
      case 0x00000001:
        /* Echo */
        {
          //TraceInfo(this, "RobotCommandConnection::Echo()" );

          PACKETHEADER Response;

          Response.MessageId = 0x80000001;
          Response.VariableLengthData = Request.VariableLengthData;

          S << Response.MessageId;
          S << Response.VariableLengthData;

          if (Packet.Length() > 0)
            m_pClient->Write(&Packet[0], Request.VariableLengthData);

          break;
        }
      case 0x00000002:
        /* GetPath */
        {
          TraceInfo(this, "RobotCommandConnection::GetPath()" );

          Ptr<Path> ActivePath = m_Server.GetMotionSubsystem()->GetActivePath();

          PACKETHEADER Response;

          Ptr<MemoryStream> M = new MemoryStream();
          m_Server.GetMotionSubsystem()->GetActivePath()->Write(M);

          Response.MessageId = 0x80000002;
          Response.VariableLengthData = (DWORD)(M->Length());

          S << Response.MessageId;
          S << Response.VariableLengthData;

          m_pClient->Write(&M->GetBuffer()[0], (SIZE_T)M->Length());

          break;
        }

      case 0x00000003:
        /* SetPath */
        {
          TraceInfo(this, "RobotCommandConnection::SetPath()" );

          Ptr<Path> P = new Path();
          Ptr<MemoryStream> M = new MemoryStream(Packet);
          P->Read(M);

          m_Server.GetMotionSubsystem()->SetActivePath(P);

          PACKETHEADER Response;

          Response.MessageId = 0x80000003;
          Response.VariableLengthData = 0;

          S << Response.MessageId;
          S << Response.VariableLengthData;

          break;
        }

      case 0x00000004:
        /* GetMap */
        {
          TraceInfo(this, "RobotCommandConnection::GetMap()" );

          PACKETHEADER Response;

          Ptr<MemoryStream> M = new MemoryStream();
          m_Server.GetScanningSubsystem()->GetMap()->Write(M);

          Response.MessageId = 0x80000004;
          Response.VariableLengthData = (DWORD)(M->Length());

          S << Response.MessageId;
          S << Response.VariableLengthData;

          m_pClient->Write(&M->GetBuffer()[0], (SIZE_T)M->Length());

          break;
        }

      case 0x00000005:
        /* SetMap */
        {
          TraceInfo(this, "RobotCommandConnection::SetMap()" );

          PACKETHEADER Response;

          Response.MessageId = 0x80000005;
          Response.VariableLengthData = 0;

          S << Response.MessageId;
          S << Response.VariableLengthData;

          break;
        }
      case 0x00000006:
        /* GetDebugMessages */
        {
          PACKETHEADER Response;

          Trace.Lock();

          Response.MessageId = 0x80000006;
          Ptr<MemoryStream> DebugMessages = m_Server.GetDebugMessages();

          Response.VariableLengthData = (UINT)DebugMessages->Length();

          S << Response.MessageId;
          S << Response.VariableLengthData;

          m_pClient->Write(&DebugMessages->GetBuffer()[0], Response.VariableLengthData);

          DebugMessages->Clear();
          Trace.Unlock();

          break;
        }
      case 0x00000007:
        /* Drive */
        {
          FLOAT* pDistance = (FLOAT*)(&Packet[0]);
          FLOAT* pBias = (FLOAT*)(&Packet[4]);
          TraceInfo(this, "RobotCommandConnection::Drive(%f,%f)", *pDistance, *pBias );

          m_Server.GetMotionSubsystem()->Drive(*pDistance, *pBias);

          PACKETHEADER Response;

          Response.MessageId = 0x80000007;
          Response.VariableLengthData = 0;

          S << Response.MessageId;
          S << Response.VariableLengthData;

          break;
        }
      case 0x00000008:
        /* Turn */
        {
          FLOAT* pAngle = (FLOAT*)(&Packet[0]);
          TraceInfo(this, "RobotCommandConnection::Turn(%f)", *pAngle);

          m_Server.GetMotionSubsystem()->Turn(*pAngle);

          PACKETHEADER Response;

          Response.MessageId = 0x80000008;
          Response.VariableLengthData = 0;

          S << Response.MessageId;
          S << Response.VariableLengthData;

          break;
        }
      case 0x00000009:
        /* SetSpeedAndBias */
        {
          FLOAT* pSpeed = (FLOAT*)(&Packet[0]);
          FLOAT* pBias = (FLOAT*)(&Packet[4]);

          TraceInfo(this, "RobotCommandConnection::SetSpeedAndBias(%f, %f)", *pSpeed, *pBias);

          m_Server.GetMotionSubsystem()->SetSpeedAndBias(*pSpeed, *pBias);

          PACKETHEADER Response;

          Response.MessageId = 0x80000009;
          Response.VariableLengthData = 0;

          S << Response.MessageId;
          S << Response.VariableLengthData;

          break;
        }
      case 0x0000000a:
        /* Stop */
        {
		  

          TraceInfo(this, "RobotCommandConnection::Stop()");

          m_Server.GetMotionSubsystem()->Stop();

          PACKETHEADER Response;

          Response.MessageId = 0x8000000a;
          Response.VariableLengthData = 0;

          S << Response.MessageId;
          S << Response.VariableLengthData;
          break;
        }
      case 0x0000000b:
        /* Scan */
        {
          TraceInfo(this, "RobotCommandConnection::Scan()");

          // Read scan parameters
		  whatis(DEBUG_COMMAND_1, Request.VariableLengthData,%d);
          BYTE* pFieldOfViewHorizontal = (BYTE*)(&Packet[0]);
		  whatis(DEBUG_COMMAND_1,pFieldOfViewHorizontal,%x);
		  whatis(DEBUG_COMMAND_1,*pFieldOfViewHorizontal,%d);
          BYTE* pResolutionHorizontal = (BYTE*)(&Packet[1]);
          whatis(DEBUG_COMMAND_1,pResolutionHorizontal,%x);
		  whatis(DEBUG_COMMAND_1,*pResolutionHorizontal,%d);
		  INT* pScanLines = (INT*)(&Packet[2]);
		  whatis(DEBUG_COMMAND_1,pScanLines,%x);
			whatis(DEBUG_COMMAND_1,*pScanLines,%d);
          FLOAT* pDegreesPerScanLine = (FLOAT*)(&Packet[6]);
		  whatis(DEBUG_COMMAND_1,*pDegreesPerScanLine,%f);
          FLOAT* pBottomOfScan = (FLOAT*)(&Packet[10]);
		  whatis(DEBUG_COMMAND_1,*pBottomOfScan,%f);
          // Set scan parameters
          m_Server.GetScanningSubsystem()->HorizontalScanMode((INT)(*pFieldOfViewHorizontal),(INT)(*pResolutionHorizontal));
          m_Server.GetScanningSubsystem()->VerticalScanMode(*pDegreesPerScanLine,*pScanLines);
          m_Server.GetScanningSubsystem()->BottomOfScan(*pBottomOfScan);

          m_Server.GetScanningSubsystem()->EnqueueScan();

          PACKETHEADER Response;

          Response.MessageId = 0x8000000b;
          Response.VariableLengthData = 0;

          S << Response.MessageId;
          S << Response.VariableLengthData;
          break;
        }
      case 0x0000000c:
        /* GetTelemetry */
        {
          SendTelemetry();
          break;
        }
	  case 0x0000000d:
		  /* stop scanning*/{
		  m_Server.GetScanningSubsystem()->InterruptScan();

		  PACKETHEADER Response;

          Response.MessageId = 0x8000000d;
          Response.VariableLengthData = 0;

          S << Response.MessageId;
          S << Response.VariableLengthData;
		  break;
		  }
	  case 0x0000000e:
		  /*request single scan*/{

		  Buffer<WORD> Scan = m_Server.GetScanningSubsystem()->SingleScan((UINT)Packet[0]);

		  PACKETHEADER Response;

		  Response.MessageId = 0x8000000e;
          Response.VariableLengthData = Scan.Length();
		  		  
          S << Response.MessageId;
          S << Response.VariableLengthData;

		  m_pClient->Write(&Scan, Response.VariableLengthData);

		  break;
		  }
      case 0xffffffff:
        /* End Session */
        {
          TraceInfo(this, "RobotCommandConnection::Disconnect()");

          PACKETHEADER Response;

          Response.MessageId = 0xffffffff;
          Response.VariableLengthData = 0;

          S << Response.MessageId;
          S << Response.VariableLengthData;

          m_pClient->Close();

          return 0;
        }
      default:
        /* Send an error response and terminate the connection */
        {
          PACKETHEADER Response;

          Response.MessageId = 0xf0000000;
          Response.VariableLengthData = 0;

          S << Response.MessageId;
          S << Response.VariableLengthData;

          m_pClient->Close();
          break;
        }
      };
    }
  }
  catch(SocketException& E)
  {
    TraceError(this, "Command connection aborted: %s", E.ToString().ConstStr() );
  }

  //delete this;

  return 0;
}
//---------------------------------------------------------------------------------------------------
VOID RobotCommandConnection::SendTelemetry()
{
  BinaryStream S(m_pClient);

  ROBOTTELEMETRY Telemetry;
  ZeroMemory(&Telemetry, sizeof(Telemetry));

  /* Set the Time */
  Telemetry.CurrentTime = DateTime::Now();
  Telemetry.StartTime = m_Server.GetStartTime();

  /* Set the Temperature subsystem values */
  for (UINT i=0; i<8; i++)
  {
    Telemetry.Temperature[i] = 0.0f;//m_Server.GetClimateSubsystem()->GetZoneTemperature(i);
    Telemetry.HeaterActive[i] = FALSE;//m_Server.GetClimateSubsystem()->GetZoneHeaterActive(i);
    Telemetry.TargetTemperature[i] = 0.0f;//m_Server.GetClimateSubsystem()->GetZoneTargetTemperature(i);
  }

  /* Set the Motion subsystem values */
  Ptr<RobotMotion> pMotion = m_Server.GetMotionSubsystem();
  Telemetry.Position = pMotion->GetPosition();
  Telemetry.PrimaryAxis = pMotion->GetPrimaryAxis();
  Telemetry.Heading = pMotion->GetHeading();

  Telemetry.TrackTicks[0] = pMotion->GetTrackTicks(0);
  Telemetry.TrackTicks[1] = pMotion->GetTrackTicks(1);
  Telemetry.TrackSpeed[0] = pMotion->GetTrackSpeed(0);
  Telemetry.TrackSpeed[1] = pMotion->GetTrackSpeed(1);

  Telemetry.MotionStatus = pMotion->GetStatus();

  //Ptr<Image> pImage;// = 0; //m_Server.GetVisionSubsystem()->GetLatestImage();
  Ptr<RangeImage> pRangeImage = m_Server.GetScanningSubsystem()->GetLatestScan();
  Ptr<Map> pMap = m_Server.GetScanningSubsystem()->GetMap();
  Ptr<Path> pPath = m_Server.GetMotionSubsystem()->GetActivePath();

  Buffer<BYTE> ImageData;
  Buffer<BYTE> RangeImageData;
  Buffer<BYTE> MapData;
  Buffer<BYTE> PathData;

  /*
     if (pImage != 0 && m_LastTransmittedImage < pImage->SequenceNumber())
     {
     TraceInfo(this, "Transmitting Image %d", pImage->SequenceNumber());

     Ptr<MemoryStream> M = new MemoryStream();
     pImage->Write(M);

     ImageData = M->GetBuffer();
     m_LastTransmittedImage = pImage->SequenceNumber();
     }
     */

  if (pRangeImage != 0 && m_LastTransmittedRangeImage < pRangeImage->SequenceNumber())
  {
    TraceInfo(this, "Transmitting RangeImage %d", pRangeImage->SequenceNumber());

    Ptr<MemoryStream> M = new MemoryStream();
    pRangeImage->Write(M);

    RangeImageData = M->GetBuffer();

    m_LastTransmittedRangeImage = pRangeImage->SequenceNumber();

    TraceInfo(this, "Transmitting Map");

    M = new MemoryStream();
    pMap->Write(M);

    MapData = M->GetBuffer();
  }

  if (pPath != 0)
  {
    TraceInfo(this, "Transmitting Path");

    Ptr<MemoryStream> M = new MemoryStream();
    pPath->Write(M);

    PathData = M->GetBuffer();
  }

  Telemetry.ImageDataLength = (UINT)ImageData.Length();
  Telemetry.ScanDataLength = (UINT)RangeImageData.Length();
  Telemetry.PathDataLength = (UINT)PathData.Length();
  Telemetry.MapDataLength = (UINT)MapData.Length();

  PACKETHEADER Response;

  Response.MessageId = 0x8000000c;
  Response.VariableLengthData = (DWORD)(sizeof(ROBOTTELEMETRY) + Telemetry.ImageDataLength + Telemetry.ScanDataLength + Telemetry.PathDataLength + Telemetry.MapDataLength);

  S << Response.MessageId;
  S << Response.VariableLengthData;

  S << Telemetry.MotionStatus;
  S << Telemetry.VisionStatus;
  S << Telemetry.NavigationStatus;
  S << Telemetry.ClimateStatus;

  S << Telemetry.CurrentTime;
  S << Telemetry.StartTime;

  S << Telemetry.Position[0];
  S << Telemetry.Position[1];
  S << Telemetry.PrimaryAxis[0];
  S << Telemetry.PrimaryAxis[1];
  S << Telemetry.Heading;
  S << Telemetry.TrackTicks[0];
  S << Telemetry.TrackTicks[1];
  S << Telemetry.TrackSpeed[0];
  S << Telemetry.TrackSpeed[1];

  S << Telemetry.Temperature[0];
  S << Telemetry.Temperature[1];
  S << Telemetry.Temperature[2];
  S << Telemetry.Temperature[3];
  S << Telemetry.Temperature[4];
  S << Telemetry.Temperature[5];
  S << Telemetry.Temperature[6];
  S << Telemetry.Temperature[7];

  S << Telemetry.TargetTemperature[0];
  S << Telemetry.TargetTemperature[1];
  S << Telemetry.TargetTemperature[2];
  S << Telemetry.TargetTemperature[3];
  S << Telemetry.TargetTemperature[4];
  S << Telemetry.TargetTemperature[5];
  S << Telemetry.TargetTemperature[6];
  S << Telemetry.TargetTemperature[7];

  S << Telemetry.HeaterActive[0];
  S << Telemetry.HeaterActive[1];
  S << Telemetry.HeaterActive[2];
  S << Telemetry.HeaterActive[3];
  S << Telemetry.HeaterActive[4];
  S << Telemetry.HeaterActive[5];
  S << Telemetry.HeaterActive[6];
  S << Telemetry.HeaterActive[7];

  S << Telemetry.ImageDataLength;
  S << Telemetry.ScanDataLength;
  S << Telemetry.PathDataLength;
  S << Telemetry.MapDataLength;

  if (ImageData.Length() > 0)
    m_pClient->Write(ImageData.GetBuffer(), ImageData.Length());
  if (RangeImageData.Length() > 0)
    m_pClient->Write(RangeImageData.GetBuffer(), RangeImageData.Length());
  if (PathData.Length() > 0)
    m_pClient->Write(PathData.GetBuffer(), PathData.Length());
  if (MapData.Length() > 0)
    m_pClient->Write(MapData.GetBuffer(), MapData.Length());

}
 