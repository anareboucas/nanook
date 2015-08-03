/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/Window.cpp $
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

#include "CodeLibrary.hpp"

#include "System/Application.hpp"

#include "UI/Window.hpp"
#include "UI/GDI/DeviceContext.hpp"

using namespace Bolitho::UI;
using namespace Bolitho;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,Window,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<WindowClass> Window::m_pDefaultWindowClass;
//----------------------------------------------------------------------------------------------------------------------------------------------------
Window::Window()
{
  if (m_pDefaultWindowClass == 0)
    m_pDefaultWindowClass = WindowClass::Create( TEXT("Window"),  CS_VREDRAW  | CS_HREDRAW );
  else
    m_pDefaultWindowClass.AddRef();

  m_hWnd = 0;
  m_pPrevWndProc = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Window::~Window()
{
  Destroy();

  m_hWnd = 0;
  m_pDefaultWindowClass.Release();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
HWND Window::Handle() CONST
{
  return m_hWnd;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Attach( HWND hWnd )
{
  m_hWnd = hWnd;
  #pragma warning(suppress:4244)
  SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Detach()
{
  #pragma warning(suppress:4312)
  SetWindowLongPtr( m_hWnd, GWLP_USERDATA, 0 );
  m_hWnd = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Subclass( HWND hWnd )
{
  Attach( hWnd );
  #pragma warning(suppress:4312)
  m_pPrevWndProc = (WNDPROC)GetWindowLongPtr( m_hWnd, GWLP_WNDPROC );
  #pragma warning(suppress:4244)
  SetWindowLongPtr( m_hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::UnSubclass()
{
  #pragma warning(suppress:4244)
  SetWindowLongPtr( m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_pPrevWndProc );
  Detach();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Window* Window::FromHandle( HWND hWnd )
{
  #pragma warning(suppress:4312)
  return reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::WindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  try
  {
		Window* pWnd = 0;

    if(Msg == WM_NCCREATE)
    {
      pWnd = reinterpret_cast<Window*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
      #pragma warning(suppress:4244)
      SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
      pWnd->m_hWnd = hWnd;
    }
    else
      pWnd = Window::FromHandle(hWnd);

    if (pWnd != 0)
      return pWnd->OnMessage(Msg, wParam, lParam);
  }
  catch (Exception& e)
  {
    g_pLibrary->GetApplication()->OnException(e);    
  }
  return DefWindowProc( hWnd,Msg, wParam, lParam);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
{
  switch (Msg)
  {
  case WM_CREATE:
    return OnCreate( (CREATESTRUCT*)lParam );
  case WM_DESTROY:
    return OnDestroy();

  case WM_ENTERSIZEMOVE:
    return OnBeginSizeOrMove();
  case WM_EXITSIZEMOVE:
    return OnEndSizeOrMove();

  case WM_SIZE:
    return OnSize( (UINT)wParam, Size( LOWORD(lParam), HIWORD(lParam) ) ); 
  case WM_SIZING:
    return OnSizing( (UINT)wParam, (Rect*)lParam );
  case WM_MOVE:
    return OnMove( Point( LOWORD(lParam), HIWORD(lParam) ) ); 
  case WM_MOVING:
    return OnMoving( (Rect*)lParam );
  case WM_SHOWWINDOW:
    return OnShowWindow( (BOOL)wParam, (UINT)lParam );

  case WM_ACTIVATE:
    return OnActivate( LOWORD(wParam), (HWND)lParam, HIWORD(wParam) );

  case WM_ERASEBKGND:
    {
    DeviceContext DC = DeviceContext( (HDC)lParam );
    return OnEraseBackground( &DC );
    }

  case WM_NOTIFY:
    return OnNotify( (INT)wParam, (LPNMHDR)lParam );

  case WM_TIMER:
    return OnTimer( wParam, (TIMERPROC)lParam );

  case WM_SETFOCUS:
    return OnSetFocus( (HWND)wParam );
  case WM_KILLFOCUS:
    return OnKillFocus( (HWND)wParam );

  case WM_LBUTTONDBLCLK:
    return OnLButtonDblClk( (UINT)wParam, Point(lParam) );
  case WM_LBUTTONDOWN:
    return OnLButtonDown( (UINT)wParam, Point(lParam) );
  case WM_LBUTTONUP:
    return OnLButtonUp( (UINT)wParam, Point(lParam) );
  case WM_RBUTTONDBLCLK:
    return OnRButtonDblClk( (UINT)wParam, Point(lParam) );
  case WM_RBUTTONDOWN:
    return OnRButtonDown( (UINT)wParam, Point(lParam) );
  case WM_RBUTTONUP:
    return OnRButtonUp( (UINT)wParam, Point(lParam) );
  case WM_MBUTTONDBLCLK:
    return OnMButtonDblClk( (UINT)wParam, Point(lParam) );
  case WM_MBUTTONDOWN:
    return OnMButtonDown( (UINT)wParam, Point(lParam) );
  case WM_MBUTTONUP:
    return OnMButtonUp( (UINT)wParam, Point(lParam) );
  case WM_MOUSEMOVE:
    return OnMouseMove( (UINT)wParam, Point(lParam) );
  case WM_MOUSEWHEEL:
    return OnMouseWheel( LOWORD(wParam), HIWORD(wParam), Point(lParam) );

  case WM_PAINT:
    {
    PAINTSTRUCT PaintStruct;
    BeginPaint( m_hWnd, &PaintStruct );
    DeviceContext DC = DeviceContext(PaintStruct.hdc);
    LRESULT Result = OnPaint( &DC, Rect(PaintStruct.rcPaint) );
    EndPaint( m_hWnd, &PaintStruct );
    return Result;
    }
    
  case WM_VSCROLL:
    return OnVScroll( LOWORD(wParam), HIWORD(wParam), (HWND)lParam );
  case WM_HSCROLL:
    return OnHScroll( LOWORD(wParam), HIWORD(wParam), (HWND)lParam );

	case WM_KEYDOWN:
		return OnKeyDown( (UINT)wParam, LOWORD(lParam), HIWORD(lParam) );
	case WM_KEYUP:
		return OnKeyUp( (UINT)wParam, LOWORD(lParam), HIWORD(lParam) );


  default:
    return BaseWindowProc( Msg, wParam, lParam);
  };
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::SendMessage( UINT Msg, WPARAM wParam, LPARAM lParam ) CONST
{
  return ::SendMessage( m_hWnd, Msg, wParam, lParam );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::SetTimer( UINT_PTR EventId, UINT MilliSeconds )
{
  ::SetTimer( m_hWnd, EventId, MilliSeconds, NULL );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnTimer( UINT_PTR EventId, TIMERPROC Func )
{
  return BaseWindowProc( WM_TIMER, EventId, (LPARAM)Func );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnBeginSizeOrMove()
{
  return BaseWindowProc( WM_ENTERSIZEMOVE, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnEndSizeOrMove()
{
  return BaseWindowProc( WM_EXITSIZEMOVE, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnEraseBackground( DeviceContext* pDC )
{
  return BaseWindowProc( WM_ERASEBKGND, 0, (LPARAM)pDC->Handle() );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnVScroll( UINT Mode, UINT Position, HWND ScrollCtrl )
{
  return BaseWindowProc( WM_VSCROLL, MAKEWORD(Mode, Position), (LPARAM)ScrollCtrl );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnHScroll( UINT Mode, UINT Position, HWND ScrollCtrl )
{
  return BaseWindowProc( WM_HSCROLL, MAKEWORD(Mode, Position), (LPARAM)ScrollCtrl );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnKeyDown( UINT Char, UINT RepCount, UINT Flags )
{
	return BaseWindowProc( WM_KEYDOWN, Char, MAKEWORD(RepCount,Flags) );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnKeyUp( UINT Char, UINT RepCount, UINT Flags )
{
	return BaseWindowProc( WM_KEYUP, Char, MAKEWORD(RepCount,Flags) );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnPaint( DeviceContext* pDC, Rect UpdateRect )
{
  return 0; 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnCreate( CREATESTRUCT* pCreateStruct )
{
  return BaseWindowProc( WM_CREATE, 0, (LPARAM)pCreateStruct );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnDestroy()
{
  return BaseWindowProc( WM_DESTROY, 0, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnActivate( UINT Status, HWND hWnd, BOOL Minimized )
{
  return BaseWindowProc( WM_ACTIVATE, MAKELONG( Status, Minimized ), (LPARAM)hWnd );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Show()
{
  ShowWindow( m_hWnd, SW_SHOW );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Hide()
{
  ShowWindow( m_hWnd, SW_HIDE );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Close()
{
  CloseWindow( m_hWnd );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Destroy()
{
  if (m_hWnd)
    ::DestroyWindow( m_hWnd );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::SetCapture()
{
  ::SetCapture( m_hWnd );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::ReleaseCapture()
{
  ::ReleaseCapture();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::SetText( LPCTSTR Text )
{
  SendMessage( WM_SETTEXT, 0, (LPARAM)Text );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Window::GetText() CONST
{
  String Text;
  SIZE_T Length = SendMessage( WM_GETTEXTLENGTH, 0, 0 );
  Text.Resize( Length+1 );
  SendMessage( WM_GETTEXT, Length+1, (LPARAM)Text.Str() );
  return Text;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::SetPosition( LONG X, LONG Y )
{
  SetWindowPos( m_hWnd, 0, X, Y, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::SetPosition( Point P )
{
  SetWindowPos( m_hWnd, 0, P[0], P[1], 0, 0, SWP_NOZORDER | SWP_NOSIZE );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::SetSize( LONG W, LONG H )
{
  SetWindowPos( (HWND)m_hWnd, 0, 0, 0, W, H, SWP_NOZORDER | SWP_NOMOVE );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::SetSize( Size S )
{
  SetWindowPos( (HWND)m_hWnd, 0, 0, 0, S[0], S[1], SWP_NOZORDER | SWP_NOMOVE );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::SetParent( Ptr<Window> pParentWnd )
{
  if (pParentWnd == 0)
    ::SetParent( (HWND)m_hWnd, 0 );
  else
    ::SetParent( (HWND)m_hWnd, (HWND)pParentWnd->Handle() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Rect Window::GetClientRect()
{
  Rect R;
  ::GetClientRect( m_hWnd, &R );
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Rect Window::GetWindowRect()
{
  Rect R;
  ::GetWindowRect( m_hWnd, &R );
  return R;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Size Window::GetWindowSize()
{
  Rect R;
  ::GetWindowRect( m_hWnd, &R );
  return Size( R.Width(), R.Height() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Size Window::GetClientSize()
{
  Rect R;
  ::GetClientRect( m_hWnd, &R );
  return Size( R.Width(), R.Height() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Invalidate()
{
  Rect R = GetClientRect();
  ::InvalidateRect( m_hWnd, &R, TRUE ); 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Create( Ptr<WindowClass> pWndClass, LPCTSTR Title, DWORD Style, DWORD StyleEx, Ptr<Window> pParent, Menu* pMenu, Point Position, Point Size )
{
  m_hWnd = CreateWindowEx( StyleEx, pWndClass->ClassName(), Title, Style, Position[0], Position[1], Size[0], Size[1], (pParent) ? pParent->Handle() : 0, (pMenu) ? pMenu->Handle() : 0, GetModuleHandle(0), (Window*)this );
  if (m_hWnd == 0)
    Win32Exception::ThrowError(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::CreateCaret( Size S )
{
  if (!::CreateCaret( m_hWnd, NULL, S[0], S[1] ))
    Win32Exception::ThrowError(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::CreateCaret( Bitmap* pBitmap )
{
  if (!::CreateCaret( m_hWnd, pBitmap->Handle(), 0, 0 ))
    Win32Exception::ThrowError(this);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::DestroyCaret()
{
  ::DestroyCaret();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::ShowCaret()
{
  ::ShowCaret( m_hWnd );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::HideCaret()
{
  ::HideCaret( m_hWnd );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Point Window::GetCaretPosition()
{
  Point P;
  ::GetCaretPos( (LPPOINT)&P );
  return P;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::SetCaretPosition( Point P )
{
  ::SetCaretPos( P[0], P[1] );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
HWND Window::SetFocus()
{
  return ::SetFocus( m_hWnd );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Window::SetScrollInfo( int Bar, LPSCROLLINFO pScrollInfo, BOOL Redraw )
{
  return ::SetScrollInfo( m_hWnd, Bar, pScrollInfo, Redraw );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnNotify( INT CtrlId, LPNMHDR pNMHdr )
{
  return BaseWindowProc( WM_NOTIFY, CtrlId, (LPARAM)pNMHdr );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnSize( UINT Type, Size S )
{
  return BaseWindowProc( WM_SIZE, Type, MAKELONG(S[0],S[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnSizing( UINT Side, Rect* WindowRect )
{
  return BaseWindowProc( WM_SIZING, Side, (LPARAM)WindowRect );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnMove( Point Position )
{
  return BaseWindowProc( WM_MOVE, 0, MAKELONG(Position[0],Position[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnMoving( Rect* WindowRect )
{
  return BaseWindowProc( WM_MOVING, 0, (LPARAM)WindowRect );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnShowWindow( BOOL ShowOrHide, UINT Status )
{
  return BaseWindowProc( WM_SHOWWINDOW, ShowOrHide, Status );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnLButtonDown( UINT Flags, Point P )
{
  return BaseWindowProc( WM_LBUTTONDOWN, Flags, MAKELONG(P[0],P[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnLButtonUp( UINT Flags, Point P )
{
  return BaseWindowProc( WM_LBUTTONUP, Flags, MAKELONG(P[0],P[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnLButtonDblClk( UINT Flags, Point P )
{
  return BaseWindowProc( WM_LBUTTONDBLCLK, Flags, MAKELONG(P[0],P[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnRButtonDown( UINT Flags, Point P )
{
  return BaseWindowProc( WM_RBUTTONDOWN, Flags, MAKELONG(P[0],P[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnRButtonUp( UINT Flags, Point P )
{
  return BaseWindowProc( WM_RBUTTONUP, Flags, MAKELONG(P[0],P[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnMouseWheel( UINT Flags, SHORT Delta, Point P )
{
  return BaseWindowProc( WM_MOUSEWHEEL, MAKELONG(Flags, Delta), MAKELONG( P[0], P[1] ) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnRButtonDblClk( UINT Flags, Point P )
{
  return BaseWindowProc( WM_RBUTTONDBLCLK, Flags, MAKELONG(P[0],P[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnMButtonDown( UINT Flags, Point P )
{
  return BaseWindowProc( WM_MBUTTONDOWN, Flags, MAKELONG(P[0],P[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnMButtonUp( UINT Flags, Point P )
{
  return BaseWindowProc( WM_MBUTTONUP, Flags, MAKELONG(P[0],P[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnMButtonDblClk( UINT Flags, Point P )
{
  return BaseWindowProc( WM_MBUTTONDBLCLK, Flags, MAKELONG(P[0],P[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnMouseMove( UINT Flags, Point P )
{
  return BaseWindowProc( WM_MOUSEMOVE, Flags, MAKELONG(P[0],P[1]) );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnSetFocus( HWND hWnd )
{
  return BaseWindowProc( WM_SETFOCUS, (WPARAM)hWnd, 0 );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::OnKillFocus( HWND hWnd)
{
  return BaseWindowProc( WM_KILLFOCUS, (WPARAM)hWnd, 0 );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT Window::BaseWindowProc( UINT Msg, WPARAM wParam, LPARAM lParam )
{
  if (m_pPrevWndProc)
    return CallWindowProc( m_pPrevWndProc, m_hWnd, Msg, wParam, lParam );
  else
    return DefWindowProc( m_hWnd, Msg, wParam, lParam );
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Enable()
{
  SendMessage( WM_ENABLE, TRUE, 0 );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::Disable()
{
  SendMessage( WM_ENABLE, FALSE, 0 );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Window::SetFont( Ptr<Font> pFont )
{
  m_pFont = pFont;
  SendMessage( WM_SETFONT, (LPARAM)(pFont ? pFont->Handle() : 0), 0 );
}

