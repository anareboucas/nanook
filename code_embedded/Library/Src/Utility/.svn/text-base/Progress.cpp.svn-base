/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Utility/Progress.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1179 $
 * Last Updated: $Date: 2007-10-08 20:36:28 -0400 (Mon, 08 Oct 2007) $
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
#include "Utility/Progress.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho,Progress,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::BeginTask( LPCTSTR TaskName, BOOL CanCancel )
{
  m_TaskName = TaskName;
  m_TaskTimer.Reset();
	m_TaskTimer.Start();

  Trace(TEXT("%s..."), m_TaskName.ConstStr() );
  Trace.Indent();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::EndTask()
{
  Trace.UnIndent();
  Trace(TEXT("Finished %s. Total Time: %s"), m_TaskName.ConstStr(), String::FormatTimeInterval( m_TaskTimer.ElapsedSeconds() ).ConstStr() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::BeginStep( LPCTSTR StepName, LONGLONG MinimumValue, LONGLONG MaximumValue )
{
  m_MinimumValue = MinimumValue;
  m_MaximumValue = MaximumValue;
  m_StepName = StepName;

  m_StepTimer.Reset();
	m_StepTimer.Start();

  Trace(TEXT("%s..."), m_StepName.ConstStr() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Progress::UpdateStep( LONGLONG Value )
{
  m_CurrentValue = Value;

	CONST INT ProgressBarLength = 30;
	FLOAT PercentComplete = (FLOAT)m_CurrentValue / m_MaximumValue;
	DWORD ElapsedTime = (DWORD)m_StepTimer.ElapsedSeconds();
	DWORD RemainingTime = (DWORD)((ElapsedTime / PercentComplete) - ElapsedTime);

	INT DoneBars = (INT)(PercentComplete * ProgressBarLength);
	INT TodoBars = ProgressBarLength - DoneBars;

  if (!StdOut.IsRedirected())
  {
    if (ElapsedTime > 10)
      StdOut << " [" << String('#', DoneBars) << String(' ', TodoBars) << "]  Elapsed: " << String::FormatTimeInterval( ElapsedTime ) << "  Remaining: " << String::FormatTimeInterval( RemainingTime ) << " \r";
    else
      StdOut << " [" << String('#', DoneBars) << String(' ', TodoBars) << "]  Elapsed: " << String::FormatTimeInterval( ElapsedTime ) << " \r";
  }
  else
  {
    if (m_CurrentValue > m_NextMilestone)
    {
      Trace(TEXT("%d%% complete"), (INT)(PercentComplete*100.0f));
      m_NextMilestone += (m_MaximumValue / 20);
    }
  }

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Progress::EndStep()
{
  if (!StdOut.IsRedirected())
    StdOut << String(' ', 79) << "\r";

  Trace(TEXT("Finished %s. Total Time: %s"), m_StepName.ConstStr(), String::FormatTimeInterval( m_StepTimer.ElapsedSeconds() ).ConstStr() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
