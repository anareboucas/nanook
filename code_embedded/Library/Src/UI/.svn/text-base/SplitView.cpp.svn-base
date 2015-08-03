/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/SplitView.cpp $
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

#include "UI/SplitView.hpp"

using namespace Bolitho;
using namespace Bolitho::UI;


//-----------------------------------------------------------------------------------------------------------------------------------------------------
SplitView::SplitView()
{
  m_pView[0] = 0;
  m_pView[1] = 0;

  m_SplitterPosition = 100;
  m_LayoutMode = LM_MOVEABLE | LM_VERTICAL;
  m_Captured = false;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SplitView::~SplitView()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SplitView::Create(Ptr<Window> pParent)
{
  Window::Create( m_pDefaultWindowClass, TEXT(""), WS_CHILD, 0, pParent );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SplitView::SetChild( Pane Index, Ptr<Window> pView )
{
  Assert (m_pView[Index] == 0);

  m_pView[Index] = pView;
  pView->SetParent(this);
  pView->Show();
  RecalcLayout();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT SplitView::OnShow( BOOL ShowOrHide )
{
  if (ShowOrHide)
  {
    if (m_pView[0])
      m_pView[0]->Show();
    if (m_pView[1])
      m_pView[1]->Show();
  }

  return 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SplitView::SetLayout( LayoutMode M )
{
  m_LayoutMode = M;
  RecalcLayout();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT SplitView::OnSize( UINT Type, Size S )
{
  m_Width = S[0];
  m_Height = S[1];
  RecalcLayout();
  return 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SplitView::RecalcLayout()
{
  if (m_pView[0])
  {
    if (m_LayoutMode & LM_HORIZONTAL)
      m_pView[0]->SetSize( m_Width, m_SplitterPosition );
    if (m_LayoutMode & LM_VERTICAL)
      m_pView[0]->SetSize( m_SplitterPosition, m_Height );
  }
  if (m_pView[1])
  {
    if (m_LayoutMode & LM_HORIZONTAL)
    {
      m_pView[1]->SetSize( m_Width, m_Height - m_SplitterPosition - SplitterWidth );
      m_pView[1]->SetPosition( 0, m_SplitterPosition + SplitterWidth );
    }
    if (m_LayoutMode & LM_VERTICAL)
    {
      m_pView[1]->SetSize( m_Width - m_SplitterPosition - SplitterWidth, m_Height );
      m_pView[1]->SetPosition( m_SplitterPosition + SplitterWidth, 0 );
    }
  }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT SplitView::OnMessage( UINT Message, WPARAM wParam, LPARAM lParam )
{
  switch (Message)
	{
  case WM_PAINT:
    {
      PAINTSTRUCT Paint;
      BeginPaint( m_hWnd, &Paint );
      HBRUSH B = (HBRUSH)(COLOR_BTNFACE+1);
      HBRUSH Old = (HBRUSH)SelectObject( Paint.hdc, B );
      RECT R;
      if (m_LayoutMode & LM_HORIZONTAL)
      {
        R.left = 0;
        R.top = m_SplitterPosition;
        R.bottom = m_SplitterPosition+SplitterWidth;
        R.right = m_Width;
      }
      if (m_LayoutMode & LM_VERTICAL)
      {
        R.left = m_SplitterPosition;
        R.top = 0;
        R.bottom = m_Height;
        R.right = m_SplitterPosition+SplitterWidth;
      }

      FillRect( Paint.hdc, &R, B );
      SelectObject( Paint.hdc, Old );
      DeleteObject(B);
      EndPaint( m_hWnd, &Paint );
      return 0;
    }
  case WM_LBUTTONDOWN:
    {
      if (m_LayoutMode & LM_FIXED)
        break;
      m_Captured = true;
      GetCursorPos( (LPPOINT)&m_MousePosition);
      SetCapture();
      m_StartSplitterPosition = m_SplitterPosition;
      break; 
    }
  case WM_LBUTTONUP:
    {
      if (m_LayoutMode & LM_FIXED)
        break;
      m_Captured = false;
      ReleaseCapture();
      break;
    }
  case WM_MOUSEMOVE:
    {
      if (m_LayoutMode & LM_FIXED)
        break;
      
      if (m_LayoutMode & LM_HORIZONTAL)
        SetCursor( LoadCursor( NULL, IDC_SIZENS ) );
      if (m_LayoutMode & LM_VERTICAL)
        SetCursor( LoadCursor( NULL, IDC_SIZEWE ) );
      
      if (wParam & MK_LBUTTON && m_Captured)
      {
        Point MousePosition;
        GetCursorPos( (LPPOINT)&MousePosition);
        if (m_LayoutMode & LM_HORIZONTAL)
        {
          m_SplitterPosition = m_StartSplitterPosition + (MousePosition[1] - m_MousePosition[1]);
          if (m_SplitterPosition > m_Height - SplitterWidth)
            m_SplitterPosition = m_Height - SplitterWidth;
        }
        if (m_LayoutMode & LM_VERTICAL)
        {
          m_SplitterPosition = m_StartSplitterPosition + (MousePosition[0] - m_MousePosition[0]);
          if (m_SplitterPosition > m_Width- SplitterWidth)
            m_SplitterPosition = m_Width - SplitterWidth;
        }
        if (m_SplitterPosition < 0)
          m_SplitterPosition = 0;

        RecalcLayout();
      }
      break;
    }
  case WM_SETCURSOR:
    return 0;
  case WM_ERASEBKGND:
    return 0;
  default:
		return Window::OnMessage(Message, wParam, lParam);
	};

  return 0;
}

