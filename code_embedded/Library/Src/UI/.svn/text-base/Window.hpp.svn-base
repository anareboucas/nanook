/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/Window.hpp $
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

#include "Utility/NonCopyable.hpp"
#include "Utility/SizePointRect.hpp"
#include "UI/GDI/Font.hpp"
#include "UI/GDI/Bitmap.hpp"
#include "UI/GDI/DeviceContext.hpp"
#include "UI/GDI/Icon.hpp"
#include "UI/GDI/Menu.hpp"
#include "UI/WindowClass.hpp"

namespace Bolitho
{
  namespace UI
  {
    class LIBRARY_API Window : public Object, public NonCopyable
    {
      DeclareRuntimeType();
    public:
      Window();
      virtual ~Window();

      HWND Handle() CONST;
      VOID Attach( HWND hWnd ); 
      VOID Detach();

      static Window* FromHandle( HWND hWnd );

      VOID Subclass( HWND hWnd );
      VOID UnSubclass();

			static LRESULT WINAPI WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

      VOID SetText( LPCTSTR Text );
      String GetText() CONST;

      VOID SetFont( Ptr<Font> pFont );
      
      VOID SetPosition( LONG X, LONG Y );
      VOID SetPosition( Point P );
      VOID SetSize( LONG W, LONG H );
      VOID SetSize( Size S );
      VOID SetParent( Ptr<Window> pParentWnd );

      Rect GetClientRect();
      Rect GetWindowRect();
      Size GetWindowSize();
      Size GetClientSize();

      VOID Invalidate();

      VOID Show();
      VOID Hide();

      VOID Enable();
      VOID Disable();

      VOID Close();

      VOID Destroy();

      HWND SetFocus();

      VOID CreateCaret( Size S );
      VOID CreateCaret( Bitmap* pBitmap );
      VOID DestroyCaret();
      VOID ShowCaret();
      VOID HideCaret();
      Point GetCaretPosition();
      VOID SetCaretPosition( Point P );

      VOID SetCapture();
      VOID ReleaseCapture();

      VOID SetTimer( UINT_PTR EventId, UINT MilliSeconds );

      LRESULT SendMessage( UINT Msg, WPARAM wParam, LPARAM lParam ) CONST;
      BOOL SetScrollInfo( int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE );

			VOID Create( Ptr<WindowClass> pWndClass, LPCTSTR Title, DWORD Style, DWORD StyleEx, Ptr<Window> pParent = Ptr<Window>::Null, Menu* pMenu = 0, Point Position = Point(CW_USEDEFAULT,CW_USEDEFAULT), Point Size = Point(CW_USEDEFAULT,CW_USEDEFAULT) );
      static Ptr<WindowClass> m_pDefaultWindowClass;
    protected:

      virtual LRESULT OnMessage( UINT Message, WPARAM wParam, LPARAM lParam );

      virtual LRESULT OnNotify( INT CtrlId, LPNMHDR pNMHdr );

      virtual LRESULT OnTimer( UINT_PTR EventId, TIMERPROC Func );

      virtual LRESULT OnBeginSizeOrMove();
      virtual LRESULT OnEndSizeOrMove();
      virtual LRESULT OnSize( UINT Type, Size S );
      virtual LRESULT OnSizing( UINT Side, Rect* WindowRect );
      virtual LRESULT OnMove( Point Position );
      virtual LRESULT OnMoving( Rect* WindowRect );
      virtual LRESULT OnShowWindow( BOOL ShowOrHide, UINT Status );

      virtual LRESULT OnEraseBackground( DeviceContext* pDeviceContext );
      virtual LRESULT OnPaint( DeviceContext* pDC, Rect UpdateRect );

      virtual LRESULT OnCreate( CREATESTRUCT* pCreateStruct );
      virtual LRESULT OnDestroy();

      virtual LRESULT OnActivate( UINT Status, HWND hWnd, BOOL Minimized );

      virtual LRESULT OnLButtonDblClk( UINT Flags, Point P );
      virtual LRESULT OnLButtonDown( UINT Flags, Point P );
      virtual LRESULT OnLButtonUp( UINT Flags, Point P );

      virtual LRESULT OnRButtonDblClk( UINT Flags, Point P );
      virtual LRESULT OnRButtonDown( UINT Flags, Point P );
      virtual LRESULT OnRButtonUp( UINT Flags, Point P );

      virtual LRESULT OnMButtonDblClk( UINT Flags, Point P );
      virtual LRESULT OnMButtonDown( UINT Flags, Point P );
      virtual LRESULT OnMButtonUp( UINT Flags, Point P );

      virtual LRESULT OnMouseMove( UINT Flags, Point P );
      virtual LRESULT OnMouseWheel( UINT Flags, SHORT Delta, Point P );

      virtual LRESULT OnSetFocus( HWND hWnd );
      virtual LRESULT OnKillFocus( HWND hWnd );

      virtual LRESULT OnVScroll( UINT Mode, UINT Position, HWND ScrollCtrl );
      virtual LRESULT OnHScroll( UINT Mode, UINT Position, HWND ScrollCtrl );

			virtual LRESULT OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
			virtual LRESULT OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );




      HWND m_hWnd;
      WNDPROC m_pPrevWndProc;
      Ptr<Font> m_pFont;

      LRESULT BaseWindowProc( UINT MSG, WPARAM wParam, LPARAM lParam );

    };

  }
}
