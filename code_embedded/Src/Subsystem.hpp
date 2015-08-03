/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Subsystem.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 193 $
 * Last Updated: $Date: 2007-08-17 09:41:12 -0400 (Fri, 17 Aug 2007) $
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

#include "System/Threading/Thread.hpp"
using Bolitho::System::Runnable;
using Bolitho::System::Thread;

#include "System/Threading/CriticalSection.hpp"
using Bolitho::System::CriticalSection;

#include "Time/DateTime.hpp"
using Bolitho::DateTime;

namespace NASA
{
  struct SubsystemState
  { enum { RUNNING = 1, PAUSED = 2 };  };

  class Subsystem : public Runnable
  {
    DeclareRuntimeType();
  public:
    Subsystem();

    /* Called to initiaize the subsystem */
    virtual BOOL Initialize();
    /* Called to un-initialize the subsystem */
    virtual VOID Destroy();

    /* Called by an external watchdog thread to determine if the subsystem is operating correctly */
    BOOL IsAlive() CONST;   

    /* Returns the number of times the subsystem has been initialized */
    SIZE_T ResetCount() CONST
    { return m_ResetCount; }

    /* Returns the state of the subsystem */
    DWORD State() CONST
    { return m_State; }
  protected:
    /* The thread used to run the internal processing loop */
    Ptr<Thread> m_pThread;

    /* The lock used to protect state shared across multiple threads */
    mutable CriticalSection m_Lock;

    /* Called by the subsystem to update the watchdog timer */
    VOID ResetWatchdogTimer();

    DWORD m_State;
  private:
    DateTime m_LastAlive;
    SIZE_T m_ResetCount;
  };

}

