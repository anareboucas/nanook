/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/GDI/DeviceContext.cpp $
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

#include "UI/GDI/DeviceContext.hpp"
#include "UI/Window.hpp"

using namespace Bolitho::UI;
using namespace Bolitho;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,DeviceContext,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
DeviceContext::DeviceContext( HDC hDC )
{
  m_hDC = hDC;

  m_RestoreFont = 0;
  m_RestoreBitmap = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DeviceContext::DeviceContext( Ptr<Window> pWindow )
{
  m_hDC = GetDC( pWindow->Handle() );

  m_RestoreFont = 0;
  m_RestoreBitmap = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DeviceContext::DeviceContext( DeviceContext* pDC )
{
  m_hDC = ::CreateCompatibleDC( pDC->Handle() );

  m_RestoreFont = 0;
  m_RestoreBitmap = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
DeviceContext::~DeviceContext()
{
  DeleteDC( m_hDC );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceContext::FillRect( Rect R, Brush* pBrush )
{
  ::FillRect( m_hDC, &R, pBrush->Handle() );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceContext::TextOut( int x, int y, LPCTSTR Text )
{
	::TextOut( m_hDC, x, y, Text, (int)CharTraits<TCHAR>::Length(Text) );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceContext::TextOut( int x, int y, CONST String& Text )
{
  ::TextOut( m_hDC, x, y, Text, (int)Text.Length() );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceContext::SelectFont( Font* pFont )
{
  if (pFont == 0)
    SelectObject( m_hDC, m_RestoreFont );
  else if (m_RestoreFont == 0)
    m_RestoreFont = (HFONT)SelectObject( m_hDC, pFont->Handle() );
  else
    SelectObject( m_hDC, pFont->Handle() );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceContext::SelectBitmap( Bitmap* pBitmap )
{
  if (pBitmap == 0)
    SelectObject( m_hDC, m_RestoreBitmap );
  else if (m_RestoreBitmap == 0)
    m_RestoreBitmap = (HBITMAP)SelectObject( m_hDC, pBitmap->Handle() );
  else
    SelectObject( m_hDC, pBitmap->Handle() );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceContext::GetTextMetrics( LPTEXTMETRIC Metric )
{
  ::GetTextMetrics( m_hDC, Metric );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceContext::BitBlt( int DestX, int DestY, int Width, int Height, DeviceContext* pSrc, int SourceX, int SourceY, DWORD Operation )
{
  ::BitBlt( m_hDC, DestX, DestY, Width, Height, pSrc->Handle(), SourceX, SourceY, Operation );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceContext::SetTextColor( COLORREF Color )
{
  ::SetTextColor( m_hDC, Color );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DeviceContext::SetBkgrndColor( COLORREF Color )
{
  ::SetBkColor( m_hDC, Color );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
int DeviceContext::PointsToPixels( int PointSize )
{
  return -MulDiv(PointSize, GetDeviceCaps(m_hDC, LOGPIXELSY), 72);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

