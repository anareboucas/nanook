/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Test.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 166 $
 * Last Updated: $Date: 2007-08-10 14:46:23 -0400 (Fri, 10 Aug 2007) $
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

#include "System/ConsoleApplication.hpp"
using Bolitho::System::ConsoleApplication;

#include "IO/File.hpp"
using Bolitho::IO::File;
using Bolitho::IO::FileMode;

#include "IO/StdIO.hpp"
using Bolitho::StdIn;

class LADARTestApplication : public ConsoleApplication
{
public:
  virtual INT Main();
  virtual BOOL OnInit();

private:
  String m_MotorDeviceName;
  DWORD m_MotorBaudRate;
  String m_LADARDeviceName;
  DWORD m_LADARBaudRate;

};

ImplementApplicationEntryPoint(LADARTestApplication);


using Bolitho::StdOut;

#include "RobotScanning.hpp"

#if defined(PLATFORM_WINDOWS)
#include "Graphics/Image.hpp"
using Bolitho::Graphics::ImageBGRA32;
using Bolitho::Graphics::Color;
#endif

#include "Math/Math.hpp"
using Bolitho::Math::PI;

using Bolitho::Parse;

BOOL LADARTestApplication::OnInit()
{
  if (!ConsoleApplication::OnInit())
    return FALSE;

  m_CmdLine.DefineKeyedValue("MotorDeviceName", "MotorDeviceName", "The device name for the step motor controller", FALSE, "COM10");
  m_CmdLine.DefineKeyedValue("MotorBaudRate", "MotorBaudRate", "The baud rate for the step motor controller", FALSE, "9600");
  m_CmdLine.DefineKeyedValue("LADARDeviceName", "LADARDeviceName", "The device name for the LADAR controller", FALSE, "COM9");
  m_CmdLine.DefineKeyedValue("LADARBaudRate", "LADARBaudRate", "The baud rate for the LADAR controller", FALSE, "9600");
  m_CmdLine.DefineKeyedValue("MotorStepsPerScanline", "MotorStepsPerScanline", "The number of steps the step motor should take between scan lines", FALSE, "-4167");
  m_CmdLine.DefineKeyedValue("ScanLinesPerScan", "ScanLinesPerScan", "The number of scan lines in a complete range image", FALSE, "361");
  m_CmdLine.DefineKeyedValue("LADARPermBaud", "LADARPermBaud", "The permanent baud rate to program into the LADAR", FALSE,"9600");

  if (!ParseCommandLine())
    return FALSE;

  m_MotorDeviceName = m_CmdLine.GetValue<String>("MotorDeviceName");
  m_MotorBaudRate = m_CmdLine.GetValue<DWORD>("MotorBaudRate");
  m_LADARDeviceName = m_CmdLine.GetValue<String>("LADARDeviceName");
  m_LADARBaudRate = m_CmdLine.GetValue<DWORD>("LADARBaudRate");

  return TRUE;
}

