/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/System/Threading/ThreadPool.cpp $
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
#include "System/Threading/ThreadPool.hpp"
#include "System/Threading/ScopedLock.hpp"



using namespace Bolitho;
using namespace Bolitho::System;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,ThreadPool,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
ThreadPool::ThreadPool()
{
	m_MinThreadCount = 0;
	m_MaxThreadCount = 0;

	m_CurrentThreadCount = 0;
	m_WorkingThreadCount = 0;

	m_CurrentJobCount = 0;
	m_IdleThreadCount = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ThreadPool::~ThreadPool()
{
	Wait();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Initialize( LONG MinimumThreadCount, LONG MaximumThreadCount )
{
	m_MinThreadCount = MinimumThreadCount;
	m_MaxThreadCount = MaximumThreadCount;

	for (LONG i=0; i<MinimumThreadCount; i++)
		CreateWorkerThread();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Stop()
{
	
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Start()
{

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Pause()
{

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Resume()
{

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD WINAPI ThreadPool::WorkerThreadProc( LPVOID Param )
{
	WorkItem* I;
	ThreadPool* pPool = (ThreadPool*)Param;

	while (TRUE)
	{
		Verify( pPool->m_QueueChange.Lock() );

		while (pPool->m_CurrentJobCount == 0)
			pPool->m_QueueChange.Wait();

		I = pPool->m_Queue.Dequeue();

 		Verify( pPool->m_StateChange.Lock() );

		pPool->m_IdleThreadCount--;
		pPool->m_WorkingThreadCount++;
		pPool->m_CurrentJobCount--;

		pPool->m_StateChange.Unlock();
		pPool->m_QueueChange.Unlock();

		if (I->m_pProc != 0)
			(*I->m_pProc)(I->m_pParam);

		Verify(pPool->m_StateChange.Lock());

		pPool->m_IdleThreadCount++;
		pPool->m_WorkingThreadCount--;
		pPool->m_StateChange.Unlock();

		pPool->m_StateChange.Broadcast();
	}

	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::CreateWorkerThread()
{
  m_StateChange.Lock();

  m_IdleThreadCount++;
  m_CurrentThreadCount++;

  Ptr<Thread> pThread = Thread::Create( WorkerThreadProc, this );
  m_Threads.Add(pThread);

  m_StateChange.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ThreadPool::Wait( DWORD Timeout )
{
	m_StateChange.Lock();
	while (m_WorkingThreadCount > 0 || m_CurrentJobCount > 0)
	{
		BOOL Result = m_StateChange.Wait( Timeout );
		if (!Result)
		{
			m_StateChange.Unlock();
			return FALSE;
		}
	}

	m_StateChange.Unlock();
	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Enqueue( LPTHREAD_START_ROUTINE Proc, LPVOID Param )
{
	Verify(m_QueueChange.Lock());
	Verify(m_StateChange.Lock());

	m_Queue.Enqueue(new WorkItem(Proc,Param));
	
	m_CurrentJobCount++;

	m_StateChange.Broadcast();
	m_QueueChange.Signal();

	m_StateChange.Unlock();
	m_QueueChange.Unlock();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ThreadPool::Enqueue( Runnable* pRunnable )
{
	Enqueue( &Thread::RunnableThreadProc, pRunnable );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ThreadPool::WorkItem::WorkItem( LPTHREAD_START_ROUTINE pProc, LPVOID pParam )
{
	m_pProc = pProc;
	m_pParam = pParam;
	m_Running = FALSE;
	m_Complete = FALSE;
	m_ReturnValue = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ThreadPool::WorkItem::Wait( DWORD Timeout )
{
	m_StateChange.Lock();
	while (!m_Complete)
	{
		BOOL Result = m_StateChange.Wait(Timeout);
		if (!Result)
		{
			m_StateChange.Unlock();
			return FALSE;
		}
	}
	m_StateChange.Unlock();
	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
