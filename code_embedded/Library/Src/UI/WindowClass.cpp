/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/WindowClass.cpp $
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

#include "UI/Window.hpp"
#include "UI/WindowClass.hpp"

using namespace Bolitho;
using namespace Bolitho::UI;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,WindowClass,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<WindowClass> WindowClass::Attach( LPCTSTR ClassName )
{
  WindowClass* pClass = new WindowClass();
  pClass->m_ClassName = ClassName;
  return pClass;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<WindowClass> WindowClass::Create( LPCTSTR ClassName, UINT Style, Icon* pSmallIcon, Icon* pLargeIcon, Cursor* pCursor, Brush* pBrush )
{
  HINSTANCE hInstance = GetModuleHandle(NULL);

  WNDCLASSEX WndClass;

	WndClass.cbSize         = sizeof(WNDCLASSEX);

	WndClass.style			    = Style;
  WndClass.lpfnWndProc	  = (WNDPROC)&Window::WindowProc;
	WndClass.cbClsExtra		  = 0;
	WndClass.cbWndExtra		  = 0;
	WndClass.hInstance		  = hInstance;
  WndClass.hIcon			    = (pLargeIcon != 0) ? pLargeIcon->Handle() : 0;
	WndClass.hCursor		    = (pCursor != 0) ? pCursor->Handle() : 0;
	WndClass.hbrBackground  = (pBrush != 0) ? pBrush->Handle() : 0;
	WndClass.lpszMenuName	  = 0;
  WndClass.lpszClassName	= ClassName;
	WndClass.hIconSm		    = (pSmallIcon != 0) ? pSmallIcon->Handle() : 0;

	RegisterClassEx(&WndClass);

  WindowClass* pClass = new WindowClass();

  pClass->m_ClassName = ClassName;
  pClass->m_pSmallIcon = pSmallIcon;
  pClass->m_pLargeIcon = pLargeIcon;
  pClass->m_pCursor = pCursor;
  pClass->m_pBrush = pBrush;

  return pClass;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
WindowClass::~WindowClass()
{
  UnregisterClass( m_ClassName, GetModuleHandle(NULL) );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

