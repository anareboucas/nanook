/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RobotCommand.hpp $
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
#include "TemperatureControl.hpp"
#include "Subsystem.hpp"

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
  class RobotServer;

  /*  */
  class RobotCommandServer : public Subsystem
  {
    DeclareRuntimeType();
  public:
    RobotCommandServer(RobotServer& Server);

    virtual BOOL Initialize();
    virtual VOID Destroy();

  protected:
    virtual INT ThreadMain();

    /* The Robot Server */
    RobotServer& m_Server;

    #if defined(SPACECUBE)
    /* The serial port used to listen for commands */
    Ptr<SerialPort> m_pServerPort;
    #else
    /* The socket used to listen for command connections */
    Ptr<Socket> m_pServerSocket;
    #endif

  };

  /* Represents an active connection to the telemetry server from a client */
  class RobotCommandConnection : public Runnable
  {
    DeclareRuntimeType();
  public:
    RobotCommandConnection(Ptr<Stream> Client, RobotServer& Server, BOOL RunOnThread);

    virtual INT ThreadMain();
  protected:
    /* The socket connection to the client */
    Ptr<Stream> m_pClient;

    /* The thread used to communicate to the client */
    Ptr<Thread> m_pThread;

    /* The Robot Server */
    RobotServer& m_Server;

    struct PACKETHEADER
    {
      DWORD MessageId;
      UINT VariableLengthData;
    };

    /* Generates and sends the latest telemetry to the client */
    VOID SendTelemetry();

    /* The sequence numbers of the last transmitted image and range images via GetTelemetry */
    UINT m_LastTransmittedImage;
    UINT m_LastTransmittedRangeImage;
  };

}

