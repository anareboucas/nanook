/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/MotorControl.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 303 $
 * Last Updated: $Date: 2008-02-08 11:17:35 -0500 (Fri, 08 Feb 2008) $
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
#include "MotorControl.hpp"

#include "Math/Math.hpp"
using Bolitho::Math::Abs;
using Bolitho::Math::Sign;

#include "System/Threading/Thread.hpp"
using Bolitho::System::Thread;

#include "System/Configuration.hpp"
using Bolitho::System::Configuration;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,MotorController,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#if defined(MOTORCONTROLLER_SIMULATION)

//-----------------------------------------------------------------------------------------------------------------------------------------------------
MotorController* MotorController::m_pActiveController = 0;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MotorController::MotorController()
{
  m_CurrentSpeed[0] = 0.0f;
  m_CurrentSpeed[1] = 0.0f;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MotorController::Initialize(LPCTSTR DeviceName, DWORD Baud)
{
  Trace0Enter("\"%s\", %d", DeviceName, Baud);
  m_pActiveController = this;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MotorController::Stop()
{
  Trace2Enter("");
  m_CurrentSpeed[0] = m_CurrentSpeed[1] = 0.0f;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MotorController::SetTrackSpeed(SIZE_T i, FLOAT v)
{
  Trace2Enter("%u, %f", i, v);

  /* Clamp v to range [-1.0,1.0] */
  if (v < -1.0f)
    v = -1.0f;
  if (v > 1.0f)
    v = 1.0f;

  /* Send left command, if applicable */
  if (i == MotorChannel::LEFT)
    m_CurrentSpeed[0] = v;  
  
  /* Send right command, if applicable */
  if (i == MotorChannel::RIGHT)
    m_CurrentSpeed[1] = v;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#else
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MotorController::MotorController()
{
  m_CurrentSpeed[0] = 0.0f;
  m_CurrentSpeed[1] = 0.0f;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MotorController::Initialize(LPCTSTR DeviceName, DWORD Baud)
{
  Trace0Enter("\"%s\", %d", DeviceName, Baud);

  #if defined(SPACECUBE)
  m_pMotorControllerPort = File::Open(DeviceName, FileMode::OPEN, FileAccess::READWRITE);
  #else  
  m_pMotorControllerPort = SerialPort::Open(DeviceName);
  m_pMotorControllerPort->SetBaudRate(Baud);
  m_pMotorControllerPort->SetParity(Parity::NONE);
  m_pMotorControllerPort->SetStopBits(StopBits::ONE);
  m_pMotorControllerPort->Purge();
  #endif
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MotorController::Stop()
{
  Trace2Enter("");

  BYTE Stop = 0;
  m_pMotorControllerPort->Write(&Stop, sizeof(BYTE));
  m_CurrentSpeed[0] = m_CurrentSpeed[1] = 0.0f;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MotorController::SetTrackSpeed(SIZE_T i, FLOAT v)
{
  Trace1Enter("%u, %f", i, v);

  /* Clamp v to range [-1.0,1.0] */
  if (v < -1.0f)
    v = -1.0f;
  if (v > 1.0f)
    v = 1.0f;

  /* Send left command, if applicable */
  if (i == MotorChannel::LEFT)
  {
    BYTE LeftSpeed = (BYTE)((v + 1.0f) * 63.0f) + 1;
    m_pMotorControllerPort->Write(&LeftSpeed, sizeof(BYTE));
    m_CurrentSpeed[0] = v;  
  }
  
  /* Send right command, if applicable */
  if (i == MotorChannel::RIGHT)
  {
    BYTE RightSpeed = (BYTE)((v + 1.0f) * 63.0f) + 1 | 0x80;
    m_pMotorControllerPort->Write(&RightSpeed, sizeof(BYTE));
    m_CurrentSpeed[1] = v;
  }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#endif

