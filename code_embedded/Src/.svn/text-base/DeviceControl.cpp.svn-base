/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/DeviceControl.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 258 $
 * Last Updated: $Date: 2007-11-05 15:44:15 -0500 (Mon, 05 Nov 2007) $
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
#include "DeviceControl.hpp"

using Bolitho::IO::StopBits;
using Bolitho::IO::Parity;
using Bolitho::IO::SerialPortException;

//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,DeviceController,Object);
//----------------------------------------------------------------------------------------------------------------------------

#if defined(DEVICECONTROL_SIMULATION)
BOOL DeviceController::Initialize(LPCTSTR DeviceName, DWORD BaudRate, BYTE RelayBoardAddress, BYTE AnalogDigitalBoardAddress)
{
  Trace0Enter("\"%s\", %d, %d, %d", DeviceName, BaudRate, RelayBoardAddress, AnalogDigitalBoardAddress);

  ZeroMemory(m_DigitalState, 32*sizeof(BOOL));
  ZeroMemory(m_RelayState, 32*sizeof(BOOL));
  ZeroMemory(m_AnalogState, 32*sizeof(INT));

  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL DeviceController::GetRelayState(SIZE_T i)
{
  Trace1Enter("%d", i);
  return m_RelayState[i];
}
//----------------------------------------------------------------------------------------------------------------------------
VOID DeviceController::SetRelayState(SIZE_T RelayNumber, BOOL Value)
{
  Trace1Enter("%d, %d", RelayNumber, Value);
  m_RelayState[RelayNumber] = Value;
}
//----------------------------------------------------------------------------------------------------------------------------
VOID DeviceController::SetRelayState(BOOL Value)
{
  Trace1Enter("%d", Value);
  for (SIZE_T i=0; i<32; i++)
    m_RelayState[i] = Value;
}
//----------------------------------------------------------------------------------------------------------------------------
INT DeviceController::GetAnalog(SIZE_T i)
{
  return m_AnalogState[i];
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL DeviceController::GetDigital(SIZE_T i)
{
  return m_DigitalState[i];
}
//----------------------------------------------------------------------------------------------------------------------------
VOID DeviceController::SetDigital(SIZE_T i, BOOL b)
{
  m_DigitalState[i] = b;
}
//----------------------------------------------------------------------------------------------------------------------------
#else
//----------------------------------------------------------------------------------------------------------------------------
BOOL DeviceController::Initialize(LPCTSTR DeviceName, DWORD BaudRate, BYTE RelayBoardAddress, BYTE AnalogDigitalBoardAddress)
{
  Trace0Enter("\"%s\", %d, %d, %d", DeviceName, BaudRate, RelayBoardAddress, AnalogDigitalBoardAddress);

  try
  {
    m_pPort = SerialPort::Open(DeviceName);
    m_pPort->SetBaudRate(BaudRate);
    m_pPort->SetStopBits(StopBits::ONE);
    m_pPort->SetParity(Parity::NONE);

    m_RelayBoardAddress = RelayBoardAddress;
    m_AnalogDigitalBoardAddress = AnalogDigitalBoardAddress;
  }
  catch(SerialPortException& E)
  {
    TraceError(this, "Failed to initialize device: %s", E.ToString().ConstStr());
    return FALSE;
  }

  /* Execute this command a number of times... */
  SetRelayState(FALSE);
  SetRelayState(FALSE);
  SetRelayState(FALSE);
  SetRelayState(FALSE);

  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL DeviceController::GetRelayState(SIZE_T i)
{
  Trace1Enter("%d", i);

  if (m_pPort == 0)
    return FALSE;

  StringA Message = StringA::Format("%dRPK%d\r", m_RelayBoardAddress, i);
  m_pPort->Write(Message.ConstStr(), Message.Length());

  while (TRUE)
  {
    BYTE B = 0;
    m_pPort->Read(&B, 1);

    if (B != 0x0d)
      return (B != 0x30);
  }

}
//----------------------------------------------------------------------------------------------------------------------------
VOID DeviceController::SetRelayState(SIZE_T RelayNumber, BOOL Value)
{
  Trace1Enter("%d, %d", RelayNumber, Value);

  if (m_pPort == 0)
    return;

  StringA Message;
  if (Value)
    Message = StringA::Format("%dSK%d\r", m_RelayBoardAddress, RelayNumber);
  else
    Message = StringA::Format("%dRK%d\r", m_RelayBoardAddress, RelayNumber);

  Trace(Message);
  m_pPort->Write(Message.ConstStr(), Message.Length());
}
//----------------------------------------------------------------------------------------------------------------------------
VOID DeviceController::SetRelayState(BOOL Value)
{
  Trace1Enter("%d", Value);

  if (m_pPort == 0)
    return;

  StringA Message;
  if (Value)
    Message = StringA::Format("%dSPK11111111\r", m_RelayBoardAddress);
  else
    Message = StringA::Format("%dSPK00000000\r", m_RelayBoardAddress);

  Trace(Message);
  m_pPort->Write(Message.ConstStr(), Message.Length());
}
//----------------------------------------------------------------------------------------------------------------------------
INT DeviceController::GetAnalog(SIZE_T i)
{
  return -1;
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL DeviceController::GetDigital(SIZE_T i)
{
  return FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------
VOID DeviceController::SetDigital(SIZE_T i, BOOL b)
{

}
//----------------------------------------------------------------------------------------------------------------------------
#endif
