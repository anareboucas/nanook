/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/TaskDialog.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1181 $
 * Last Updated: $Date: 2007-10-09 08:19:03 -0400 (Tue, 09 Oct 2007) $
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

#if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_VISTA

#include "TaskDialog.hpp"
#include "Utility/Flags.hpp"

using namespace Bolitho;
using namespace Bolitho::UI;

#if defined(ARCH_I386)
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined(ARCH_AMD64)
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,TaskDialog,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
TaskDialog::TaskDialog()
{
  m_hWnd = 0;

  ZeroMemory( &m_TDC, sizeof(m_TDC) );
  m_TDC.cbSize = sizeof(m_TDC);

  m_TDC.pfCallback = &TaskDialog::TaskDialogCallbackProc;
  m_TDC.lpCallbackData = (LONG_PTR)this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
TaskDialog::~TaskDialog()
{
  
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT TaskDialog::ShowModal( CONST Ptr<Window> pParent )
{
  if (pParent != 0)
    m_TDC.hwndParent = pParent->Handle();

  HRESULT Result = TaskDialogIndirect( &m_TDC, &m_SelectedButton, &m_SelectedRadioButton, &m_CheckboxValue ); 
  return m_SelectedButton;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetStyle( DWORD Style )
{
  m_TDC.dwFlags |= Style;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD TaskDialog::GetStyle() CONST
{
  return m_TDC.dwFlags;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::ClearStyle( DWORD Style )
{
  m_TDC.dwFlags &= ~Style;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::AddButton( UINT Id, LPCTSTR Text, BOOL Default )
{
  TASKDIALOG_BUTTON* pButtons = new TASKDIALOG_BUTTON[m_TDC.cButtons+1];
  CopyMemory( pButtons, m_TDC.pButtons, m_TDC.cButtons*sizeof(TASKDIALOG_BUTTON) );
  pButtons[m_TDC.cButtons].nButtonID = Id;
  pButtons[m_TDC.cButtons].pszButtonText = CharTraits<WCHAR>::Duplicate( StringW(Text) );

  if (m_TDC.pButtons)
    delete[] m_TDC.pButtons;
  m_TDC.pButtons = pButtons;

  if (Default)
    m_TDC.nDefaultButton = m_TDC.cButtons;
  m_TDC.cButtons++;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::AddRadioButton( UINT Id, LPCTSTR Text, BOOL Default )
{
  TASKDIALOG_BUTTON* pRadioButtons = new TASKDIALOG_BUTTON[m_TDC.cRadioButtons+1];
  CopyMemory( pRadioButtons, m_TDC.pRadioButtons, m_TDC.cRadioButtons*sizeof(TASKDIALOG_BUTTON) );
  pRadioButtons[m_TDC.cRadioButtons].nButtonID = Id;
  pRadioButtons[m_TDC.cRadioButtons].pszButtonText = CharTraits<WCHAR>::Duplicate(StringW(Text));
  
  if (m_TDC.pRadioButtons)
    delete[] m_TDC.pRadioButtons;
  m_TDC.pRadioButtons = pRadioButtons;

  if (Default)
    m_TDC.nDefaultRadioButton = m_TDC.cRadioButtons;
  m_TDC.cRadioButtons++;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetVerification( LPCTSTR Text )
{
  m_TDC.pszVerificationText = CharTraits<WCHAR>::Duplicate(StringW(Text));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetTitle( LPCTSTR Title )
{
  LPWSTR WTitle = CharTraits<WCHAR>::Duplicate(StringW(Title));
  if (m_hWnd == 0)
    m_TDC.pszWindowTitle = WTitle;
  else
  {
    SendMessageW( m_hWnd, WM_SETTEXT, 0, (LPARAM)WTitle );
    FreeMemory(WTitle);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetBodyTitle( LPCTSTR BodyTitle )
{
  LPWSTR WBodyTitle = CharTraits<WCHAR>::Duplicate(StringW(BodyTitle));
  if (m_hWnd == 0)
    m_TDC.pszMainInstruction = CharTraits<WCHAR>::Duplicate( StringW(BodyTitle) );
  else
  {
    SendMessage(m_hWnd, TDM_SET_ELEMENT_TEXT, TDE_MAIN_INSTRUCTION, (LPARAM)WBodyTitle);
    FreeMemory(WBodyTitle);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetBody( LPCTSTR BodyText )
{
  LPWSTR WBodyText = CharTraits<WCHAR>::Duplicate(StringW(BodyText));
  if (m_hWnd == 0)
    m_TDC.pszContent = WBodyText;
  else
  {
    SendMessage(m_hWnd, TDM_SET_ELEMENT_TEXT, TDE_CONTENT, (LPARAM)WBodyText);
    FreeMemory(WBodyText);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetExpandedBody( LPCTSTR ExpandedBody, LPCTSTR ExpandedControlText, LPCTSTR CollapsedControlText )
{
  LPWSTR WExpandedBody = CharTraits<WCHAR>::Duplicate(StringW(ExpandedBody));
  if (m_hWnd == 0)
  {
    m_TDC.pszExpandedInformation = CharTraits<WCHAR>::Duplicate( StringW(ExpandedBody) );
    if (ExpandedControlText)
      m_TDC.pszExpandedControlText  = CharTraits<WCHAR>::Duplicate( StringW(ExpandedControlText) );
    if (CollapsedControlText)
      m_TDC.pszCollapsedControlText = CharTraits<WCHAR>::Duplicate( StringW(CollapsedControlText) );
  }
  else
  {
    SendMessage(m_hWnd, TDM_SET_ELEMENT_TEXT, TDE_EXPANDED_INFORMATION, (LPARAM)WExpandedBody);
    FreeMemory(WExpandedBody);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetFooter( LPCTSTR Footer, WORD Icon )
{
  LPWSTR WFooter = CharTraits<WCHAR>::Duplicate(StringW(Footer));
  
  if (m_hWnd == 0)
  {
    m_TDC.pszFooter = WFooter;
    m_TDC.pszFooterIcon = MAKEINTRESOURCEW(Icon);
    m_TDC.dwFlags &= ~TDF_USE_HICON_FOOTER;
  }
  else
  {
    SendMessage(m_hWnd, TDM_SET_ELEMENT_TEXT, TDE_FOOTER, (LPARAM)WFooter);
    FreeMemory(WFooter);

    Assert( !IsFlagSet(m_TDC.dwFlags,TDF_USE_HICON_FOOTER) );
    SendMessage(m_hWnd, TDM_UPDATE_ICON, TDIE_ICON_FOOTER, (LPARAM)MAKEINTRESOURCE(Icon) );
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetFooter( LPCTSTR Footer, LPCTSTR Icon )
{
  LPWSTR WFooter = CharTraits<WCHAR>::Duplicate(StringW(Footer));
  LPWSTR WIcon = CharTraits<WCHAR>::Duplicate(StringW(Icon));
  
  if (m_hWnd == 0)
  {
    m_TDC.pszFooter = WFooter;
    m_TDC.pszFooterIcon = WIcon;
    m_TDC.dwFlags &= ~TDF_USE_HICON_FOOTER;
  }
  else
  {
    SendMessage(m_hWnd, TDM_SET_ELEMENT_TEXT, TDE_FOOTER, (LPARAM)WFooter);
    FreeMemory(WFooter);

    Assert( !IsFlagSet(m_TDC.dwFlags,TDF_USE_HICON_FOOTER) );
    SendMessage(m_hWnd, TDM_UPDATE_ICON, TDIE_ICON_FOOTER, (LPARAM)WIcon );
    FreeMemory(WIcon);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetFooter( LPCTSTR Footer, Ptr<Icon> pIcon )
{
  LPWSTR WFooter = CharTraits<WCHAR>::Duplicate(StringW(Footer));
  if (m_hWnd == 0)
  {
    m_TDC.pszFooter = WFooter;
    m_TDC.hFooterIcon = pIcon->Handle();
    m_TDC.dwFlags |= TDF_USE_HICON_FOOTER;

  }
  else
  {
    SendMessage(m_hWnd, TDM_SET_ELEMENT_TEXT, TDE_FOOTER, (LPARAM)WFooter);
    FreeMemory(WFooter);

    Assert( IsFlagSet(m_TDC.dwFlags,TDF_USE_HICON_FOOTER) );
    SendMessage(m_hWnd, TDM_UPDATE_ICON, TDIE_ICON_FOOTER, (LPARAM)pIcon->Handle());
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetIcon( WORD Icon )
{
  SetIcon(MAKEINTRESOURCE(Icon));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetIcon( LPCTSTR Icon )
{
  Assert(m_hWnd == 0);
  m_TDC.pszMainIcon = CharTraits<WCHAR>::Duplicate( StringW(Icon) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetIcon( Ptr<Icon> pIcon )
{
  Assert(m_hWnd == 0);
  m_TDC.hMainIcon = pIcon->Handle();
  m_TDC.dwFlags |= TDF_USE_HICON_MAIN;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetProgressBarRange( UINT Min, UINT Max )
{
  Assert(m_hWnd != 0);
  SendMessage(m_hWnd, TDM_SET_PROGRESS_BAR_RANGE, 0, MAKELPARAM(Min,Max));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetProgressBarValue( UINT Value )
{
  Assert(m_hWnd != 0);
  SendMessage(m_hWnd, TDM_SET_PROGRESS_BAR_POS, Value, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetProgressBarMarquee( BOOL Marquee, DWORD Speed )
{
  Assert(m_hWnd != 0);
  SendMessage(m_hWnd, TDM_SET_PROGRESS_BAR_MARQUEE, Marquee, Speed);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetMarqueeProgressBar(BOOL Marquee)
{
  Assert(m_hWnd != 0);
  SendMessage(m_hWnd, TDM_SET_MARQUEE_PROGRESS_BAR, Marquee, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::SetWidth( UINT Width )
{
  Assert(m_hWnd == 0);
  m_TDC.cxWidth = Width;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID TaskDialog::ClickButton( UINT Button )
{
  Assert(m_hWnd != 0);
  SendMessage( m_hWnd, TDM_CLICK_BUTTON, Button, 0 );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT TaskDialog::TaskDialogCallbackProc( HWND hWnd, UINT Notification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData )
{
  TaskDialog* pThis = (TaskDialog*)dwRefData;
  Assert(pThis != 0);

  pThis->m_hWnd = hWnd;

  switch(Notification)
  {
  case TDN_CREATED:
    return pThis->OnCreate();
  case TDN_BUTTON_CLICKED:
    break;
  case TDN_HYPERLINK_CLICKED:
    break;
  case TDN_TIMER:
    break;
  case TDN_DESTROYED:
    return pThis->OnDestroy();
  };
  return S_OK;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT TaskDialog::OnCreate()
{
  return S_OK;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT TaskDialog::OnDestroy()
{
  return S_OK;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


#endif