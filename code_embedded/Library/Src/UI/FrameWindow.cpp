/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/FrameWindow.cpp $
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

#include "UI/FrameWindow.hpp"

using namespace Bolitho;
using namespace Bolitho::UI;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,FrameWindow,Window);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<WindowClass> FrameWindow::m_pDefaultFrameClass;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
FrameWindow::FrameWindow()
{
  if (m_pDefaultFrameClass == 0)
    m_pDefaultFrameClass = WindowClass::Create( TEXT("FrameWindow"), CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS );
  m_pDefaultFrameClass.AddRef();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL FrameWindow::Create()
{
  Window::Create( m_pDefaultFrameClass, TEXT(""), WS_OVERLAPPEDWINDOW, 0 );
  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
FrameWindow::~FrameWindow()
{
  m_pControls[0] = 0;
  m_pControls[1] = 0;
  m_pControls[2] = 0;
  m_pControls[3] = 0;
  m_pControls[4] = 0;

  m_pDefaultFrameClass.Release();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FrameWindow::SetIcon( Ptr<Icon> I )
{
  m_pIcon = I;
  SendMessage( WM_SETICON, ICON_BIG, (LPARAM)I->Handle() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FrameWindow::SetMainMenu( Ptr<Menu> M )
{
  m_pMenu = M;
  SetMenu( (HWND)m_hWnd, (HMENU)M->Handle() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FrameWindow::Minimize()
{
  ShowWindow( (HWND)m_hWnd, SW_MINIMIZE );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FrameWindow::Maximize()
{
  ShowWindow( (HWND)m_hWnd, SW_SHOWMAXIMIZED );
}   
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FrameWindow::Restore()
{
  ShowWindow( (HWND)m_hWnd, SW_RESTORE );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FrameWindow::SetControl( Ptr<Control> pWindow, DWORD Location )
{
  m_pControls[Location] = pWindow;
  pWindow->SetParent(this);
  pWindow->Show();
  if (GetFocus() == m_hWnd && Location == ControlLocation::CENTER)
    pWindow->SetFocus();
  RecalcLayout();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FrameWindow::CreateControl( Ptr<Control> pWindow, DWORD Location )
{
  m_pControls[Location] = pWindow;
  pWindow->Create(this);
  pWindow->Show();
  if (GetFocus() == m_hWnd && Location == ControlLocation::CENTER)
    pWindow->SetFocus();
  RecalcLayout();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT FrameWindow::OnMessage( UINT Message, WPARAM wParam, LPARAM lParam )
{
  switch( Message )
  {
	case WM_COMMAND:
		{
		UINT Id = LOWORD(wParam);
		if (m_CommandMap.Contains(Id))
			m_CommandMap[Id].Invoke();
    return Window::OnMessage(Message, wParam, lParam);
		}
	default:
    return Window::OnMessage(Message, wParam, lParam);
  };
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT FrameWindow::OnMove( Point Pos )
{
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT FrameWindow::OnSize( UINT Type, Size S )
{
  RecalcLayout();
  return Window::OnSize( Type, S );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT FrameWindow::OnBeginSizeOrMove()
{
	for (SIZE_T i=0; i<5; i++)
		if (m_pControls[i] != 0)
			m_pControls[i]->SendMessage(WM_ENTERSIZEMOVE, 0, 0);
  return Window::OnBeginSizeOrMove();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT FrameWindow::OnEndSizeOrMove()
{
	for (SIZE_T i=0; i<5; i++)
		if (m_pControls[i] != 0)
			m_pControls[i]->SendMessage(WM_EXITSIZEMOVE, 0, 0);
			
  return Window::OnEndSizeOrMove();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT FrameWindow::OnClose()
{
  Destroy();
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT FrameWindow::OnDestroy()
{
  PostQuitMessage(0);
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT FrameWindow::OnEraseBackground( DeviceContext* DC )
{
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT FrameWindow::OnSetFocus( HWND hWndLost )
{
  if (m_pControls[ControlLocation::CENTER])
    m_pControls[ControlLocation::CENTER]->SetFocus();
  return 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FrameWindow::BindCommand( UINT CommandId, FuncPtr<VOID>& S )
{
	m_CommandMap.Add(CommandId,S);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FrameWindow::RecalcLayout()
{
  Rect WindowRect = GetClientRect();
  Rect ClientRect = GetClientRect();

  if (m_pControls[ControlLocation::TOP])
  {
    Rect ControlRect = m_pControls[ControlLocation::TOP]->GetWindowRect();
    WindowRect.top += ControlRect.Height();
    m_pControls[ControlLocation::TOP]->SetSize( WindowRect.Width(), ControlRect.Height() );
    m_pControls[ControlLocation::TOP]->SetPosition( 0, 0 );
    m_pControls[ControlLocation::TOP]->RecalcLayout();
  }

  if (m_pControls[ControlLocation::BOTTOM])
  {
    Rect ControlRect = m_pControls[ControlLocation::BOTTOM]->GetWindowRect();
    WindowRect.bottom -= ControlRect.Height();
    m_pControls[ControlLocation::BOTTOM]->SetSize( WindowRect.Width(), ControlRect.Height() );
    m_pControls[ControlLocation::BOTTOM]->SetPosition( 0, ClientRect.Height()-ControlRect.Height() );
    m_pControls[ControlLocation::BOTTOM]->RecalcLayout();
  }

  if (m_pControls[ControlLocation::CENTER])
  {
    m_pControls[ControlLocation::CENTER]->SetSize(WindowRect.Width(),WindowRect.Height());
    m_pControls[ControlLocation::CENTER]->SetPosition(WindowRect.left, WindowRect.top);
    m_pControls[ControlLocation::CENTER]->RecalcLayout();
  }

}

