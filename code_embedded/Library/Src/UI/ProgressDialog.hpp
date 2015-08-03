/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/ProgressDialog.hpp $
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




#pragma once

#if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_VISTA

#include "UI/TaskDialog.hpp"
#include "Utility/Progress.hpp"
#include "System/Threading/Thread.hpp"
#include "System/Threading/Event.hpp"
#include "Time/Timer.hpp"

namespace Bolitho
{
  namespace UI
  {
    class LIBRARY_API ProgressDialog : public TaskDialog, public IProgress, protected System::Runnable
    {
      DeclareRuntimeType();
    public:
      ProgressDialog(LPCTSTR Title = 0, Ptr<Icon> pIcon = Ptr<Icon>::Null, Ptr<Window> pParent = Ptr<Window>::Null);
      virtual ~ProgressDialog();

      virtual VOID BeginTask( LPCTSTR TaskName, BOOL CanCancel = FALSE );
      virtual VOID EndTask();

      virtual VOID BeginStep( LPCTSTR StepName, LONGLONG MinValue = 0, LONGLONG MaxValue = 0 );
      virtual BOOL UpdateStep( LONGLONG Value );
      virtual VOID EndStep();

    protected:
      virtual INT ThreadMain();
      virtual HRESULT OnCreate();

      Ptr<System::Thread> m_pThread;
      System::Event m_ThreadWait;

      LONGLONG m_MinimumValue;
      LONGLONG m_MaximumValue;

      String m_TaskName;
      String m_StepName;

      Timer m_StepTimer;
      Timer m_TaskTimer;

      USHORT ProgressValue(LONGLONG x);

    private:
      ProgressDialog(CONST ProgressDialog&) : m_ThreadWait(FALSE)
      {}

    };
  }
}

#endif
