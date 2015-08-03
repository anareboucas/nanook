/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/DeviceControl.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 260 $
 * Last Updated: $Date: 2007-11-07 15:21:03 -0500 (Wed, 07 Nov 2007) $
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

#pragma once

#include "IO/SerialPort.hpp"
using Bolitho::IO::SerialPort;

#if defined(DEVICECONTROL_SIMULATION)
namespace NASA
{
  class DeviceController : public Object
  {
    DeclareRuntimeType();
  public:
    /* Initializes the device controller */
    BOOL Initialize(LPCTSTR DeviceName, DWORD BaudRate, BYTE RelayBoardAddress, BYTE AnalogDigitalBoardAddress);

    /* Gets the state of a relay circuit switch */
    BOOL GetRelayState(SIZE_T i);

    /* Sets the state of a relay circuit switch */
    VOID SetRelayState(SIZE_T i, BOOL b);

    /* Sets the state of all relay circuits */
    VOID SetRelayState(BOOL b);

    /* Reads an analogue input */
    INT GetAnalog(SIZE_T i);

    /* Reads a digital input */ 
    BOOL GetDigital(SIZE_T i);

    /* Sets a digital output */
    VOID SetDigital(SIZE_T i, BOOL b);

  private:
    BOOL m_DigitalState[32];
    BOOL m_RelayState[32];
    INT m_AnalogState[32];
  };

}
#else

namespace NASA
{
  class DeviceController : public Object
  {
    DeclareRuntimeType();
  public:
    /* Initializes the device controller */
    BOOL Initialize(LPCTSTR DeviceName, DWORD BaudRate, BYTE RelayBoardAddress, BYTE AnalogDigitalBoardAddress);

    /* Gets the state of a relay circuit switch */
    BOOL GetRelayState(SIZE_T i);

    /* Sets the state of a relay circuit switch */
    VOID SetRelayState(SIZE_T i, BOOL b);

    /* Sets the state of all relay circuits */
    VOID SetRelayState(BOOL b);

    /* Reads an analogue input */
    INT GetAnalog(SIZE_T i);

    /* Reads a digital input */ 
    BOOL GetDigital(SIZE_T i);

    /* Sets a digital output */
    VOID SetDigital(SIZE_T i, BOOL b);

  private:
    Ptr<SerialPort> m_pPort;

    BYTE m_RelayBoardAddress;
    BYTE m_AnalogDigitalBoardAddress;
  };

}

#endif

