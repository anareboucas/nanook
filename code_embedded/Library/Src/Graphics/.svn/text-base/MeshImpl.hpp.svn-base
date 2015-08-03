/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Graphics/MeshImpl.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1313 $
 * Last Updated: $Date: 2007-12-02 17:54:01 -0500 (Sun, 02 Dec 2007) $
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* Mesh::DataTable::Row(UINT i)
{
  LPBYTE pData = (LPBYTE)m_pData;
  return (T*)(pData + m_pHeader->ElementSize*i);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T* Mesh::DataTable::Row(UINT i) CONST
{
  LPCBYTE pData = (LPCBYTE)m_pData;
  return (CONST T*)(pData + m_pHeader->ElementSize*i);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T& Mesh::DataTable::Attribute(UINT i, UINT Offset)
{
  LPBYTE pData = (LPBYTE)m_pData;
  return *(T*)(pData + m_pHeader->ElementSize*i + Offset);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
CONST T& Mesh::DataTable::Attribute(UINT i, UINT Offset) CONST
{
  LPCBYTE pData = (LPCBYTE)m_pData;
  return *(CONST T*)(pData + m_pHeader->ElementSize*i + Offset);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func>
VOID Mesh::DataTable::Iterate(Func F)
{
  Iterate<T>(F, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U>
VOID Mesh::DataTable::Iterate(Func F, U& P0)
{
  Iterate<T>(F, 0, P0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U, class V>
VOID Mesh::DataTable::Iterate(Func F, U& P0, V& P1)
{
  Iterate<T>(F, 0, P0, P1);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func>
VOID Mesh::DataTable::Iterate(Func F, CONST ElementDecl& Attribute)
{
  Iterate<T>(F, GetAttributeOffset(Attribute));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U>
VOID Mesh::DataTable::Iterate(Func F, CONST ElementDecl& Attribute, U& P0)
{
  Iterate<T>(F, GetAttributeOffset(Attribute), P0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U, class V>
VOID Mesh::DataTable::Iterate(Func F, CONST ElementDecl& Attribute, U& P0, V& P1)
{
  Iterate<T>(F, GetAttributeOffset(Attribute), P0, P1);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func>
VOID Mesh::DataTable::Iterate(Func F, UINT Offset)
{
  MapView(PageAccess::READWRITE);

  UINT Stride = m_pHeader->ElementSize;
  LPBYTE pData = (LPBYTE)m_pData + Offset;

  UINT j = 0;
  for (UINT i=0; i<m_pHeader->Count; i++, pData+=Stride)
  {
    F(i, *(T*)pData);

    if (j - i > 1048576)
    {
      Unlock(j, i - j - 65536);
      j = i-65536;
    }
  }
  
  Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U>
VOID Mesh::DataTable::Iterate(Func F, UINT Offset, U& P0)
{
  MapView(PageAccess::READWRITE);

  UINT Stride = m_pHeader->ElementSize;
  LPBYTE pData = (LPBYTE)m_pData + Offset;

  UINT j = 0;
  for (UINT i=0; i<m_pHeader->Count; i++, pData+=Stride)
  {
    F(i, *(T*)pData, P0);

    if (i - j > 1048576)
    {
      Unlock(j, i - j - 65536);
      j = i-65536;
    }
  }

  Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class Func, class U, class V>
VOID Mesh::DataTable::Iterate(Func F, UINT Offset, U& P0, V& P1)
{
  MapView(PageAccess::READWRITE);

  UINT Stride = m_pHeader->ElementSize;
  LPBYTE pData = (LPBYTE)m_pData + Offset;

  UINT j = 0;
  for (UINT i=0; i<m_pHeader->Count; i++, pData+=Stride)
  {
    F(i, *(T*)pData, P0, P1);

    if (i - j > 1048576)
    {
      Unlock(j, i - j - 65536);
      j = i-65536;
    }
  }

  Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
