/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/MotorEncoder.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 297 $
 * Last Updated: $Date: 2008-01-23 20:39:27 -0500 (Wed, 23 Jan 2008) $
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
#include "MotorEncoder.hpp"

#include "Memory/Memory.hpp"
using Bolitho::SwapByteOrder;

using Bolitho::IO::Parity;
using Bolitho::IO::StopBits;
using Bolitho::TimeoutException;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,MotorEncoder,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#if defined(MOTORENCODER_SIMULATION)

#include "System/Configuration.hpp"
using Bolitho::System::Configuration;
#include "MotorControl.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------------
MotorEncoder::MotorEncoder()
{
  m_LastValue = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MotorEncoder::Initialize(LPCTSTR DeviceName, DWORD Baud)
{
  Trace0Enter("\"%s\", %d", DeviceName, Baud);

  if (Configuration::GetValue<String>("RobotMotion.MotorEncoders.Left.DeviceName") == DeviceName)
    m_Channel = 0;
  if (Configuration::GetValue<String>("RobotMotion.MotorEncoders.Right.DeviceName") == DeviceName)
    m_Channel = 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MotorEncoder::ResetEncoder(UINT32 Value)
{
  Trace1Enter("%u", Value);

  m_Value = Value;
  m_LastValue = Value;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT32 MotorEncoder::ReadEncoderValue()
{
  Trace2Enter("");

  if (MotorController::m_pActiveController)
  {
    FLOAT v = MotorController::m_pActiveController->GetSpeed(m_Channel);
    m_Value += (UINT32)(v * 500);
  }

  Trace2Return("%u", m_Value);
  return m_Value;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT MotorEncoder::ReadEncoderDelta()
{
  Trace2Enter("");

  UINT32 Value = ReadEncoderValue();
  INT Delta = 0;
  if ((Value & 0x800000) != (m_LastValue & 0x800000))
  {
    if (Value > m_LastValue)
      Delta = m_LastValue + (0xffffff - Value);
    else
      Delta = Value + (0xffffff - m_LastValue);
  }
  else
  {
    Delta = Value - m_LastValue;
  }

  m_LastValue = Value;

  Trace2Return("%d", Delta);
  return Delta;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#else
//-----------------------------------------------------------------------------------------------------------------------------------------------------
MotorEncoder::MotorEncoder()
{
  m_LastValue = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MotorEncoder::Initialize(LPCTSTR DeviceName, DWORD Baud)
{
  Trace0Enter("\"%s\", %d", DeviceName, Baud);
  
  #if defined(SPACECUBE)
  m_pEncoder = File::Open(DeviceName, FileMode::OPEN, FileAccess::READWRITE);
  #else
  m_pEncoder = SerialPort::Open(DeviceName);
  m_pEncoder->SetBaudRate(Baud);
  m_pEncoder->SetParity(Parity::NONE);
  m_pEncoder->SetStopBits(StopBits::ONE);
  m_pEncoder->SetTimeout(1000, 1000);
  m_pEncoder->Purge();
  #endif

  BYTE Message = 'A';
  BYTE Response = 0;

  for (SIZE_T i=0; i<10; i++)
  {
    m_pEncoder->Write(&Message, sizeof(BYTE));
    if (m_pEncoder->Read(&Response, sizeof(BYTE)) == sizeof(BYTE))
      break;
    if (i == 9)
      throw TimeoutException(this, String::Format((LPCTSTR) "The motor encoder failed to respond on device \"%s\" at baud %d", DeviceName, Baud));
  }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID MotorEncoder::ResetEncoder(UINT32 Value)
{
  Trace1Enter("%u", Value);

  struct
  {
    BYTE Command;
    BYTE Quadrature;
    DWORD Value;
  } Message;

  Message.Command = 0x22;
  Message.Quadrature = 0;
  Message.Value = Value & 0x00ffffff;

  #if defined(ARCH_BIGENDIAN)
  SwapByteOrder(&Message.Value);
  #endif

  m_pEncoder->Write(&Message, sizeof(Message));
  m_pEncoder->Read(&Message, sizeof(BYTE));

  m_LastValue = Value;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
UINT32 MotorEncoder::ReadEncoderValue()
{
  Trace2Enter("");

  if (m_pEncoder == 0)
    return 0;

  BYTE Message = 0x01;
  UINT32 Value = 0;

  m_pEncoder->Write(&Message, sizeof(BYTE));
  m_pEncoder->Read(&Value, sizeof(UINT32));
  
  #if defined(ARCH_BIGENDIAN)
  SwapByteOrder(&Value);
  #endif

  Value &= 0x00ffffff;
  
  Trace2Return("%u", Value);
  return Value;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT MotorEncoder::ReadEncoderDelta()
{
  Trace2Enter("");

  if (m_pEncoder == 0)
    return 0;

  UINT32 Value = ReadEncoderValue();
  INT Delta = 0;
  if ((Value & 0x800000) != (m_LastValue & 0x800000))
  {
    if (Value > m_LastValue)
      Delta = m_LastValue + (0xffffff - Value);
    else
      Delta = Value + (0xffffff - m_LastValue);
  }
  else
  {
    Delta = Value - m_LastValue;
  }

  m_LastValue = Value;

  Trace2Return("%d", Delta);
  return Delta;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#endif
