/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/ProgressDialog.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1241 $
 * Last Updated: $Date: 2007-11-12 13:55:16 -0500 (Mon, 12 Nov 2007) $
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

#include "Debug/Trace.hpp"

#if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_VISTA
#include "ProgressDialog.hpp"

using namespace Bolitho;
using namespace Bolitho::UI;
using namespace Bolitho::System;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,ProgressDialog,TaskDialog);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ProgressDialog::ProgressDialog(LPCTSTR Title, Ptr<Icon> pIcon, Ptr<Window> pParent ) : m_ThreadWait(FALSE)
{
  if (pIcon)
    SetIcon(pIcon);
  SetWidth(200);
  if (Title)
    SetTitle(Title);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ProgressDialog::~ProgressDialog()
{
  ClickButton(0);
  m_pThread->Wait();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT ProgressDialog::OnCreate()
{
  SetProgressBarMarquee(TRUE,0);
  m_ThreadWait.Set();
  return S_OK;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ProgressDialog::BeginTask( LPCTSTR TaskName, BOOL CanCancel )
{
  if (m_hWnd == 0)
  {
    m_pThread = Thread::Create(this);
    m_ThreadWait.Wait();
  }
  else
  {
    ShowWindow( m_hWnd, SW_SHOW);
  }

  SetBodyTitle(TaskName);
  m_TaskName = TaskName;
  m_TaskTimer.Reset();
  m_TaskTimer.Start();

  Trace(TEXT("%s..."), m_TaskName.ConstStr() );
  Trace.Indent();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ProgressDialog::EndTask()
{
  Trace.UnIndent();
  Trace(TEXT("Finished %s. Total Time: %s"), m_TaskName.ConstStr(), String::FormatTimeInterval( m_TaskTimer.ElapsedSeconds() ).ConstStr() );

  ShowWindow( m_hWnd, SW_HIDE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ProgressDialog::BeginStep(LPCTSTR StepName, LONGLONG MinValue, LONGLONG MaxValue)
{
  m_StepName = StepName;
  m_StepTimer.Reset();
  m_StepTimer.Start();

  SetBody(StepName);
  if (MaxValue > MinValue)
  {
    m_MinimumValue = MinValue;
    m_MaximumValue = MaxValue;

    SetMarqueeProgressBar(FALSE);
    SetProgressBarRange(0, 1000);
    SetProgressBarValue(ProgressValue(MinValue));
  }
  else
  {
    SetMarqueeProgressBar(TRUE);
    SetProgressBarMarquee(TRUE, 0);
  }

  Trace(TEXT("%s..."), m_StepName.ConstStr() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL ProgressDialog::UpdateStep( LONGLONG Value )
{
  FLOAT PercentComplete = (FLOAT)Value / m_MaximumValue;
	DWORD ElapsedTime = (DWORD)m_StepTimer.ElapsedSeconds();
	DWORD RemainingTime = (DWORD)((ElapsedTime / PercentComplete) - ElapsedTime);
  
  
  String Body = m_StepName;
  if (ElapsedTime > 8)
    Body = m_StepName + "\nElapsed Time: " + String::FormatTimeInterval( ElapsedTime ) + "    Estimated Time Remaining: " + String::FormatTimeInterval( RemainingTime );
  else
    Body = m_StepName + "\nElapsed Time: " + String::FormatTimeInterval( ElapsedTime );

  SetBody(Body);
  SetProgressBarValue(ProgressValue(Value));

  return FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
USHORT ProgressDialog::ProgressValue(LONGLONG x)
{
  x -= m_MinimumValue;
  x *= 1000;
  x /= (m_MaximumValue - m_MinimumValue);
  return (USHORT)x;
} 
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ProgressDialog::EndStep()
{
  m_StepTimer.Stop();
  Trace(TEXT("Finished %s. Total Time: %s"), m_StepName.ConstStr(), String::FormatTimeInterval( m_StepTimer.ElapsedSeconds() ).ConstStr() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT ProgressDialog::ThreadMain()
{
  SetStyle( TDF_SHOW_MARQUEE_PROGRESS_BAR );
  AddButton( IDCANCEL, "Hide", TRUE );
  ShowModal();

  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#endif
