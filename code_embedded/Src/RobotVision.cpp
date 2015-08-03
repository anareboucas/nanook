/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotVision.cpp $
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
#include "RobotVision.hpp"
#include "RobotServer.hpp"

#include "System/Configuration.hpp"
using Bolitho::System::Configuration;
using Bolitho::IO::SerialPortException;

#include "IO/File.hpp"
using Bolitho::IO::File;
using Bolitho::IO::FileMode;

#include "System/Process.hpp"
using Bolitho::System::Process;

//----------------------------------------------------------------------------------------------------------------------------
RobotVision::RobotVision(RobotServer& RobotServer) : m_RobotServer(RobotServer)
{
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL RobotVision::Initialize()
{
  Trace0Enter("");

  m_State = 0;
  m_NextSequenceNumber = 2;
  m_pLatestImage = new Image(1, 640, 480);

  if (Configuration::GetValue<DWORD>("RobotVision.Enabled"))
  {
#if defined(PLATFORM_WINDOWS)
    try
    {
      Array<StringPair> CaptureDevices = StillImageCapture::EnumerateCaptureDevices();
      for (SIZE_T i=0; i<CaptureDevices.Length(); i++)
        TraceInfo(this, "Available StillImageCapture Device: %s", CaptureDevices[i].First.ConstStr());

      m_pImageCapture = new StillImageCapture
        (
        Configuration::GetValue<String>("RobotVision.StillImageCapture.DeviceName"),
        Configuration::GetValue<UINT>("RobotVision.StillImageCapture.ImageWidth"),
        Configuration::GetValue<UINT>("RobotVision.StillImageCapture.ImageHeight")
        );
    }
    catch(Exception& E)
    {
      TraceError(this, "Failed to initialize StillImageCapture\n%s", E.ToString().ConstStr() ); 
      return FALSE;
    }
#endif
  }

  return Subsystem::Initialize();
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RobotVision::Destroy()
{
  Trace0Enter("");
  
  #if defined(PLATFORM_WINDOWS)
  m_pImageCapture = 0;
  #endif
  
  Subsystem::Destroy();
}
//----------------------------------------------------------------------------------------------------------------------------
INT RobotVision::ThreadMain()
{
  #if defined(PLATFORM_WINDOWS)
  if (m_pImageCapture)
    m_pImageCapture->BeginRawCapture(FALSE);

  while(TRUE)
  {
    if (m_pImageCapture)
    {
      Ptr<File> pFile = File::Open(String::Format("Logs\\Robot.%d.%d.Image", Process::GetCurrentProcessId(), m_NextSequenceNumber), FileMode::CREATE);
      m_pLatestImage = AquireImage();
      m_pLatestImage->Write(pFile);
    }

    ResetWatchdogTimer();
    Thread::Sleep(Configuration::GetValue<DWORD>("RobotVision.Delay"));
  }

  if (m_pImageCapture)
    m_pImageCapture->EndRawCapture();
  #else

  while(TRUE)
  {
    ResetWatchdogTimer();
    Thread::Sleep(Configuration::GetValue<DWORD>("RobotVision.Delay"));
  }

  #endif


}
//----------------------------------------------------------------------------------------------------------------------------
Ptr<Image> RobotVision::AquireImage()
{
  Trace2Enter("");

  #if defined(PLATFORM_WINDOWS)
  Image* pImage = new Image(m_NextSequenceNumber++, m_pImageCapture->Width(), m_pImageCapture->Height());
  
  Buffer<BYTE> I = m_pImageCapture->GetLastRawFrame();
  for (SIZE_T i=0; i<pImage->Height(); i++)
    CopyMemory(pImage->GetScanLine(i), &I[(pImage->Height()-i-1)*pImage->Width()*3], pImage->Width()*3);

  pImage->AquiredOn() = DateTime::Now();
  
  Ptr<RobotMotion> pMotion = m_RobotServer.GetMotionSubsystem();
  pImage->CameraPosition() = pMotion->GetPosition();
  pImage->CameraPrimaryAxis() = pMotion->GetPrimaryAxis();

  return pImage;
  #endif
}
//----------------------------------------------------------------------------------------------------------------------------
Ptr<Image> RobotVision::GetLatestImage()
{
  return m_pLatestImage;
}
//----------------------------------------------------------------------------------------------------------------------------


