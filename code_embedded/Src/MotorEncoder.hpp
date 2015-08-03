/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/MotorEncoder.hpp $
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

#pragma once

#include "IO/File.hpp"
using Bolitho::IO::File;
using Bolitho::IO::FileMode;
using Bolitho::IO::FileAccess;

#include "IO/SerialPort.hpp"
using Bolitho::IO::SerialPort;

#if defined(MOTORENCODER_SIMULATION)
namespace NASA
{
  class MotorEncoder : public Object
  {
    DeclareRuntimeType();
  public:
    MotorEncoder();

    /* Initializes the MotorEncoder for use */
    VOID Initialize(LPCTSTR DeviceName, DWORD Baud);

    /* Reset the hardware counter to a known value */
    VOID ResetEncoder(UINT32 Value = 0);

    /* Reads a raw value from the encoder */
    UINT ReadEncoderValue();

    /* Reads the change in the counter from the previous call */
    INT ReadEncoderDelta();

  private:
    /* The last value read from the encoder */
    UINT m_LastValue;

    /* The current simulated value */
    UINT m_Value;

    /* The channel (left/right) this encoder is simulating */
    SIZE_T m_Channel;

  };
}
#else
namespace NASA
{
  class MotorEncoder : public Object
  {
    DeclareRuntimeType();
  public:
    MotorEncoder();

    /* Initializes the MotorEncoder for use */
    VOID Initialize(LPCTSTR DeviceName, DWORD Baud);

    /* Reset the hardware counter to a known value */
    VOID ResetEncoder(UINT32 Value = 0);

    /* Reads a raw value from the encoder */
    UINT ReadEncoderValue();

    /* Reads the change in the counter from the previous call */
    INT ReadEncoderDelta();

  private:
    /* The serial port used to communicate with the encoder */
    #if defined(SPACECUBE)
    Ptr<File> m_pEncoder;
    #else
    Ptr<SerialPort> m_pEncoder;
    #endif

    /* The last value read from the encoder */
    UINT m_LastValue;
  };
}
#endif
