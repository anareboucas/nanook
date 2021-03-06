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

#include "System/Threading/Thread.hpp"
using Bolitho::System::Thread;

#include "IO/StdIO.hpp"
using Bolitho::StdIn;
using Bolitho::StdOut;

class MotorEncoderTestApplication : public ConsoleApplication
{
protected:
  String m_LeftDeviceName;
  DWORD m_LeftBaudRate;
  String m_RightDeviceName;
  DWORD m_RightBaudRate;

public:
  virtual INT Main();
  virtual BOOL OnInit();

};

ImplementApplicationEntryPoint(MotorEncoderTestApplication);

#include "MotorEncoder.hpp"

#include "Math/Math.hpp"
using Bolitho::Math::Abs;

BOOL MotorEncoderTestApplication::OnInit()
{
  Trace0Enter("");

  if (!ConsoleApplication::OnInit())
    return FALSE;

  m_CmdLine.DefineKeyedValue((LPCTSTR) "LeftDeviceName", (LPCTSTR) "LeftDeviceName", (LPCTSTR) "The device name for the left encoder", TRUE);
  m_CmdLine.DefineKeyedValue((LPCTSTR) "LeftBaudRate", (LPCTSTR) "LeftBaudRate", (LPCTSTR) "The baud rate to connect to the left encoder with", FALSE, (LPCTSTR) "9600");
  m_CmdLine.DefineKeyedValue((LPCTSTR) "RightDeviceName", (LPCTSTR) "EightDeviceName", (LPCTSTR) "The device name for the right encoder", TRUE);
  m_CmdLine.DefineKeyedValue((LPCTSTR) "RightBaudRate", (LPCTSTR) "RightBaudRate", (LPCTSTR) "The baud rate to connect to the right encoder with", FALSE, (LPCTSTR) "9600");

  if (!ParseCommandLine())
    return FALSE;

  m_LeftDeviceName = m_CmdLine.GetValue<String>((LPCTSTR) "LeftDeviceName");
  m_LeftBaudRate = m_CmdLine.GetValue<DWORD>((LPCTSTR) "LeftBaudRate");
  m_RightDeviceName = m_CmdLine.GetValue<String>((LPCTSTR) "RightDeviceName");
  m_RightBaudRate = m_CmdLine.GetValue<DWORD>((LPCTSTR) "RightBaudRate");

  return TRUE;
}

INT MotorEncoderTestApplication::Main()
{
  Trace0Enter("");

  MotorEncoder Left;
  MotorEncoder Right;

  Left.Initialize(m_LeftDeviceName, m_LeftBaudRate);
  Right.Initialize(m_RightDeviceName, m_RightBaudRate);

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
      StdOut.WriteLine("VALUE [LEFT|RIGHT]");
      StdOut.WriteLine("DELTA [LEFT|RIGHT]");
      StdOut.WriteLine("READ [LEFT|RIGHT]");
      StdOut.WriteLine("REALTIME");
    }

    if (Commands[0] == "VALUE")
    {
      if (Commands.Length() == 1)
      {
        Trace((LPCTSTR) "%u %u", Left.ReadEncoderValue(), Right.ReadEncoderValue() );
      }
      else
      {
        if (Commands[1] == "LEFT")
          Trace((LPCTSTR) "%u", Left.ReadEncoderValue() );
        if (Commands[2] == "RIGHT")
          Trace((LPCTSTR) "%u", Right.ReadEncoderValue() );
      }
    }

    if (Commands[0] == "DELTA")
    {
      if (Commands.Length() == 1)
      {
        INT LeftDelta = Left.ReadEncoderDelta();
        INT RightDelta = Right.ReadEncoderDelta();
        Trace((LPCTSTR) "%d %d", LeftDelta, RightDelta, LeftDelta-RightDelta );
      }
      else
      {
        if (Commands[1] == "LEFT")
          Trace((LPCTSTR) "%d", Left.ReadEncoderDelta() );
        if (Commands[2] == "RIGHT")
          Trace((LPCTSTR) "%d", Right.ReadEncoderDelta() );
      }
    }

    if (Commands[0] == "READ")
    {
      INT LeftDelta = Left.ReadEncoderDelta();
      INT RightDelta = Right.ReadEncoderDelta();
      
      Trace((LPCTSTR) "LEFT %u (%d)   RIGHT %u (%d)    BIAS %d", Left.ReadEncoderValue(), LeftDelta, Right.ReadEncoderValue(), RightDelta, LeftDelta-RightDelta );
    }

    if (Commands[0] == "REALTIME")
    {
      while (TRUE)
      {
        INT LeftDelta = Left.ReadEncoderDelta();
        INT RightDelta = Right.ReadEncoderDelta();
      
        Trace((LPCTSTR) "LEFT %u (%d)    RIGHT %u (%d)    BIAS %d   ABSBIAS %d", Left.ReadEncoderValue(), LeftDelta, Right.ReadEncoderValue(), RightDelta, LeftDelta-RightDelta, Abs(LeftDelta)-Abs(RightDelta) );

        Thread::Sleep(100);
      }
    }

  }

  return 0;
}


