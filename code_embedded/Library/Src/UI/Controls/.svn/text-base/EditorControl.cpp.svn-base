/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/Controls/EditorControl.cpp $
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

#if defined(USE_SCINTILLA)

#define PLAT_WIN 1
#include <scintilla.h>
#pragma comment(lib,"scilexer.lib")
#pragma comment(lib,"imm32.lib")

using namespace Bolitho::UI;
using namespace Bolitho;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,EditorControl,View);
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<WindowClass> EditorControl::m_pScintillaClass;
//----------------------------------------------------------------------------------------------------------------------------------------------------
EditorControl::EditorControl()
{
  m_ScintillaFunc = 0;
  m_ScintillaPtr  = 0;

  if (m_pScintillaClass == 0)
    m_pScintillaClass = WindowClass::Attach(TEXT("Scintilla"));
  else
    m_pScintillaClass.AddRef();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL EditorControl::Attach( Dialog* pDialog, UINT ControlId ) 
{
  return FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
EditorControl::~EditorControl()
{
  m_pScintillaClass.Release();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL EditorControl::Create( Ptr<Window> pParent )
{
  Window::Create( m_pScintillaClass, TEXT(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, 0, pParent );

  m_ScintillaFunc = (LRESULT (*)(LPVOID,UINT,WPARAM,LPARAM))SendMessage( SCI_GETDIRECTFUNCTION, 0, 0 );
  m_ScintillaPtr = (LPVOID)SendMessage( SCI_GETDIRECTPOINTER, 0, 0  );

  EditFunc( SCI_SETMARGINWIDTHN, 0, 40 );
  EditFunc( SCI_SETMARGINWIDTHN, 1, 0 );
  EditFunc( SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)TEXT("Consolas") );
  EditFunc( SCI_STYLESETSIZE, STYLE_DEFAULT, 10 );
  EditFunc( SCI_SETWRAPMODE, 2, 0 );

  return m_hWnd != 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT EditorControl::OnCreate( CREATESTRUCT* pCreateStruct )
{
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT EditorControl::OnSize( UINT Type, Size S )
{
  if (m_ScintillaPtr)
  {
    SetReadOnly(FALSE);
    String Str = String::Format(TEXT("OnSize %d %d\r\n"), S[0], S[1] );
    AppendText( Str );
    int N = (int)EditFunc( SCI_GETLINECOUNT, 0, 0 );
    EditFunc( SCI_GOTOLINE, N, 0 );
    SetReadOnly(TRUE);
  }

  return 1;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT EditorControl::OnNotify( LPNMHDR pNMHdr )
{
  switch( pNMHdr->code )
  {
  case SCN_KEY:
    Trace(TEXT("Key"));
    break;
  case SCN_CHARADDED:
    Trace(TEXT("Char Added"));
    break;
  default:
    break;
  };
  return 1;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String EditorControl::GetText() CONST
{
  SIZE_T Length = (SIZE_T)EditFunc( SCI_GETLENGTH, 0, 0 );
  String Text;
  Text.Resize(Length);
  EditFunc( SCI_GETTEXT, (WPARAM)Length, (LPARAM)Text.Str() );
  return Text;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LONG EditorControl::GetTextLength() CONST
{
  return (LONG)EditFunc( SCI_GETLENGTH, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::SetText( LPCTSTR Text )
{
  EditFunc( SCI_SETTEXT, 0, (LPARAM)Text );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String EditorControl::GetLine( LONG i ) CONST
{
  String Text;
  SIZE_T Length = (SIZE_T)EditFunc( SCI_LINELENGTH, (WPARAM)i, 0 );
  Text.Resize(Length);
  EditFunc( SCI_GETLINE, (WPARAM)i, (LPARAM)Text.Str() );
  return Text;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::ReplaceSelection( LPCTSTR Text )
{
  EditFunc( SCI_REPLACESEL, 0, (WPARAM)Text );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::SetReadOnly( BOOL ReadOnly )
{
  EditFunc( SCI_SETREADONLY, ReadOnly, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL EditorControl::IsReadOnly() CONST
{
  return (BOOL)EditFunc( SCI_GETREADONLY, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::SetOvertype( BOOL Overtype )
{
  EditFunc( SCI_SETOVERTYPE, Overtype, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL EditorControl::IsOvertype() CONST
{
  return (BOOL)EditFunc( SCI_GETOVERTYPE, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String EditorControl::GetTextRange( LONG Begin, LONG End )
{
  TextRange Range;
  String Text;
  SIZE_T Length = End-Begin;
  Text.Resize(Length);
  Range.lpstrText = Text.Str();
  Range.chrg.cpMin = Begin;
  Range.chrg.cpMax = End;

  EditFunc( SCI_GETTEXTRANGE, 0, (WPARAM)&Range );

  return Text;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::AddText( LPCTSTR Text )
{
  EditFunc( SCI_ADDTEXT, (WPARAM)CharTraits<TCHAR>::Length(Text), (LPARAM)Text ); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::AppendText( LPCTSTR Text )
{
  EditFunc( SCI_APPENDTEXT, (WPARAM)CharTraits<TCHAR>::Length(Text), (LPARAM)Text ); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::InsertText( LONG i, LPCTSTR Text )
{
  EditFunc( SCI_INSERTTEXT, (WPARAM)i, (LPARAM)Text ); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::ClearAll()
{
  EditFunc( SCI_CLEARALL, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
TCHAR EditorControl::GetChar( LONG i ) CONST
{
  return (TCHAR)EditFunc( SCI_GETCHARAT, (WPARAM)i, 0 ); 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Pair<LONG,LONG> EditorControl::Find( LPCTSTR Text, BOOL MatchCase, BOOL WholeWord, Pair<LONG,LONG> Range )
{
  TextToFind Find;
  Find.lpstrText = (LPTSTR)Text;
  Find.chrg.cpMin = Range.First;
  Find.chrg.cpMax = Range.Second == -1 ? GetTextLength() : Range.Second;
  Find.chrgText.cpMin = -1;
  Find.chrgText.cpMax = -1;
  
  WPARAM Flags = 0;
  if (MatchCase)
    Flags |= SCFIND_MATCHCASE;
  if (WholeWord)
    Flags |= SCFIND_WHOLEWORD;

  LONG Result = (LONG)EditFunc( SCI_FINDTEXT, Flags, (LPARAM)&Find );

  return Pair<LONG,LONG>(Find.chrgText.cpMin, Find.chrgText.cpMax);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::Cut()
{
  EditFunc( SCI_CUT, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::Copy() CONST
{
  EditFunc( SCI_COPY, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::Paste()
{
  EditFunc( SCI_PASTE, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL EditorControl::CanPaste() CONST
{
  return (BOOL)EditFunc( SCI_CANPASTE, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::Copy( Pair<LONG,LONG> Range ) CONST
{
  EditFunc( SCI_COPYRANGE, Range.First, Range.Second );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::Copy( LPCTSTR Text ) CONST
{
  EditFunc( SCI_COPYTEXT, CharTraits<TCHAR>::Length(Text), (LPARAM)Text );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::Undo()
{
  EditFunc( SCI_UNDO, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::Redo()
{
  EditFunc( SCI_REDO, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL EditorControl::CanUndo() CONST
{
  return (BOOL)EditFunc( SCI_CANUNDO, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL EditorControl::CanRedo() CONST
{
  return (BOOL)EditFunc( SCI_CANREDO, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID EditorControl::ClearUndoBuffer()
{
  EditFunc( SCI_EMPTYUNDOBUFFER, 0, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
LONG EditorControl::GetLineLength( LONG i ) CONST
{
  return (LONG)EditFunc( SCI_LINELENGTH, i, 0 );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL EditorControl::IsModified() CONST
{
  return (BOOL)EditFunc( SCI_GETMODIFY, 0, 0 );
}


#endif
