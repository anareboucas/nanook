/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/ConsoleView.cpp $
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

#include "UI/ConsoleView.hpp"

using namespace Bolitho;
using namespace Bolitho::UI;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,ConsoleView,View);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ConsoleView::ConsoleView()
{
  for (SIZE_T i=0; i<100; i++)
    m_TextLines.Add( String::Format( TEXT("%d %d %d %f %f %f"), i, 2*i, 3*i, i/1.0, i/2.0, i/3.0 ) );

  m_ViewPosition = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID ConsoleView::Create( Ptr<Window> pParent )
{
  Window::Create( m_pDefaultWindowClass, TEXT(""), WS_CHILD | WS_VSCROLL, WS_EX_CLIENTEDGE, pParent );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnCreate( CREATESTRUCT* pCreateStruct )
{
  DeviceContext DC(this);
  TEXTMETRIC Tm;

  m_pBkgrnd = Brush::CreateSolidBrush( 0x00ffffff );
  m_pFont = new Font( DC.PointsToPixels(12), TEXT("Consolas") );
  
  DC.SelectFont( m_pFont );
  DC.GetTextMetrics( &Tm );
  DC.SelectFont(0);
  
  m_FontSize[0] = Tm.tmAveCharWidth;
  m_FontSize[1] = Tm.tmHeight;

  SCROLLINFO Si;
  Si.cbSize = sizeof(SCROLLINFO);
  Si.nMin = 0;
  Si.nMax = (int)m_TextLines.Length() - 1;
  Si.nPos = 0;
  Si.fMask = SIF_RANGE | SIF_POS;

  SetScrollInfo( SB_VERT, &Si );

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnSize( Size S )
{
  SCROLLINFO Si;
  Si.cbSize = sizeof(SCROLLINFO);
  Si.nPage = S[0] / m_FontSize[1];
  Si.fMask = SIF_PAGE;

  SetScrollInfo( SB_VERT, &Si );

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnVScroll( UINT Mode, UINT Position, HWND ScrollCtrl )
{
  if (Mode == SB_THUMBTRACK)
    m_ViewPosition = Position;

  if (Mode == SB_LINEUP)
    m_ViewPosition--;
  if (Mode == SB_LINEDOWN)
    m_ViewPosition++;

  Rect R = GetClientRect();
  LONG PageSize = R.Height() / m_FontSize[1];
  if (Mode == SB_PAGEUP)
    m_ViewPosition -= PageSize;
  if (Mode == SB_PAGEDOWN)
    m_ViewPosition += PageSize;

  if (m_ViewPosition > (int)m_TextLines.Length() - PageSize)
    m_ViewPosition = (int)m_TextLines.Length() - PageSize;

  SCROLLINFO Si;
  Si.cbSize = sizeof(SCROLLINFO);
  Si.nPos = m_ViewPosition;
  Si.fMask = SIF_POS;

  SetScrollInfo( SB_VERT, &Si );
  Invalidate();

  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnDestroy()
{
  delete m_pFont;
  delete m_pBkgrnd;

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnPaint( DeviceContext* pDC, Rect UpdateRect )
{
  Rect ClientRect = GetClientRect();

  DeviceContext* pBitmapDC = new DeviceContext(pDC);
  Bitmap* pBitmap = Bitmap::CreateCompatible(pDC, ClientRect.Width(), ClientRect.Height() );

  pBitmapDC->SelectBitmap( pBitmap );
  pBitmapDC->SelectFont( m_pFont );
  pBitmapDC->FillRect( ClientRect, m_pBkgrnd );
  
  for (SIZE_T i=m_ViewPosition; i<m_TextLines.Length(); i++)
    pBitmapDC->TextOut( 0, (INT)(i-m_ViewPosition)*m_FontSize[1], m_TextLines[i] );


  pDC->BitBlt(0, 0, ClientRect.Width(), ClientRect.Height(), pBitmapDC, 0, 0, SRCCOPY ); 

  pBitmapDC->SelectFont(0);
  pBitmapDC->SelectBitmap(0);

  delete pBitmap;
  delete pBitmapDC;

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnActivate( UINT Show )
{
  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnSetFocus( HWND hWndLost )
{
  CreateCaret( m_FontSize );
  SetCaretPosition( m_CaretPosition );
  ShowCaret();

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnKillFocus()
{
  DestroyCaret();

  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnMouseDown( Point P, UINT Buttons )
{
  HideCaret();

  m_SelectionBegin[0] = P[0] / m_FontSize[0];
  m_SelectionBegin[1] = P[1] / m_FontSize[0] + m_ViewPosition; 
  m_SelectionEnd[0] = P[0] / m_FontSize[0];
  m_SelectionEnd[1] = P[1] / m_FontSize[0] + m_ViewPosition;
  
  Invalidate();
  
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnMouseUp( Point P, UINT Buttons )
{
  ShowCaret();

  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ConsoleView::OnMouseMove( Point P, UINT Buttons )
{
  if (Buttons & MK_LBUTTON)
  {
    m_SelectionEnd[0] = P[0] / m_FontSize[0];
    m_SelectionEnd[0] = P[1] / m_FontSize[1] + m_ViewPosition;

    Invalidate();
  }

  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
