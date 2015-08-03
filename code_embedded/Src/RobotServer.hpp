/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotServer.hpp $
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

#pragma once

#include "TemperatureControl.hpp"

#include "RobotMotion.hpp"
#include "RobotNavigation.hpp"
#include "RobotScanning.hpp"
#include "RobotTelemetry.hpp"
#include "RobotCommand.hpp"

#include "DeviceControl.hpp"
#include "PathPlanning.hpp"
#include "Map.hpp"

#include "System/ConsoleApplication.hpp"
using Bolitho::System::ConsoleApplication;

#include "IO/MemoryStream.hpp"
using Bolitho::IO::MemoryStream;
using Bolitho::IO::BinaryStream;
using Bolitho::IO::Stream;

namespace NASA
{
  /* The main class for the robot.  Initializes all subsystems, and listens for connections from a controller */
  class RobotServer : public ConsoleApplication
  {
  public:
    RobotServer();

    virtual INT Main();
    virtual BOOL OnInit();
    virtual VOID OnExit();

    Ptr<RobotClimateControl> GetClimateSubsystem()
    { return m_pClimateControl; }

    Ptr<RobotMotion> GetMotionSubsystem()
    { return m_pMotion; }

    Ptr<RobotScanning> GetScanningSubsystem()
    { return m_pScanning; }

    Ptr<MemoryStream> GetDebugMessages()
    { return m_DebugLog; }

    DateTime GetStartTime() CONST
    { return m_StartTime; }

  protected:
    /* A buffer for storing all debug messages */
    Ptr<MemoryStream> m_DebugLog;

    /* The Robot Temperature Management Subsystem */
    Ptr<RobotClimateControl> m_pClimateControl;
    /* The Robot Motion Subsystem */
    Ptr<RobotMotion> m_pMotion;
    /* The Robot Navigation Subsystem */
    Ptr<RobotNavigation> m_pNavigation;
    /* The Robot Scanning Subsystem */
    Ptr<RobotScanning> m_pScanning;

    /* The telemetry subsystem */
    Ptr<RobotTelemetryServer> m_pTelemetryServer;
    /* The command subsystem */
    Ptr<RobotCommandServer> m_pCommandServer;

    /* The A2D and relay control board */
    Ptr<DeviceController> m_pDeviceControl;

    /* The time the robot started */
    DateTime m_StartTime;
  };
}

