/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/MotorControl.hpp $
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

#include "IO/SerialPort.hpp"
using Bolitho::IO::SerialPort;
using Bolitho::IO::StopBits;
using Bolitho::IO::Parity;

#include "IO/File.hpp"
using Bolitho::IO::File;
using Bolitho::IO::FileMode;
using Bolitho::IO::FileAccess;

#if defined(MOTORCONTROLLER_SIMULATION)
namespace NASA
{
  struct MotorChannel
  { enum { LEFT = 0, RIGHT = 1 }; };

  /* A "driver" class for the main motor controllers */
  class MotorController : public Object
  {
    DeclareRuntimeType();
  public:
    MotorController();

    /* Initialize the motor controller for use.  Connects via the device specified in 'DeviceName'. */
    VOID Initialize(LPCTSTR DeviceName, DWORD Baud);

    /* Returns the track speed value */
    FLOAT GetSpeed(SIZE_T i) CONST
    { return m_CurrentSpeed[i]; }

    /* Wrapper from SendMotorControllerMessage */
    VOID SetTrackSpeed(SIZE_T i, FLOAT v);

    /* Halts all current motion */
    VOID Stop();

    static MotorController* m_pActiveController;
  protected:

    /* The current speed of each motor */
    FLOAT m_CurrentSpeed[2];

  };
}
#else
namespace NASA
{
  struct MotorChannel
  { enum { LEFT = 0, RIGHT = 1 }; };

  /* A "driver" class for the main motor controllers */
  class MotorController : public Object
  {
    DeclareRuntimeType();
  public:
    MotorController();

    /* Initialize the motor controller for use.  Connects via the device specified in 'DeviceName'. */
    VOID Initialize(LPCTSTR DeviceName, DWORD Baud);

    /* Returns the track speed value */
    FLOAT GetSpeed(SIZE_T i) CONST
    { return m_CurrentSpeed[i]; }

    /* Wrapper from SendMotorControllerMessage */
    VOID SetTrackSpeed(SIZE_T i, FLOAT v);

    /* Halts all current motion */
    VOID Stop();
  protected:
    #if defined(SPACECUBE)
    Ptr<File> m_pMotorControllerPort;
    #else
    Ptr<SerialPort> m_pMotorControllerPort;
    #endif

    /* Send a command to a motor controller */
    VOID SendMotorControllerMessage(INT LeftSpeed, INT RightSpeed);

    /* The current speed of each motor */
    FLOAT m_CurrentSpeed[2];

  };
}
#endif
