/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Time/Timer.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1168 $
 * Last Updated: $Date: 2007-09-29 11:47:34 -0400 (Sat, 29 Sep 2007) $
 * 
 * 
 * Copyright (c) 2004-2007, Matthew G Bolitho
 * All rights reserved.
 *
 *
 * Microsoft Reference License (Ms-RL)
 *   
 * This license governs use of the accompanying software. If you use the software, you accept 
 * this license. If you do not accept the license, do not use the software.
 *
 * 1. Definitions
 *  - The terms "reproduce," "reproduction" and "distribution" have the same meaning here as under 
 *    U.S. copyright law.
 *  - "You" means the licensee of the software.
 *  - "Your company" means the company you worked for when you downloaded the software.
 *  - "Reference use" means use of the software within your company as a reference, in read only 
 *    form, for the sole purposes of debugging your products, maintaining your products, or 
 *    enhancing the interoperability of your products with the software, and specifically excludes 
 *    the right to distribute the software outside of your company.
 *  - "Licensed patents" means any Licensor patent claims which read directly on the software as 
 *    distributed by the Licensor under this license.
 * 
 * 2. Grant of Rights
 *  (A) Copyright Grant- Subject to the terms of this license, the Licensor grants you a non-transferable, 
 *      non-exclusive, worldwide, royalty-free copyright license to reproduce the software for reference use.
 *  (B) Patent Grant- Subject to the terms of this license, the Licensor grants you a non-transferable,
 *      non-exclusive, worldwide, royalty-free patent license under licensed patents for reference use.
 * 
 * 3. Limitations
 *  (A) No Trademark License - This license does not grant you any rights to use the Licensor's name
 *      logo, or trademarks.
 *  (B) If you begin patent litigation against the Licensor over patents that you think may apply 
 *      to the software (including a cross-claim or counterclaim in a lawsuit), your license to the 
 *      software ends automatically.
 *  (C) The software is licensed "as-is." You bear the risk of using it. The Licensor gives no express 
 *      warranties, guarantees or conditions. You may have additional consumer rights under your local 
 *      laws which this license cannot change. To the extent permitted under your local laws, the 
 *      Licensor excludes the implied warranties of merchantability, fitness for a particular purpose 
 *      and non-infringement.
 *
 ***************************************************************************************************/





#include "CodeLibrary.hpp"

#include "Time/Timer.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
//------------------------------------------------------------------------------------------------------------------------------------------------------
Timer::Timer()
{
  Reset();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID Timer::Reset()
{
  m_StartTicks = 0;
  m_StopTicks = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_LINUX)
VOID Timer::Start()
{
  struct timeval T;
  gettimeofday( &T, 0 );

  m_StartTicks = (ULONGLONG)T.tv_sec;
  m_StartTicks += 111208150;
  m_StartTicks *= 1000000;
  m_StartTicks += (ULONGLONG)T.tv_usec;
  m_StartTicks *= 100;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::Stop()
{
  struct timeval T;
  gettimeofday( &T, 0 );

  m_StopTicks = (ULONGLONG)T.tv_sec;
  m_StopTicks += 111208150;
  m_StopTicks *= 1000000;
  m_StopTicks += (ULONGLONG)T.tv_usec;
  m_StopTicks *= 100;

  return m_StopTicks-m_StartTicks;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::ElapsedTicks()
{
  if (m_StartTicks == 0)
    return 0;

  if (m_StopTicks == 0)
  {
    ULONGLONG CurrentTicks;
    struct timeval T;
    gettimeofday( &T, 0 );

    CurrentTicks = (ULONGLONG)T.tv_sec;
    CurrentTicks += 111208150;
    CurrentTicks *= 1000000;
    CurrentTicks += (ULONGLONG)T.tv_usec;
    CurrentTicks *= 100;

    return CurrentTicks-m_StartTicks;
  }
  else
    return m_StopTicks-m_StartTicks;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::ElapsedNanoSeconds()
{
  ULONGLONG Ticks = ElapsedTicks();
  return Ticks;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT Timer::ElapsedSeconds()
{
  ULONGLONG Ticks = ElapsedTicks();
  return (FLOAT)Ticks / 100000000.0f;
}
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
VOID Timer::Start()
{
  ::QueryPerformanceCounter( (LARGE_INTEGER*)&m_StartTicks );
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::Stop()
{
  ::QueryPerformanceCounter( (LARGE_INTEGER*)&m_StopTicks );
  return m_StopTicks-m_StartTicks;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::ElapsedTicks()
{
  if (m_StartTicks == 0)
    return 0;

  if (m_StopTicks == 0)
  {
    LONGLONG CurrentTicks;
    ::QueryPerformanceCounter( (LARGE_INTEGER*)&CurrentTicks );
    return CurrentTicks-m_StartTicks;
  }
  else
    return m_StopTicks-m_StartTicks;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Timer::ElapsedNanoSeconds()
{
  ULONGLONG TicksPerSecond;
  ::QueryPerformanceFrequency( (LARGE_INTEGER*)&TicksPerSecond );

  ULONGLONG Ticks = ElapsedTicks();

  return (Ticks * 1000000000) / TicksPerSecond;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FLOAT Timer::ElapsedSeconds()
{
  ULONGLONG TicksPerSecond;
  ::QueryPerformanceFrequency( (LARGE_INTEGER*)&TicksPerSecond );

  ULONGLONG Ticks = ElapsedTicks();

  return (FLOAT)Ticks / TicksPerSecond;
}
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------------------------
DebugTimer::DebugTimer( CONST TCHAR* Description )
{
  m_Timer.Start();
  m_pDescription = CharTraits<TCHAR>::Duplicate(Description);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
DebugTimer::~DebugTimer()
{
  ULONGLONG ns = m_Timer.ElapsedNanoSeconds();
  if (ns > 1000000000L)
    Trace( TEXT("%s elapsed time: %.2fs"), m_pDescription, ns/1000000000.0f );
  else if (ns > 1000000L)
    Trace( TEXT("%s elapsed time: %.2fms"), m_pDescription, ns/1000000.0f );
  else if (ns > 1000L)
    Trace( TEXT("%s elapsed time: %.2fus"), m_pDescription, ns/1000.0f );
  else
    Trace( TEXT("%s elapsed time: %.2fns"), m_pDescription, ns/1.0f );

  free(m_pDescription);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

