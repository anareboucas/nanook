/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/UI/FileOpenDialog.cpp $
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

#include "CodeLibrary.hpp"

#include "UI/FileOpenDialog.hpp"

using namespace Bolitho;
using namespace Bolitho::UI;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::UI,FileOpenDialog,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
FileOpenDialog::FileOpenDialog()
{
  ZeroMemory( &m_OFN, sizeof(OPENFILENAME) );
  ZeroMemory( &m_Filename[0], MAX_PATH*sizeof(TCHAR) );
  ZeroMemory( &m_Filters[0], 500*sizeof(TCHAR) );
  m_FilterIndex = 0;
 
  m_OFN.lStructSize = sizeof(OPENFILENAME);
  m_OFN.Flags = OFN_EXPLORER;
  m_OFN.lpstrFile = m_Filename;
  m_OFN.nMaxFile = MAX_PATH;
  m_OFN.lpstrFilter = m_Filters;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
FileOpenDialog::~FileOpenDialog()
{
  
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
INT FileOpenDialog::ShowModal( CONST Ptr<Window> pParent )
{
  m_OFN.hwndOwner = (pParent != 0) ? pParent->Handle() : 0;

  if (GetOpenFileName( &m_OFN ))
    return IDOK;
  return IDCANCEL;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileOpenDialog::SetInitialDirectory( LPCTSTR pDirectory )
{
  m_OFN.lpstrInitialDir = CharTraits<TCHAR>::Duplicate(pDirectory);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileOpenDialog::SetDefaultExtension( LPCTSTR pExt )
{
  m_OFN.lpstrDefExt = CharTraits<TCHAR>::Duplicate(pExt);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileOpenDialog::SetFilename( LPCTSTR pFilename )
{
  SIZE_T Length = Max( CharTraits<TCHAR>::Length(pFilename), (SIZE_T)MAX_PATH );
  CopyMemory( m_Filename, pFilename, Length );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String FileOpenDialog::GetFilename() CONST
{ 
  return String(m_Filename);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileOpenDialog::AddFilter( LPCTSTR Description, LPCTSTR FileSpec )
{
  LPCTSTR pCh = Description;
  while(*pCh != 0)
    m_Filters[m_FilterIndex++] = *pCh++;
  m_FilterIndex++;
  pCh = FileSpec;
  while(*pCh != 0)
    m_Filters[m_FilterIndex++] = *pCh++;
  m_FilterIndex++;
}

