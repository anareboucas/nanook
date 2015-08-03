/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/ConsoleView.hpp $
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

#include "UI/View.hpp"

namespace Bolitho
{
  namespace UI
  {
    class LIBRARY_API ConsoleView : public View
    {
      DeclareRuntimeType();
    public:
      ConsoleView();

      virtual VOID Create( Ptr<Window> pParent );

    protected:
      virtual LRESULT OnPaint( DeviceContext* pDC, Rect UpdateRect );

      virtual LRESULT OnCreate( CREATESTRUCT* pCreateStruct );
      virtual LRESULT OnDestroy();

      virtual LRESULT OnSize( Size S );

      virtual LRESULT OnMouseDown( Point P, UINT Buttons );
      virtual LRESULT OnMouseUp( Point P, UINT Buttons );
      virtual LRESULT OnMouseMove( Point P, UINT Buttons );

      virtual LRESULT OnActivate( UINT Show );
      virtual LRESULT OnSetFocus( HWND hWndLost );
      virtual LRESULT OnKillFocus();

      virtual LRESULT OnVScroll( UINT Mode, UINT Position, HWND ScrollCtrl );

      Brush* m_pBkgrnd;
      Font* m_pFont;
      Size m_FontSize;
      Point m_CaretPosition;

      LONG m_ViewPosition;
      Array<String> m_TextLines;

      Point m_SelectionBegin;
      Point m_SelectionEnd;
    };
  }
}