INT LADARTestApplication::Main()
{
  UINT ScanCount = 0;
 
  INT StepsPerScanLine = m_CmdLine.GetValue<INT>("MotorStepsPerScanline");
  UINT ScanLinesPerScan = m_CmdLine.GetValue<UINT>("ScanLinesPerScan");

  Trace0Enter("");

  LADARMotorController M;
  LADAR L;

  if (m_CmdLine.IsPresent("MotorDeviceName"))
    M.Initialize(m_MotorDeviceName, m_MotorBaudRate);
  if (m_CmdLine.IsPresent("LADARDeviceName"))
    L.Initialize(m_LADARDeviceName, m_LADARBaudRate);


  if (m_CmdLine.IsPresent("LADARPermBaud"))
  {

    Trace("Testing LADAR...");
    L.RequestSingleScan();

    Trace("Reconfiguring LADAR Baud Rate");
    Trace("Changing to new baud rate (%d)...", m_CmdLine.GetValue<DWORD>("LADARPermBaud"));
    L.RequestBaudRate(m_CmdLine.GetValue<DWORD>("LADARPermBaud"));
    Trace("Testing LADAR...");
    L.RequestSingleScan();

    Trace("Making changed permanent...");
    L.MakeBaudRatePermanent();
    
    Trace("The LADAR has been reprogrammed.  TURN THE LADAR OFF AND ON AGAIN");

    return 0;
  }


  while (TRUE)
  {
    StdOut.Write(">");
    String Command = StdIn.ReadLine();
    Command.ToUpper();
    
    Array<String> Commands = Command.Split(String::IsWhitespace);

    if (Commands.Length() < 1)
      continue;

    if (Commands[0] == "EXIT" || Commands[0] == "QUIT")
      break;
  
    if (Commands[0] == "SCAN")
    {
      RangeImage* pRangeImage = new RangeImage(ScanCount, L.SamplesPerScanLine(), ScanLinesPerScan);

      for (UINT i=0; i<pRangeImage->Height(); i++)
      {
        Buffer<WORD> Scan = L.RequestSingleScan();
        FLOAT* pDestScanLine = pRangeImage->GetScanLine(i);

        for (UINT j=0; j<pRangeImage->Width(); j++)
          pDestScanLine[j] = Scan[j];

        M.MoveAndWait(StepsPerScanLine);
      }

      M.MoveAndWait(pRangeImage->Height() * -StepsPerScanLine);

	  #if defined(PLATFORM_WINDOWS) && 0
      FLOAT MinRangeValue = MAXFLOAT;
      FLOAT MaxRangeValue = MINFLOAT;

      for (UINT i=0; i<pRangeImage->Height(); i++)
      {
        for (UINT j=0; j<pRangeImage->Width(); j++)
        {
          if (pRangeImage->Get(i,j) < MinRangeValue)
            MinRangeValue = pRangeImage->Get(i,j);
          if (pRangeImage->Get(i,j) > MaxRangeValue)
            MaxRangeValue = pRangeImage->Get(i,j);
        }
      }

      ImageBGRA32 I = ImageBGRA32(pRangeImage->Width(), pRangeImage->Height());
      for (UINT i=0; i<I.Height(); i++)
      {
        for (UINT j=0; j<I.Width(); j++)
        {
          FLOAT v = (pRangeImage->Get(i,j) - MinRangeValue) / (MaxRangeValue-MinRangeValue);
          I(i,(I.Width()-j-1)) = Color( (BYTE)(v*255), (BYTE)(v*255), (BYTE)(v*255));
        }
      }
      I.Save(String::Format("Scan.%d.%d.bmp", Process::GetCurrentProcessId(), pRangeImage->SequenceNumber()) );

      for (UINT i=0; i<I.Height(); i++)
      {
        for (UINT j=0; j<I.Width(); j++)
        {
          FLOAT v = (pRangeImage->Get(i,j) - MinRangeValue) / (MaxRangeValue-MinRangeValue);
          I(i,(I.Width()-j-1)) = Color::FromHSV(v * 2 * PI, 0.8f, 0.8f);
        }
      }
      I.Save(String::Format("ScanHSV.%d.%d.bmp", Process::GetCurrentProcessId(), pRangeImage->SequenceNumber()) );
      #endif

      String Filename = String::Format("Logs\\Scan.%d.%d", Process::GetCurrentProcessId(), ScanCount );
      Trace("Saving scan to file \"%s\"", Filename.ConstStr());

      pRangeImage->Write(File::Open(Filename));

    }

    if (Commands[0] == "STEP")
    {
      INT Steps = Parse<INT>(Commands[1]);
      M.MoveAndWait(Steps);
    }

    if (Commands[0] == "SCANLINE")
    {
      Buffer<WORD> ScanLine = L.RequestSingleScan();
      for (SIZE_T i=0; i<ScanLine.Length(); i++)
        Trace("%d %d", i, ScanLine[i]);
    }
  
  }

  return 0;
}


