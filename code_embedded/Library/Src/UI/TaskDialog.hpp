/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/TaskDialog.hpp $
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




#pragma once

#if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_VISTA

#include "UI/Window.hpp"

namespace Bolitho
{
  namespace UI
  {
    class LIBRARY_API TaskDialog : public virtual Object
    {
      DeclareRuntimeType();
    public:
			TaskDialog();
			virtual ~TaskDialog();

      VOID SetStyle( DWORD Style );
      VOID ClearStyle( DWORD Style );
      DWORD GetStyle() CONST;

      VOID SetTitle( LPCTSTR Title );
      
      VOID SetBodyTitle( LPCTSTR BodyTitle );
      VOID SetBody( LPCTSTR BodyText );
      VOID SetExpandedBody( LPCTSTR ExpandedBody, LPCTSTR ExpandedControlText = 0, LPCTSTR CollapsedControlText = 0 );
      
      VOID SetVerification( LPCTSTR Title );

      VOID SetFooter( LPCTSTR Footer, WORD Icon );
      VOID SetFooter( LPCTSTR Footer, LPCTSTR Icon );
      VOID SetFooter( LPCTSTR Footer, Ptr<Icon> pIcon );

      VOID SetIcon( WORD Icon );
      VOID SetIcon( LPCTSTR Icon );
      VOID SetIcon( Ptr<Icon> pIcon );

      VOID AddButton( UINT Id, LPCTSTR Text, BOOL Default = FALSE );
      VOID AddRadioButton( UINT Id, LPCTSTR Text, BOOL Default = FALSE );

      VOID SetProgressBarRange( UINT Min, UINT Max );
      VOID SetProgressBarValue( UINT Value );
      VOID SetProgressBarMarquee(BOOL Marquee, DWORD Speed = 0 );
      VOID SetMarqueeProgressBar(BOOL Marquee);

      VOID SetWidth( UINT Width );

      VOID ClickButton( UINT Button );

      INT ShowModal( CONST Ptr<Window> pParent = Ptr<Window>::Null );
    protected:
      TASKDIALOGCONFIG m_TDC;
      HWND m_hWnd;

      virtual HRESULT OnCreate();
      virtual HRESULT OnDestroy();
      
      static HRESULT CALLBACK TaskDialogCallbackProc( HWND hWnd, UINT Notification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData );

      INT m_SelectedButton;
      INT m_SelectedRadioButton;
      BOOL m_CheckboxValue;

    };
  }
}

#endif
