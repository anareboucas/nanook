/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/Dialog.cpp $
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

#include "UI/Dialog.hpp"

using namespace Bolitho;
using namespace Bolitho::UI;
using namespace Bolitho::System;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,Dialog,Window);
//----------------------------------------------------------------------------------------------------------------------------------------------------
Dialog::Dialog( LPCTSTR Name )
{
  m_hWnd = 0;
  m_ResourceName = CharTraits<TCHAR>::Duplicate(Name);
  m_pModule = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Dialog::Dialog( WORD Id )
{
  m_hWnd = 0;
  m_ResourceName = MAKEINTRESOURCE(Id);
  m_pModule = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Dialog::Dialog( LPCTSTR Name, Module* pModule )
{
  m_hWnd = 0;
  m_ResourceName = CharTraits<TCHAR>::Duplicate(Name);
  m_pModule = pModule;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Dialog::Dialog( WORD Id, Module* pModule )
{
  m_hWnd = 0;
  m_ResourceName = MAKEINTRESOURCE(Id);
  m_pModule = pModule;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Dialog::~Dialog()
{
  if (!IS_INTRESOURCE(m_ResourceName))
    free(m_ResourceName);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
INT_PTR Dialog::DialogProc( HWND hDialog, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  Dialog* pDlg = 0;

  if(Msg == WM_INITDIALOG)
  {
    pDlg = reinterpret_cast<Dialog*>(lParam);
    SetWindowLongPtr( hDialog, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pDlg));
    pDlg->m_hWnd = hDialog;
  }
  else
    pDlg = reinterpret_cast<Dialog*>(GetWindowLongPtr(hDialog, GWLP_USERDATA));

  if (pDlg != 0)
    return pDlg->OnDialogMessage(Msg, wParam, lParam);

  return FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Dialog::OnDialogMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
{
  switch (Msg)
  {
  case WM_INITDIALOG:
    {
      for (SIZE_T i=0; i<m_ControlBindings.Length(); i++)
        m_ControlBindings[i].Second->Attach( GetDlgItem( m_hWnd, m_ControlBindings[i].First ) );
      
      OnInitDialog();
      return TRUE;
    }
  case WM_CLOSE:
    EndDialog(0); return TRUE;
  case WM_COMMAND:
    {
      if (m_CommandMap.Contains(wParam))
      {
        m_CommandMap[wParam].Invoke();
        return TRUE;
      }
      return FALSE;
    }
  default:
    return FALSE;
  };
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
INT_PTR Dialog::ShowModal( CONST Ptr<Window> pParent )
{
  return DialogBoxParam( m_pModule ? m_pModule->Handle() : GetModuleHandle(0), m_ResourceName, pParent ? pParent->Handle() : 0, &DialogProc, (LPARAM)this );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Dialog::Show( CONST Ptr<Window> pParent )
{
  m_hWnd = CreateDialogParam( m_pModule ? m_pModule->Handle() : GetModuleHandle(0), m_ResourceName, pParent ? pParent->Handle() : 0, &DialogProc, (LPARAM)this );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Dialog::BindNotify( WORD CommandId, WORD Notification, FuncPtr<VOID>& S )
{
	m_CommandMap.Add(MAKELONG(CommandId,Notification),S);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Dialog::BindControl( WORD ControlId, Control* pControl )
{
  m_ControlBindings.Add( Pair<WORD,Control*>( ControlId, pControl ) );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Dialog::EndDialog( INT_PTR Result )
{
  ::EndDialog( m_hWnd, Result );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Dialog::OnInitDialog()
{
  return;
}

