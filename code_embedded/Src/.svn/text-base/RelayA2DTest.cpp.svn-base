/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Test.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 166 $
 * Last Updated: $Date: 2007-08-10 14:46:23 -0400 (Fri, 10 Aug 2007) $
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

#include "System/ConsoleApplication.hpp"
using Bolitho::System::ConsoleApplication;

#include "IO/File.hpp"
using Bolitho::IO::File;
using Bolitho::IO::FileMode;

#include "IO/StdIO.hpp"
using Bolitho::StdIn;
using Bolitho::StdOut;

using Bolitho::Parse;

class MotorEncoderTestApplication : public ConsoleApplication
{
protected:
  String m_DeviceName;
  DWORD m_BaudRate;

  BYTE m_RelayBoardAddress;
  BYTE m_AnalogDigitalAddress;

public:
  virtual INT Main();
  virtual BOOL OnInit();

};

ImplementApplicationEntryPoint(MotorEncoderTestApplication);

#include "DeviceControl.hpp"

BOOL MotorEncoderTestApplication::OnInit()
{
  Trace0Enter("");

  if (!ConsoleApplication::OnInit())
    return FALSE;

  m_CmdLine.DefineKeyedValue("DeviceName", "DeviceName", "The device name for the left encoder", TRUE);
  m_CmdLine.DefineKeyedValue("BaudRate", "BaudRate", "The baud rate to connect to the left encoder with", FALSE, "9600");
  
  m_CmdLine.DefineKeyedValue("RelayBoardAddress", "RelayBoardAddress", "The address of the relay board", FALSE, "1");
  m_CmdLine.DefineKeyedValue("AnalogDigitalAddress", "AnalogDigitalAddress", "The address of the analog/digital board", FALSE, "0");

  if (!ParseCommandLine())
    return FALSE;

  m_DeviceName = m_CmdLine.GetValue<String>("DeviceName");
  m_BaudRate = m_CmdLine.GetValue<DWORD>("BaudRate");
  m_RelayBoardAddress = (BYTE)m_CmdLine.GetValue<DWORD>("RelayBoardAddress");
  m_AnalogDigitalAddress = (BYTE)m_CmdLine.GetValue<DWORD>("AnalogDigitalAddress");

  return TRUE;
}

INT MotorEncoderTestApplication::Main()
{
  Trace0Enter("");

  DeviceController DevCtrl;

  DevCtrl.Initialize(m_DeviceName, m_BaudRate, m_RelayBoardAddress, m_AnalogDigitalAddress);

  StdOut.WriteLine("Commands are:");
  StdOut.WriteLine("EXIT");
  StdOut.WriteLine("QUIT");
  StdOut.WriteLine("VALUE [LEFT|RIGHT]");
  StdOut.WriteLine("DELTA [LEFT|RIGHT]");
  StdOut.WriteLine("READ [LEFT|RIGHT]");
  StdOut.WriteLine("REALTIME");

  while (TRUE)
  {
    StdOut.Write(">");
    String Command = StdIn.ReadLine();
    Command.ToUpper();
    
    Array<String> Commands = Command.Split(String::IsWhitespace);

    if (Commands.Length() < 1)
      continue;

    if (Commands[0] == "EXIT" || Commands[0] == "QUIT")
      break;

    if (Commands[0] == "HELP")
    {
      StdOut.WriteLine("Commands are:");
      StdOut.WriteLine("EXIT");
      StdOut.WriteLine("QUIT");
    }

    if (Commands[0] == "READ")
    {
      if (Commands.Length() < 2)
        continue;

      if (Commands[1] == "RELAY")
      {
        if (Commands.Length() > 2)
        {
          SIZE_T i = Parse<SIZE_T>(Commands[2]);
          Trace("Relay %d: %d", i, DevCtrl.GetRelayState(i));
        }
        else
        {
          for (SIZE_T i=0; i<8; i++)
            Trace("Relay %d: %d", i, DevCtrl.GetRelayState(i));
        }
      }
      else if (Commands[1] == "DIGITAL")
      {
        if (Commands.Length() > 2)
        {
          SIZE_T i = Parse<SIZE_T>(Commands[2]);
          Trace("Relay %d: %d", i, DevCtrl.GetDigital(i));
        }
        else
        {
          for (SIZE_T i=0; i<8; i++)
            Trace("Relay %d: %d", i, DevCtrl.GetDigital(i));
        }
      }
      else if (Commands[1] == "ANALOG")
      {
        if (Commands.Length() > 2)
        {
          SIZE_T i = Parse<SIZE_T>(Commands[2]);
          Trace("Relay %d: %d", i, DevCtrl.GetAnalog(i));
        }
        else
        {
          for (SIZE_T i=0; i<8; i++)
            Trace("Relay %d: %d", i, DevCtrl.GetAnalog(i));
        }
      }

    }

    if (Commands[0] == "SET")
    {
      if (Commands.Length() < 2)
        continue;

      if (Commands[1] == "RELAY")
      {
        if (Commands.Length() > 2)
        {
          SIZE_T i = Parse<SIZE_T>(Commands[2]);
          Trace("Setting Relay %d", i);
          DevCtrl.SetRelayState(i, TRUE);
        }
        else
        {
          Trace("Setting all Relays");
          DevCtrl.SetRelayState(TRUE);
        }
      }

    }

    if (Commands[0] == "CLEAR")
    {

      if (Commands.Length() < 2)
        continue;

      if (Commands[1] == "RELAY")
      {
        if (Commands.Length() > 2)
        {
          SIZE_T i = Parse<SIZE_T>(Commands[2]);
          Trace("Clearing Relay %d", i);
          DevCtrl.SetRelayState(i, FALSE);
        }
        else
        {
          Trace("Clearing All Relays");
          DevCtrl.SetRelayState(FALSE);
        }
      }

    }

  }

  return 0;
}


