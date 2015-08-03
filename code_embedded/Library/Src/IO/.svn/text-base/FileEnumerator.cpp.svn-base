/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/FileEnumerator.cpp $
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

#include "IO/File.hpp"
#include "IO/Path.hpp"
#include "IO/FileEnumerator.hpp"
#include "Utility/Flags.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//---------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,FileEnumerator,Object);
//------------------------------------------------------------------------------------------------------------------------------------------------------
FileEnumerator::FileEnumerator(LPCTSTR Directory, LPCTSTR SearchPattern, DWORD Flags) 
  : 
  m_SearchPattern(TEXT(".*"))
{
	String Path = Path::FullPath(Directory); 
	if (Path.EndsWith(TEXT("\\")))
		Path = Path.Left(Path.Length()-1);
	m_Path.Push(Path);
	m_hFind.Push(NULL);
	m_Done = FALSE;
	m_Flags = Flags;
  m_SearchPattern = RegularExpression::FromFileSpec(SearchPattern, RegularExpressionFlags::NOCASE);

	NextElement();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FileEnumerator::~FileEnumerator()
{
	while (!m_hFind.IsEmpty())
		FindClose(m_hFind.Pop());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL FileEnumerator::HasMoreElements() CONST
{	
	return !m_Done;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
CONST FileInfo& FileEnumerator::CurrentElement() CONST
{
	return m_CurrentFile;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileEnumerator::NextElement()
{
	if (m_Done)
		return;

	HANDLE hFind = m_hFind.Peek();
	String Path = m_Path.Peek();
	WIN32_FIND_DATA FindData;
	
	ZeroMemory(&FindData, sizeof(FindData));

	do
	{
		hFind = m_hFind.Peek();
		Path = m_Path.Peek();

		if (hFind == NULL)
		{
			hFind = FindFirstFile(Path + TEXT("\\*.*") , &FindData);
  	  if (hFind == INVALID_HANDLE_VALUE)
			{
				DWORD Error = GetLastError();
				if (Error == ERROR_FILE_NOT_FOUND || Error == ERROR_PATH_NOT_FOUND)
				{
					m_hFind.Pop();
					m_Path.Pop();
					
					FindClose(hFind);
					
					if (m_hFind.IsEmpty())
					{
						m_Done = TRUE;
						return;
					}
					hFind = m_hFind.Peek();
					Path = m_Path.Peek();
					
					continue;
				}
				SystemException::ThrowError(this, Error);
				m_Done = TRUE;
			}

			m_hFind.Pop();
			m_hFind.Push(hFind);
		}
		else
		{
			if (FindNextFile(hFind, &FindData) == 0)
			{
				DWORD Error = GetLastError();
				if (Error == ERROR_NO_MORE_FILES)
				{
					m_hFind.Pop();
					m_Path.Pop();

					FindClose(hFind);
					if (m_hFind.IsEmpty())
					{
						m_Done = TRUE;
						return;
					}
					hFind = m_hFind.Peek();
					Path = m_Path.Peek();
					
					continue;
				}
				SystemException::ThrowError( this, Error );
			}
		}

		if (FindData.cFileName[0] == '.')
			if (FindData.cFileName[1] == 0 || FindData.cFileName[1] == '.')
				continue;

    if (FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN && !(m_Flags & FileEnumeratorFlags::HIDDEN))
      continue;

		SIZE_T Matches = m_SearchPattern.Match(FindData.cFileName);

    if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (m_Flags & FileEnumeratorFlags::RECURSIVE))
		{
			m_hFind.Push(NULL);
			m_Path.Push(Path + TEXT("\\") + FindData.cFileName);
		}

		if (Matches > 0)
			break;
	}
	while(TRUE);
	
	m_CurrentFile.Directory = Path;
	m_CurrentFile.Filename = FindData.cFileName;
	
	m_CurrentFile.AccessedOn = DateTime(FindData.ftLastAccessTime);
	m_CurrentFile.CreatedOn = DateTime(FindData.ftCreationTime);
	m_CurrentFile.ModifiedOn = DateTime(FindData.ftLastWriteTime);

	m_CurrentFile.Attributes = FindData.dwFileAttributes;
	m_CurrentFile.Length = MAKEULONGLONG(FindData.nFileSizeLow,FindData.nFileSizeHigh);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Array<String> FileEnumerator::ExpandFilePattern(LPCTSTR FilePattern, BOOL Recursive, LPCTSTR Directory)
{
  Array<String> Filenames;
  FileEnumerator E(Directory, FilePattern, Recursive ? FileEnumeratorFlags::RECURSIVE : FileEnumeratorFlags::DEFAULT);

  while (E.HasMoreElements())
  {
    if (!E.CurrentElement().IsDirectory())
      Filenames.Add(E.CurrentElement().Directory + Path::Separator + E.CurrentElement().Filename);
    E.NextElement();
  }

  return Filenames;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
