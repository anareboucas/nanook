/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotTelemetry.hpp $
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

#include "RobotMotion.hpp"
#include "RobotNavigation.hpp"

#include "System/Threading/Thread.hpp"
using Bolitho::System::Thread;
using Bolitho::System::Runnable;

#include "Time/DateTime.hpp"
using Bolitho::DateTime;

#include "Math/Matrix3.hpp"
using Bolitho::Math::Matrix3;
using Bolitho::Math::Vector3;

#include "Math/Vector2.hpp"
using Bolitho::Math::Vector2;

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

namespace NASA
{
  #pragma pack(push, 1)
  struct ROBOTTELEMETRY
  {
    DWORD MotionStatus;
    DWORD VisionStatus;
    DWORD NavigationStatus;
    DWORD ClimateStatus;

    ULONGLONG CurrentTime;
    ULONGLONG StartTime;

    /* Motion subsystem information */
    Vector2F Position;
    Vector2F PrimaryAxis;
    FLOAT Heading;
    UINT64 TrackTicks[2];
    FLOAT TrackSpeed[2];

    /* Temperature subsystem information */
    FLOAT Temperature[8];
    FLOAT TargetTemperature[8];
    BOOL HeaterActive[8];

    UINT ImageDataLength;
    UINT ScanDataLength;
    UINT PathDataLength;
    UINT MapDataLength;
  };
  #pragma pack(pop)

  class RobotServer;
  class RobotTelemetryServer : public Subsystem
  {
    DeclareRuntimeType();
  public:
    RobotTelemetryServer(RobotServer& Server);

    virtual BOOL Initialize();
    virtual VOID Destroy();

  protected:
    virtual INT ThreadMain();

    /* The socket used to listen on */
    Ptr<Socket> m_pServerSocket;

    /* The Robot Server */
    RobotServer& m_Server;

    /* The next connection number */
    SIZE_T m_ConnectionNumber;
  };

  /* Represents an active connection to the telemetry server from a client */
  class RobotTelemetryConnection : public Runnable
  {
    DeclareRuntimeType();
  public:
    RobotTelemetryConnection(Ptr<Stream> pClient, RobotServer& Server);

  protected:
    virtual INT ThreadMain();

    /* The socket connection to the client */
    Ptr<Stream> m_pClient;

    /* The thread used to communicate with the client */
    Ptr<Thread> m_pThread;

    /* The Robot Server */
    RobotServer& m_Server;
  };
}

