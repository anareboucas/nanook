/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/Controls/EditorControl.hpp $
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

#if defined(USE_SCINTILLA)

#include "UI/Controls/Control.hpp"

namespace Bolitho
{
  namespace UI
  {
    class LIBRARY_API EditorControl : public Control
    {
      DeclareRuntimeType();
    public:
      EditorControl();
      virtual ~EditorControl();

      virtual BOOL Create( Ptr<Window> pParent );
      virtual BOOL Attach( Dialog* pDialog, UINT ControlId );

      /* Text */
      String GetText() CONST;
      LONG GetTextLength() CONST;
      String GetLine( LONG i ) CONST;
      LONG GetLineLength( LONG i ) CONST;
      String GetTextRange( LONG Begin, LONG End );
      String GetTextRange( Pair<LONG, LONG> Range )
      { return GetTextRange( Range.First, Range.Second ); }

      TCHAR GetChar( LONG i ) CONST; 
      
      VOID SetText( LPCTSTR Text );
      VOID ReplaceSelection( LPCTSTR Text );
      VOID AddText( LPCTSTR Text );
      VOID AppendText( LPCTSTR Text );
      VOID InsertText( LONG i, LPCTSTR Text ); 
      VOID ClearAll();

      /* Misc */
      VOID SetReadOnly( BOOL ReadOnly = TRUE );
      BOOL IsReadOnly() CONST;

      VOID SetOvertype( BOOL Overtype = TRUE );
      BOOL IsOvertype() CONST;

      BOOL IsModified() CONST;

      /* Clipboard */
      VOID Cut();
      VOID Copy() CONST;
      VOID Copy( Pair<LONG,LONG> Range ) CONST;
      VOID Copy( LPCTSTR Text ) CONST;
      VOID Paste();
      BOOL CanPaste() CONST;

      /* Undo / Redo */
      VOID Undo();
      VOID Redo();
      BOOL CanUndo() CONST;
      BOOL CanRedo() CONST;
      VOID ClearUndoBuffer();

      /* Searching and Replacing */
      Pair<LONG,LONG> Find( LPCTSTR Text, BOOL MatchCase = FALSE, BOOL WholeWord = FALSE, Pair<LONG,LONG> Range = Pair<LONG,LONG>(0, -1) );
      
      /* Style */

      /* Selection and Information */
      LONG GetLineCount() CONST;
      LONG GetFirstVisibleLine() CONST;
      LONG GetVisibleLineCount() CONST;

      VOID SetSelection( Pair<LONG,LONG> Range );
      Pair<LONG,LONG> GetSelection() CONST;
      
      VOID GotoLine( LONG i );
      
      VOID SetPosition( LONG i );
      LONG GetPosition() CONST;

      VOID SetAnchor( LONG i );
      LONG GetAnchor() CONST;

      /* Caret */

    protected:
      static Ptr<WindowClass> m_pScintillaClass;

      virtual LRESULT OnCreate( CREATESTRUCT* pCreateStruct );
      virtual LRESULT OnSize( UINT Type, Size S );
      virtual LRESULT OnNotify( LPNMHDR pNMHdr );

      LRESULT (*m_ScintillaFunc)(LPVOID,UINT,WPARAM,LPARAM);
	    LPVOID m_ScintillaPtr;

      LRESULT EditFunc( UINT Msg, WPARAM wParam, LPARAM lParam ) CONST
      { return m_ScintillaFunc( m_ScintillaPtr, Msg, wParam, lParam ); }

    };
  }
}

#endif
