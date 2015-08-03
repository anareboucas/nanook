/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Subsystem.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 290 $
 * Last Updated: $Date: 2008-01-19 12:38:14 -0500 (Sat, 19 Jan 2008) $
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
#include "Subsystem.hpp"

//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,Subsystem,Runnable);
//----------------------------------------------------------------------------------------------------------------------------
Subsystem::Subsystem()
{
  Trace0Enter("");

  m_ResetCount = 0;
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL Subsystem::Initialize()
{
  Trace0Enter("");

  m_ResetCount++;
  ResetWatchdogTimer();
  m_State |= SubsystemState::RUNNING;

  m_pThread = Thread::Create(this);

  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------
VOID Subsystem::Destroy()
{
  Trace0Enter("");

  m_State = 0;

  if (m_pThread)
  {
    TraceInfo(this, "Waiting for subsystem to terminate itself");
    if (!m_pThread->Wait(2000))
    {
      TraceWarning(this, "The subsystem manager timed-out whilst waiting for the subsystem to terminate.  Subsystem will be forcably terminated");
      m_pThread->Terminate();
    }

    m_pThread = 0;
  }
}
//----------------------------------------------------------------------------------------------------------------------------
BOOL Subsystem::IsAlive() CONST
{
  return (DateTime::Now() - m_LastAlive).TotalSeconds() < 10.0f;
}
//----------------------------------------------------------------------------------------------------------------------------
VOID Subsystem::ResetWatchdogTimer()
{
  m_LastAlive = DateTime::Now();
}

