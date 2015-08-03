/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Graphics/Mesh.cpp $
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

#include "CodeLibrary.hpp"

#include "Graphics/Mesh.hpp"
#include "IO/Path.hpp"
#include "Utility/Flags.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Math::Geometry;
using namespace Bolitho::Graphics;
using namespace Bolitho::IO;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Graphics,Mesh,Object);
ImplementRuntimeType(Bolitho::Graphics,MeshException,UserException);
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
#pragma warning(suppress:4355)
Mesh::Mesh() : VertexData(*this), TriangleData(*this)
{
  m_pVertexData = 0;
  m_pTriangleData = 0;

  InitializeInMemory();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
#pragma warning(suppress:4355)
Mesh::Mesh(LPCTSTR Filename, DWORD FileMode) : VertexData(*this), TriangleData(*this)
{
  if (FileMode == FileMode::OPEN)
    InitializeOpen(Filename, PageAccess::READWRITE);
  if (FileMode == FileMode::CREATE)
    InitializeCreate(Filename);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::~Mesh()
{
  Free();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Free()
{
  if (m_pMappedHeader)
  {
    m_pMappedHeader = 0;
    m_pFile = 0;
  }
  else
  {
    if (m_pVertexData)
      FreeMemory(m_pVertexData);
    if (m_pTriangleData)
      FreeMemory(m_pTriangleData);
    if (m_pHeader)
      delete m_pHeader;
  }

  m_pHeader = 0;
  m_pVertexData = 0;
  m_pTriangleData = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::InitializeInMemory()
{
  m_pHeader = new MESHHEADER();
  ZeroMemory(m_pHeader, sizeof(MESHHEADER));

  m_pHeader->Signature = MESHSIGNATURE;
  m_pHeader->Version = MESHVERSION;

  InitializeMapping();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::InitializeMapping()
{
  if (m_pMappedHeader)
  {
    VertexData.Initialize(&m_pHeader->VertexDataDecl, &m_pHeader->Extents[m_pHeader->VertexDataDecl.Extent]);
    TriangleData.Initialize(&m_pHeader->TriangleDataDecl, &m_pHeader->Extents[m_pHeader->TriangleDataDecl.Extent]);
  }
  else
  {
    VertexData.Initialize(&m_pHeader->VertexDataDecl, 0);
    TriangleData.Initialize(&m_pHeader->TriangleDataDecl, 0);
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::InitializeOpen(LPCTSTR Filename, DWORD Flags)
{
  m_pFile = File::Open(Filename, FileMode::OPEN, FileAccess::READWRITE);
  m_pMappedHeader = m_pFile->MapView(0, RoundUp<ULONGLONG>(sizeof(MESHHEADER), 65536), PageAccess::READWRITE);

  m_pHeader = (MESHHEADER*)m_pMappedHeader->GetBaseAddress();

  if (m_pHeader->Signature != MESHSIGNATURE)
    throw MeshException(this, String::Format("The file \'%s\' is not a valid mesh", Filename));

  if (m_pHeader->Version != MESHVERSION)
    throw MeshException(this, String::Format("The file \'%s\' has an recognised version number %x", Filename, m_pHeader->Version));

  InitializeMapping();
  ValidateExtents();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::InitializeCreate(LPCTSTR Filename)
{
  ULONGLONG ReservedLength = RoundUp<ULONGLONG>(sizeof(MESHHEADER), 65536);

  m_pFile = File::Open(Filename, FileMode::CREATE);
  m_pFile->Resize(ReservedLength);
  m_pMappedHeader = m_pFile->MapView(0, ReservedLength, PageAccess::READWRITE);
  m_pHeader = (MESHHEADER*)m_pMappedHeader->GetBaseAddress();

  m_pHeader->Signature = MESHSIGNATURE;
  m_pHeader->Version = MESHVERSION;
  m_pHeader->Extents[0].Offset = 0;
  m_pHeader->Extents[0].Length = ReservedLength;
  m_pHeader->Extents[0].Flags = EXTENT::RESERVED;
  m_pHeader->Extents[1].Offset = ReservedLength;
  m_pHeader->Extents[1].Length = 0;
  m_pHeader->Extents[1].Flags = EXTENT::END;
  
  InitializeMapping();
  ValidateExtents();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Import(LPCTSTR Filename, IProgress* pProgress)
{
  if (Path::ExtensionMatches(Filename, "Ply"))
    ImportPLY(Filename, pProgress);
  else if (Path::ExtensionMatches(Filename, "Bnpts"))
    ImportBNPTS(Filename, pProgress);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Export(LPCTSTR Filename, IProgress* pProgress)
{
}

UINT N = 0;
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ConstructSpatialIndex(UINT MaximumNodeSize)
{
  ComputeBoundingBox(TRUE);

  /* Constructs an octree based on vertex positions */
  UINT PositionOffset = VertexData.GetAttributeOffset(VertexAttribute::POSITION3);
  Assert(PositionOffset < VertexData.GetElementSize());

  SpatialIndexConstructionNode Root;
  Root.Count = VertexData.GetCount();

  SpatialIndexConstructionNode* pNode = &Root;
  UINT Offset = 0;
  
  Ptr<File> pFile = File::OpenTemporary();
  pFile->SetBufferSize(1048576);
  VertexData.Dump(pFile, 0, VertexData.GetCount());
  pFile->Seek(0);

  Split(pFile, PositionOffset, VertexData.GetElementSize(), m_BoundingBox.Min(), m_BoundingBox.Max(), pNode, Offset, MaximumNodeSize, 10);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::Split(Ptr<SeekableStream> pTableData, UINT PositionOffset, UINT Stride, Vector3F MinimumBound, Vector3F MaximumBound, SpatialIndexConstructionNode* pNode, UINT& Offset, UINT MaximumNodeSize, UINT MaximumDepth)
{
  if (pNode->Count < MaximumNodeSize || MaximumDepth == 0)
  {
    N--;
    Trace("Done %d %d %d", N, Offset, pNode->Count);
    pNode->pStorage = 0;
    pNode->Offset = Offset;
    Offset += pNode->Count;    
    return;
  }

  //Trace("Splitting File");
  //Trace("Count: %d", pNode->Count);
  //Trace("Min: %s  Max: %s", MinimumBound.ToString().ConstStr(), MaximumBound.ToString().ConstStr());
  Vector3F Center = MinimumBound + 0.5f * (MaximumBound - MinimumBound);

  LPBYTE pBuffer = new BYTE[Stride];
  pNode->CreateChildren();
  N += 8;
  for (UINT i=0; i<pNode->Count; i++)
  {
    pTableData->Read(pBuffer, Stride);
    Vector3F& Position = *(Vector3F*)(pBuffer + PositionOffset);
    
    UINT Index = 0;
    if (Position[0] > Center[0])
      Index |= 1;
    if (Position[1] > Center[1])
      Index |= 2;
    if (Position[2] > Center[2])
      Index |= 4;

    pNode->pChildren[Index].pStorage->Write(pBuffer, Stride);
    pNode->pChildren[Index].Count++;
  }

  for (SIZE_T i=0; i<8; i++)
  {
    pNode->pChildren[i].pStorage->Seek(0);

    Vector3F NewMinimumBound = MinimumBound;
    Vector3F NewMaximumBound = MaximumBound;

    if (i & 1)
      NewMinimumBound[0] = Center[0];
    else
      NewMaximumBound[0] = Center[0];

    if (i & 2)
      NewMinimumBound[1] = Center[1];
    else
      NewMaximumBound[1] = Center[1];

    if (i & 4)
      NewMinimumBound[2] = Center[2];
    else
      NewMaximumBound[2] = Center[2];

    Split(pNode->pChildren[i].pStorage, PositionOffset, Stride, NewMinimumBound, NewMaximumBound, &pNode->pChildren[i], Offset, MaximumNodeSize, MaximumDepth-1);
  }
  N--;

  delete[] pBuffer;
}

/*
VOID Mesh::ResizeInMemory(UINT VertexCount, UINT TriangleCount)
{
  LPVOID pVertexData = 0;
  LPVOID pTriangleData = 0;

  if (VertexCount > 0)
  {
    pVertexData = AllocateMemory(VertexCount * m_pHeader->VertexDataDecl.ElementSize);
    CopyMemory(pVertexData, m_pVertexData, Min(VertexCount, m_pHeader->VertexDataDecl.Count) * m_pHeader->VertexDataDecl.ElementSize);
  }

  if (TriangleCount > 0)
  {
    pTriangleData = AllocateMemory(TriangleCount * m_pHeader->TriangleDataDecl.ElementSize);
    CopyMemory(pTriangleData, m_pTriangleData, Min(TriangleCount, m_pHeader->TriangleDataDecl.Count) *  m_pHeader->TriangleDataDecl.ElementSize);
  }

  if (m_pVertexData)
    FreeMemory(m_pVertexData);
  if (m_pTriangleData)
    FreeMemory(m_pTriangleData);

  m_pVertexData = pVertexData;
  m_pTriangleData = pTriangleData;

  m_pHeader->VertexDataDecl.Count = VertexCount;
  m_pHeader->TriangleDataDecl.Count = TriangleCount;

}
*/
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ValidateExtents()
{
  if (m_pHeader->Extents[0].Flags != EXTENT::RESERVED)
    throw MeshException(this, "The first extent is not reserved");

  ULONGLONG Offset = 0;
  UINT EndExtent = 0;
  for (UINT i=0; i<64; i++)
  {
    if (m_pHeader->Extents[i].Offset != Offset)
      throw MeshException(this, String::Format("Extent %d does not began after the previous extent", i));
    Offset += m_pHeader->Extents[i].Length;

    if (m_pHeader->Extents[i].Flags == EXTENT::END)
    {
      EndExtent = i;
      break;
    }    
  }

  if (m_pFile->Length() != Offset)
    throw MeshException(this, "The end extent does not include all free space");
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ResizeExtent(UINT i, ULONGLONG Length)
{
  Length = RoundUp<ULONGLONG>(Length, 65536);

  UINT EndExtent = 0;
  for (UINT j=0; j<64; j++)
  {
    if (m_pHeader->Extents[j].Flags == EXTENT::END)
    {
      EndExtent = j;
      break;
    }
  }

  if (m_pHeader->Extents[i].Length > Length)
  {
    /* Shrink the extent */
    ULONGLONG Offset = m_pHeader->Extents[i].Offset;
    ULONGLONG OldLength = m_pHeader->Extents[i].Length;
  }
  else if (m_pHeader->Extents[i].Length < Length)
  {
    /* Grow the extent */
    ULONGLONG Offset = m_pHeader->Extents[i].Offset;
    ULONGLONG OldLength = m_pHeader->Extents[i].Length;
    ULONGLONG OldEndExtentOffset = m_pHeader->Extents[EndExtent].Offset;

    ULONGLONG Delta = Length - OldLength;
    
    if (m_pHeader->Extents[EndExtent].Length < Delta)
    {
      ULONGLONG NewLength = m_pHeader->Extents[EndExtent].Offset + Length;
      
      m_pMappedHeader = 0;
      m_pFile->Resize(NewLength);
      m_pMappedHeader = m_pFile->MapView(0, RoundUp<ULONGLONG>(sizeof(MESHHEADER), 65536), PageAccess::READWRITE);

      m_pHeader->Extents[EndExtent].Length = Length;
    }

    for (UINT j=i+1; j<=EndExtent; j++)
      m_pHeader->Extents[j].Offset += Delta;
    m_pHeader->Extents[EndExtent].Length -= Delta;

    m_pMappedHeader = 0;
    m_pFile->MoveBlock(Offset + OldLength, Offset + Length, OldEndExtentOffset - Offset - OldLength);
    m_pMappedHeader = m_pFile->MapView(0, RoundUp<ULONGLONG>(sizeof(MESHHEADER), 65536), PageAccess::READWRITE);

    InitializeMapping();
  }
  m_pHeader->Extents[i].Length = Length;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::AllocateExtent(ULONGLONG Length)
{
  Length = RoundUp<ULONGLONG>(Length, 65536);

  UINT EndExtent = 0;
  for (UINT i=0; i<64; i++)
  {
    if (m_pHeader->Extents[i].Flags == EXTENT::END)
    {
      EndExtent = i;
      break;
    }
  }

  if (m_pHeader->Extents[EndExtent].Length < Length)
  {
    ULONGLONG NewLength = m_pFile->Length() + Length - m_pHeader->Extents[EndExtent].Length;
    
    /* Otherwise, resize and allocate from the end */
    m_pMappedHeader = 0;
    m_pFile->Resize(NewLength);
    m_pMappedHeader = m_pFile->MapView(0, RoundUp<ULONGLONG>(sizeof(MESHHEADER), 65536), PageAccess::READWRITE);
    InitializeMapping();

    m_pHeader->Extents[EndExtent].Length = Length;
    m_pHeader->Extents[EndExtent].Flags = EXTENT::USED;

    m_pHeader->Extents[EndExtent + 1].Flags = EXTENT::END;
    m_pHeader->Extents[EndExtent + 1].Offset = NewLength;
    m_pHeader->Extents[EndExtent + 1].Length = 0;
  }
  else
  {
    m_pHeader->Extents[EndExtent + 1].Flags = EXTENT::END;
    m_pHeader->Extents[EndExtent + 1].Length = m_pHeader->Extents[EndExtent].Length - Length;
    m_pHeader->Extents[EndExtent + 1].Offset = m_pHeader->Extents[EndExtent].Offset + Length;

    m_pHeader->Extents[EndExtent].Flags = EXTENT::USED;
    m_pHeader->Extents[EndExtent].Length = Length;
  }
  return EndExtent;
  
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::FreeExtent(UINT i)
{
  m_pHeader->Extents[i].Flags = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<IO::FileView> Mesh::MapExtent(UINT i, DWORD PageAccess)
{
  return m_pFile->MapView(m_pHeader->Extents[i].Offset, m_pHeader->Extents[i].Length, PageAccess);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ElementDecl VertexAttribute::POSITION3 = ElementDecl("Position", ElementDeclType::FLOAT, 3);
CONST ElementDecl VertexAttribute::POSITION2 = ElementDecl("Position", ElementDeclType::FLOAT, 2);
CONST ElementDecl VertexAttribute::NORMAL2 = ElementDecl("Normal", ElementDeclType::FLOAT, 2);
CONST ElementDecl VertexAttribute::NORMAL3 = ElementDecl("Normal", ElementDeclType::FLOAT, 3);
CONST ElementDecl VertexAttribute::DIFFUSE = ElementDecl("Diffuse", ElementDeclType::BYTE, 3);
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ElementDecl TriangleAttribute::VERTICES = ElementDecl("Vertices", ElementDeclType::INT, 3);
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
Mesh::DataTable::DataTable(Mesh& M) : m_Mesh(M)
{
  m_pHeader = 0;
  m_pExtent = 0;
  m_pData = 0;
  m_MappingFlags = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::Initialize(TABLEHEADER *pHeader, EXTENT* pExtent)
{
  m_pHeader = pHeader;
  m_pExtent = pExtent;

  m_pData = 0;
  m_MappingFlags = 0;

  m_Declaration.Resize(m_pHeader->AttributeCount);
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
    m_Declaration[i] = ElementDecl(m_pHeader->ElementDecl[i].Name, m_pHeader->ElementDecl[i].Type, m_pHeader->ElementDecl[i].Length);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::DataTable::GetCount() CONST
{
  return m_pHeader->Count;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG Mesh::DataTable::GetSize() CONST
{
  return m_pHeader->Count * m_pHeader->ElementSize;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::DataTable::GetElementSize() CONST
{
  return m_pHeader->ElementSize;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Mesh::DataTable::HasAttribute(CONST ElementDecl& A) CONST
{
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
    if (m_Declaration[i] == A)
      return TRUE;
  return FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::DataTable::GetAttributeOffset(CONST ElementDecl& A) CONST
{
  UINT Offset = 0;
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
    if (m_Declaration[i] == A)
      return Offset;
    else
      Offset += m_Declaration[i].GetSize();
  return -1;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::DataTable::GetAttributeIndex(CONST ElementDecl& A) CONST
{
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
    if (m_Declaration[i] == A)
      return (UINT)i;
  return -1;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::AddAttribute(CONST ElementDecl& A, IProgress* pProgress)
{
  UINT OldElementSize = m_pHeader->ElementSize;
  UINT NewElementSize = m_pHeader->ElementSize + A.GetSize();

  m_Declaration.Add(A);
  m_pHeader->ElementSize += A.GetSize();
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
  {
    m_pHeader->ElementDecl[i].Type = m_Declaration[i].Type;
    m_pHeader->ElementDecl[i].Length = m_Declaration[i].Length;
    ZeroMemory(m_pHeader->ElementDecl[i].Name, 32);
    CopyMemory(m_pHeader->ElementDecl[i].Name, m_Declaration[i].Name, Min<SIZE_T>(m_Declaration[i].Name.Length(), 32));
  }
  m_pHeader->AttributeCount = (UINT)m_Declaration.Length();

  Resize(m_pHeader->Count);

  MapView(PageAccess::READWRITE);
  LPBYTE pFrom = (LPBYTE)m_pData + OldElementSize*m_pHeader->Count;
  LPBYTE pTo = (LPBYTE)m_pData + NewElementSize*m_pHeader->Count;
  
  for (UINT i=0; i<m_pHeader->Count; i++)
  {
    pFrom -= OldElementSize;
    pTo -= NewElementSize;
    MoveMemory(pTo, pFrom, OldElementSize);    
    ZeroMemory(pTo + OldElementSize, NewElementSize-OldElementSize); 
  }
  Unmap();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::SetElementDecl(CONST Array<ElementDecl>& Decl, IProgress* pProgress)
{
  throw NotImplementedException(&m_Mesh);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::RemoveAttribute(CONST ElementDecl& A)
{
  UINT Offset = 0;
  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
  {
    if (m_Declaration[i] == A)
      break;
    Offset += m_Declaration[i].GetSize();
  }

  for (SIZE_T i=0; i<m_Declaration.Length(); i++)
  {
    m_pHeader->ElementDecl[i].Type = m_Declaration[i].Type;
    m_pHeader->ElementDecl[i].Length = m_Declaration[i].Length;
    ZeroMemory(m_pHeader->ElementDecl[i].Name, 32);
    CopyMemory(m_pHeader->ElementDecl[i].Name, m_Declaration[i].Name, Min<SIZE_T>(m_Declaration[i].Name.Length(), 32));
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::RemoveAllAttributes()
{
  m_Declaration.Resize(0);
  ZeroMemory(m_pHeader->ElementDecl, sizeof(ELEMENTDECL)*32);
  m_pHeader->AttributeCount = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST ElementDecl& Mesh::DataTable::GetAttribute(UINT i)
{
  return m_Declaration[i];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
CONST Array<ElementDecl>& Mesh::DataTable::GetAttributes() CONST
{
  return m_Declaration;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::Resize(UINT Count)
{
  ULONGLONG Length = Count * m_pHeader->ElementSize;
  
  Unmap();
  if (Length == 0)
  {
    if (m_pHeader->Extent != 0)
    {
      m_Mesh.FreeExtent(m_pHeader->Extent);
      m_pHeader->Extent = 0;
    }
  }
  else if (m_pHeader->Extent == 0)
  {
    m_pHeader->Extent = m_Mesh.AllocateExtent(Length);
  }
  else
  {
    m_Mesh.ResizeExtent(m_pHeader->Extent, Length);
  }

  m_Mesh.InitializeMapping();
  m_pHeader->Count = Count;

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::MapView(DWORD PageAccess)
{
  if (m_pHeader->Extent == 0)
  {
    m_pMappedView = 0;
    m_pData = 0;
    m_MappingFlags = 0;
    return;
  }

  if (m_MappingFlags == PageAccess)
    return;

  if (m_MappingFlags == 0 && m_pMappedView)
  {
    if (m_pMappedView->GetPageAccess() == PageAccess)
    {
      m_pData = m_pMappedView->GetBaseAddress();
      m_MappingFlags = PageAccess;
      return;
    }
    m_pMappedView = 0;
  }

  if (m_MappingFlags != PageAccess && m_pMappedView)
    throw MeshException(&m_Mesh, "The table is already mapped");

  m_pMappedView = m_Mesh.MapExtent(m_pHeader->Extent, PageAccess);
  m_pData = m_pMappedView->GetBaseAddress();
  m_MappingFlags = PageAccess;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::Unmap(BOOL Force)
{
  if (m_pMappedView && m_MappingFlags == 0 || Force)
    m_pMappedView = 0;
  m_MappingFlags = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::Lock(UINT i, UINT Length)
{
  m_pMappedView->Lock(i * m_pHeader->ElementSize, Length * m_pHeader->ElementSize);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::Unlock(UINT i, UINT Length)
{
  m_pMappedView->Unlock(i * m_pHeader->ElementSize, Length * m_pHeader->ElementSize);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::Load(Ptr<IO::Stream> pSource, UINT TableOffset, UINT Length)
{
  if (IsMapped())
    throw MeshException(&m_Mesh, "Unable to load the table -- the table is mapped");
  m_pMappedView = 0;

  ULONGLONG FileOffset = m_pExtent->Offset + (TableOffset * m_pHeader->ElementSize);
  ULONGLONG FileLength = Length * m_pHeader->ElementSize;

  m_Mesh.m_pFile->Seek(FileOffset);
  pSource->ReadToStream(m_Mesh.m_pFile, FileLength);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::DataTable::Dump(Ptr<IO::Stream> pDestination, UINT TableOffset, UINT Length)
{
  if (IsMapped())
    throw MeshException(&m_Mesh, "Unable to dump the table -- the table is mapped");
  m_pMappedView = 0;

  ULONGLONG FileOffset = m_pExtent->Offset + (TableOffset * m_pHeader->ElementSize);
  ULONGLONG FileLength = Length * m_pHeader->ElementSize;

  m_Mesh.m_pFile->Seek(FileOffset);
  pDestination->WriteFromStream(m_Mesh.m_pFile, FileLength);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

