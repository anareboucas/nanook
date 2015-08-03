/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/StatusBar.cpp $
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

#include "UI/StatusBar.hpp"

using namespace Bolitho;
using namespace Bolitho::UI;

//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<WindowClass> StatusBar::m_pStatusClass;
//------------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,StatusBar,Control);
//------------------------------------------------------------------------------------------------------------------------------------------------------
StatusBar::StatusBar()
{
  if (m_pStatusClass == 0)
    m_pStatusClass = WindowClass::Attach(STATUSCLASSNAME);
  m_pStatusClass.AddRef();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
StatusBar::~StatusBar()
{
  m_pStatusClass.Release();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL StatusBar::Create( Ptr<Window> pParent )
{
  Window::Create( m_pStatusClass, TEXT(""), WS_CHILD | WS_VISIBLE, 0, pParent );
  return TRUE;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID StatusBar::SetText( UINT Part, LPCTSTR Text )
{
  SendMessage( SB_SETTEXT, Part, (LPARAM)Text );
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID StatusBar::SetParts( Array<INT> PartSizes )
{
  if (!PartSizes.IsEmpty())
    SendMessage( SB_SETPARTS, PartSizes.Length(), (LPARAM)&PartSizes[0] );
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID StatusBar::AddPanel(StatusBarPanel *pPanel)
{
  pPanel->m_pStatusBar = this;
  m_Panels.Add( pPanel );
  RecalcLayout();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID StatusBar::RecalcLayout()
{
  Array<INT> Sizes;

  enum { LEFT, FLOW, RIGHT } Align = LEFT;
  SIZE_T FlowIndex = -1;
  UINT Width = 0;
  Rect R = GetWindowRect();

  for (SIZE_T i=0; i<m_Panels.Length(); i++)
  {
    if (!m_Panels[i]->m_Visible)
      continue;

    m_Panels[i]->m_Index = (UINT)Sizes.Length();
    Width += m_Panels[i]->m_Width;
    Sizes.Add(Width);
    
    if (Align == LEFT && m_Panels[i]->m_Width == 0)
    {
      Align = FLOW;
      FlowIndex = i;
    }
    else if (Align == FLOW)
    {
      if (m_Panels[i]->m_Width == 0)
        throw InvalidOperationException(this, TEXT("Cannot have more than one StatusBarPanel in flow mode") );
      else  
        Align = RIGHT;
    }
  }

  if (R.Width() > (LONG)Width)
    for (SIZE_T i=FlowIndex; i<Sizes.Length(); i++)
      Sizes[i] += R.Width() - Width;

  SetParts(Sizes);

  UINT j = 0;
  for (SIZE_T i=0; i<m_Panels.Length(); i++)
  {
    if (!m_Panels[i]->m_Visible)
      continue;
    m_Panels[i]->RecalcLayout( j, Sizes[j] );
    j++;
  }

}

//------------------------------------------------------------------------------------------------------------------------------------------------------
StatusBarPanel::StatusBarPanel( UINT Width )
{
  m_Width = Width;
  m_Visible = TRUE;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
StatusBarPanel::StatusBarPanel()
{
  m_Width = 0;
  m_Visible = TRUE;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID TextStatusBarPanel::SetText(LPCTSTR Value)
{
  m_Value = Value;
  m_pStatusBar->SetText( m_Index, Value );
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID TextStatusBarPanel::SetText(CONST String& Value)
{
  m_Value = Value;
  m_pStatusBar->SetText( m_Index, Value.ConstStr() );
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID TextStatusBarPanel::RecalcLayout( UINT Index, UINT Position )
{
  StatusBarPanel::RecalcLayout( Index, Position );
  m_pStatusBar->SetText( m_Index, m_Value.ConstStr() );
}

