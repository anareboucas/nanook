/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotTelemetry.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 280 $
 * Last Updated: $Date: 2007-12-03 09:56:28 -0500 (Mon, 03 Dec 2007) $
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

#include "RobotTelemetry.hpp"
#include "RobotServer.hpp"

#include "System/Configuration.hpp"
using Bolitho::System::Configuration;

#include "Net/SocketStream.hpp"
using Bolitho::Net::Socket;
using Bolitho::Net::SocketStream;
using Bolitho::Net::IPAddress;
using Bolitho::Net::IPEndPoint;
using Bolitho::Net::AddressFamily;
using Bolitho::Net::SocketType;
using Bolitho::Net::ProtocolType;
using Bolitho::Net::SocketException;

using Bolitho::Buffer;

#include "IO/File.hpp"
using Bolitho::IO::File;

//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,RobotTelemetryServer,Runnable);
ImplementRuntimeType(NASA,RobotTelemetryConnection,Runnable);
//----------------------------------------------------------------------------------------------------------------------------
RobotTelemetryServer::RobotTelemetryServer(RobotServer& Server) : m_Server(Server)
{
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL RobotTelemetryServer::Initialize()
{
  try
  {
    m_pServerSocket = new Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);

    m_pServerSocket->Bind(IPEndPoint(IPAddress::Any(), Configuration::GetValue<UINT>("RobotTelemetryServer.Port") ));
    m_pServerSocket->Listen();
    TraceInfo(this, "Listening for telemetry connections on %s", m_pServerSocket->EndPoint().ToString().ConstStr() );
  }
  catch(SocketException& E)
  {
    TraceError(this, "Failed to create server socket: %s", E.ToString().ConstStr() );
    return FALSE;
  }

  return Subsystem::Initialize();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotTelemetryServer::Destroy()
{
  Subsystem::Destroy();
}
//----------------------------------------------------------------------------------------------------------------------------
INT RobotTelemetryServer::ThreadMain()
{
  Trace0Enter("");

  if (Configuration::GetValue<DWORD>("RobotTelemetry.EnableGlobalTelemetryLog"))
    new RobotTelemetryConnection(File::Open("Logs\\Robot.%d.Telemetry"), m_Server);

  while (TRUE)
  {
    Ptr<Socket> Client;
    try
    {
      Client = m_pServerSocket->Accept();
      TraceInfo(this, String::Format("Accepted telemetry connection from '%s'", Client->EndPoint().Address().ToString().ConstStr() ));
      new RobotTelemetryConnection(new SocketStream(Client), m_Server);
    }
    catch(SocketException&)
    {
    }
  }

}
//----------------------------------------------------------------------------------------------------------------------------
RobotTelemetryConnection::RobotTelemetryConnection(Ptr<Stream> pClient, RobotServer& Server) : m_pClient(pClient), m_Server(Server)
{
  m_pThread = Thread::Create(this);
}
//----------------------------------------------------------------------------------------------------------------------------
INT RobotTelemetryConnection::ThreadMain()
{
  Trace0Enter("");
  
  SIZE_T LastTransmittedImage = 0;
  SIZE_T LastTransmittedRangeImage = 0;

  try
  {
    while (TRUE)
    {
      struct PACKETHEADER
      {
        DWORD MessageId;
        UINT VariableLengthData;
      };

      ROBOTTELEMETRY Telemetry;
      ZeroMemory(&Telemetry, sizeof(Telemetry));

      /* Set the Time */
      Telemetry.CurrentTime = DateTime::Now();
      Telemetry.StartTime = m_Server.GetStartTime();

      /* Set the Temperature subsystem values */
      for (UINT i=0; i<8; i++)
      {
        Telemetry.Temperature[i] = m_Server.GetClimateSubsystem()->GetZoneTemperature(i);
        Telemetry.HeaterActive[i] = m_Server.GetClimateSubsystem()->GetZoneHeaterActive(i);
        Telemetry.TargetTemperature[i] = m_Server.GetClimateSubsystem()->GetZoneTargetTemperature(i);
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

      Ptr<Image> pImage = m_Server.GetVisionSubsystem()->GetLatestImage();
      Ptr<RangeImage> pRangeImage = m_Server.GetScanningSubsystem()->GetLatestScan();
      Ptr<Map> pMap = m_Server.GetScanningSubsystem()->GetMap();
      Ptr<Path> pPath = m_Server.GetMotionSubsystem()->GetActivePath();

      Buffer<BYTE> ImageData;
      Buffer<BYTE> RangeImageData;
      Buffer<BYTE> MapData;
      Buffer<BYTE> PathData;

      if (pImage != 0 && LastTransmittedImage < pImage->SequenceNumber())
      {
        TraceInfo(this, "Transmitting Image %d", pImage->SequenceNumber());

        Ptr<MemoryStream> M = new MemoryStream();
        pImage->Write(M);

        ImageData = M->GetBuffer();
        LastTransmittedImage = pImage->SequenceNumber();
      }

      if (pRangeImage != 0 && LastTransmittedRangeImage < pRangeImage->SequenceNumber())
      {
        TraceInfo(this, "Transmitting RangeImage %d", pRangeImage->SequenceNumber());

        Ptr<MemoryStream> M = new MemoryStream();
        pRangeImage->Write(M);

        RangeImageData = M->GetBuffer();

        LastTransmittedRangeImage = pRangeImage->SequenceNumber();

        TraceInfo(this, "Transmitting Map");

        M = new MemoryStream();
        pMap->Write(M);
        
        MapData = M->GetBuffer();
      }

      if (pPath != 0)
      {
        TraceInfo(this, "Transmitting Image %d", pImage->SequenceNumber());

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

      m_pClient->Write(&Response, sizeof(PACKETHEADER));
      m_pClient->Write(&Telemetry, sizeof(ROBOTTELEMETRY));
      
      if (ImageData.Length() > 0)
        m_pClient->Write(ImageData, ImageData.Length());
      if (RangeImageData.Length() > 0)
        m_pClient->Write(RangeImageData, RangeImageData.Length());
      if (PathData.Length() > 0)
        m_pClient->Write(PathData, PathData.Length());
      if (MapData.Length() > 0)
        m_pClient->Write(MapData, MapData.Length());


      Thread::Sleep(Configuration::GetValue<DWORD>("RobotTelemetryServer.Delay"));
    }

  }
  catch(Exception& E)
  {
    TraceError(this, "Telemetry Collection Failed %s", E.ToString().ConstStr());
  }

  return 0;
}

