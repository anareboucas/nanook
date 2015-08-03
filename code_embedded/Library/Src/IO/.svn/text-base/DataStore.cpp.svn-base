/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/DataStore.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1269 $
 * Last Updated: $Date: 2007-11-17 13:39:46 -0500 (Sat, 17 Nov 2007) $
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
#include "IO/DataStore.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//---------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO, DataStoreException, UserException);
ImplementRuntimeType(Bolitho::IO, DataStore, Object);
//---------------------------------------------------------------------------------------------------
DataStore::Table::Stream::Stream(Ptr<DataStore::Table> pTable)
  : m_pTable(pTable)
{
  m_CurrentBlock = 0;
  m_CurrentOffset = 0;
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Table::Stream::Flush()
{}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Table::Stream::Close()
{}
//---------------------------------------------------------------------------------------------------
SIZE_T DataStore::Table::Stream::Write(LPCVOID pData, SIZE_T N)
{
  SIZE_T Offset = 0;

  /* Write whole blocks */
  while (N > m_BlockSize - m_CurrentOffset)
  {
    LPVOID pTableBlock = m_pTable->GetBlock(m_CurrentBlock);
    CopyMemory(pTableBlock, (LPCBYTE)pData + Offset, m_BlockSize - m_CurrentOffset);

    N -= m_BlockSize - m_CurrentOffset;
    Offset += (m_BlockSize - m_CurrentOffset);

    m_CurrentOffset = 0;
    m_CurrentBlock++;

    if (m_CurrentBlock >= m_pTable->GetBlockCount())
      m_pTable->Resize(m_pTable->GetBlockCount() + 1);
  }

  /* Write partial block */
  Offset += N;

  return Offset;
}
//---------------------------------------------------------------------------------------------------
SIZE_T DataStore::Table::Stream::Read(LPVOID pData, SIZE_T N)
{
  return 0;
}
//---------------------------------------------------------------------------------------------------
BOOL DataStore::Table::Stream::EndOfStream()
{
  return FALSE;
}
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
DataStore::Table::Table(DataStore* pDataStore, TABLEHEADER* pHeader)
{
  m_TranslationLookup[0] = pHeader->BlockCount + 1;
  m_TranslationLookup[1] = 0;

  m_pDataStore = pDataStore;
  m_pHeader = pHeader;
  m_HeaderOffset = (LPBYTE)pHeader - (LPBYTE)pDataStore->GetBlock(0);

  if (m_pHeader->BlockIndex0 == 0)
    m_pHeader->BlockIndex0 = pDataStore->AllocateBlock(TRUE);

  Remap();
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Table::Remap()
{
  m_pHeader = (TABLEHEADER*)((LPBYTE)m_pDataStore->GetBlock(0) + m_HeaderOffset);

  m_pBlockIndex0 = (BLOCKADDRESS*)m_pDataStore->GetBlock(m_pHeader->BlockIndex0);
  m_pBlockIndex1 = new BLOCKADDRESS*[m_pDataStore->GetBlockSize() / sizeof(BLOCKADDRESS)];
 
  for (SIZE_T i=0; i<m_pDataStore->GetBlockSize() / sizeof(BLOCKADDRESS); i++)
  {
    if (m_pBlockIndex0[i] != 0)
      m_pBlockIndex1[i] = (BLOCKADDRESS*)m_pDataStore->GetBlock(m_pBlockIndex0[i]);
    else
      m_pBlockIndex1[i] = 0;
  }
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Table::UpdateDirectory(BLOCKADDRESS TableAddress, BLOCKADDRESS PhysicalAddress) CONST
{
  BLOCKADDRESS i = TableAddress / (m_pDataStore->GetBlockSize() / 4);
  BLOCKADDRESS j = TableAddress % (m_pDataStore->GetBlockSize() / 4);
  
  if (m_pBlockIndex0[i] == 0)
  {
    m_pBlockIndex0[i] = m_pDataStore->AllocateBlock();
    m_pBlockIndex1[i] = (BLOCKADDRESS*)m_pDataStore->GetBlock(m_pBlockIndex0[i]);
  }

  m_pBlockIndex1[i][j] = PhysicalAddress;
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Table::Resize(BLOCKADDRESS BlockCount)
{
  if (BlockCount > m_pHeader->BlockCount)
  {
    for (BLOCKADDRESS i=m_pHeader->BlockCount; i<BlockCount; i++)
      UpdateDirectory(i, m_pDataStore->AllocateBlock());
  }
  else
  {
    for (BLOCKADDRESS i=BlockCount; i<m_pHeader->BlockCount; i++)
    {
      m_pDataStore->FreeBlock(Translate(i));
      UpdateDirectory(i, 0);
    }
  }
  m_pHeader->BlockCount = BlockCount;
}
//---------------------------------------------------------------------------------------------------
LPCVOID DataStore::Table::GetBlock(BLOCKADDRESS Address) CONST
{
  return m_pDataStore->GetBlock(Translate(Address));
}
//---------------------------------------------------------------------------------------------------
LPVOID DataStore::Table::GetBlock(BLOCKADDRESS Address)
{
  return m_pDataStore->GetBlock(Translate(Address));
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Table::Lock(BLOCKADDRESS Address)
{
  m_pDataStore->Lock(Translate(Address));
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Table::Unlock(BLOCKADDRESS Address)
{
  m_pDataStore->Unlock(Translate(Address));
}
//---------------------------------------------------------------------------------------------------
DataStore::BLOCKADDRESS DataStore::Table::Translate(BLOCKADDRESS TableAddress) CONST
{
  if (m_TranslationLookup[0] == TableAddress)
    return m_TranslationLookup[1];

  BLOCKADDRESS Address = 0;

  BLOCKADDRESS i = TableAddress / (m_pDataStore->GetBlockSize() / 4);
  BLOCKADDRESS j = TableAddress % (m_pDataStore->GetBlockSize() / 4);
  
  Assert(m_pBlockIndex0[i] != 0);
  Address = m_pBlockIndex1[i][j];

  m_TranslationLookup[0] = TableAddress;
  m_TranslationLookup[1] = Address;

  Assert(Address != 0);
  return Address;
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
DataStore::~DataStore()
{
  if (m_pBlockBitmaps)
    delete[] m_pBlockBitmaps;
  m_pBlockBitmaps = 0;
}
//---------------------------------------------------------------------------------------------------
Ptr<DataStore> DataStore::Create(BLOCKADDRESS BlockSize)
{
  DataStore* pStore = new DataStore();

  if (BlockSize < sizeof(STOREHEADER))
    throw DataStoreException(pStore, "The BlockSize must be at least as large as the size of the header");

  pStore->m_pFile = MemoryMappedFile::OpenAnonymous(10 * BlockSize);
  pStore->m_pView = pStore->m_pFile->CreateView();

  pStore->m_pHeader = (STOREHEADER*)pStore->m_pView->GetBasePointer();
  ZeroMemory(pStore->m_pHeader, sizeof(STOREHEADER));

  pStore->m_pHeader->DataStoreVersion = DATASTOREVERSION;
  pStore->m_pHeader->BlockAllocationIndex0 = 1;
  pStore->m_pHeader->BlockCount = 10;
  pStore->m_pHeader->BlockSize = BlockSize;
  pStore->m_pHeader->FreeBlockCount = 6;
  //pStore->m_pHeader->MetaDataTable.BlockCount = 0;
  //pStore->m_pHeader->MetaDataTable.BlockDirectory = 0;

  pStore->m_pBlockAllocationIndex0 = (BLOCKADDRESS*)pStore->GetBlock(1);
  pStore->m_pBlockBitmaps = new DWORD*[BlockSize / sizeof(BLOCKADDRESS)];
  ZeroMemory(pStore->m_pBlockBitmaps, sizeof(DWORD*) * BlockSize / sizeof(BLOCKADDRESS));

  pStore->m_pBlockAllocationIndex0[0] = 2;
  pStore->m_pBlockBitmaps[0] = (DWORD*)pStore->GetBlock(2);

  pStore->m_pBlockBitmaps[0][0] = 0xf;

  return pStore;
}
//---------------------------------------------------------------------------------------------------
Ptr<DataStore> DataStore::Create(LPCTSTR Filename, BLOCKADDRESS BlockSize)
{
  DataStore* pStore = new DataStore();

  if (BlockSize < sizeof(STOREHEADER))
    throw DataStoreException(pStore, "The BlockSize must be at least as large as the size of the header");

  pStore->m_pFile = MemoryMappedFile::Open(Filename, FileMode::CREATE);
  pStore->m_pFile->Resize(10 * BlockSize);
  pStore->m_pView = pStore->m_pFile->CreateView();

  pStore->m_pHeader = (STOREHEADER*)pStore->m_pView->GetBasePointer();
  ZeroMemory(pStore->m_pHeader, sizeof(STOREHEADER));

  pStore->m_pHeader->DataStoreVersion = DATASTOREVERSION;
  pStore->m_pHeader->BlockAllocationIndex0 = 1;
  pStore->m_pHeader->BlockCount = 10;
  pStore->m_pHeader->BlockSize = BlockSize;
  pStore->m_pHeader->FreeBlockCount = 6;
  //pStore->m_pHeader->MetaDataTable.BlockCount = 0;
  //pStore->m_pHeader->MetaDataTable.BlockDirectory = 0;

  pStore->m_pBlockAllocationIndex0 = (BLOCKADDRESS*)pStore->GetBlock(1);
  pStore->m_pBlockBitmaps = new DWORD*[BlockSize / sizeof(BLOCKADDRESS)];
  ZeroMemory(pStore->m_pBlockBitmaps, sizeof(DWORD*) * BlockSize / sizeof(BLOCKADDRESS));

  pStore->m_pBlockAllocationIndex0[0] = 2;
  pStore->m_pBlockBitmaps[0] = (DWORD*)pStore->GetBlock(2);
  pStore->m_pBlockAllocationIndex0[1] = 3;
  pStore->m_pBlockBitmaps[1] = (DWORD*)pStore->GetBlock(3);

  pStore->m_pBlockBitmaps[0][0] = 0xf;

  return pStore;
}
//---------------------------------------------------------------------------------------------------
Ptr<DataStore> DataStore::Open(LPCTSTR Filename)
{
  DataStore* pStore = new DataStore();

  pStore->m_pFile = MemoryMappedFile::Open(Filename, FileMode::OPEN);
  pStore->m_pView = pStore->m_pFile->CreateView();

  pStore->m_pHeader = (STOREHEADER*)pStore->m_pView->GetBasePointer();

  if (pStore->m_pHeader->DataStoreVersion != DATASTOREVERSION)
    throw DataStoreException(pStore, "The DataStore version number is unknown");

  pStore->m_pBlockAllocationIndex0 = (BLOCKADDRESS*)pStore->GetBlock(pStore->m_pHeader->BlockAllocationIndex0);
  pStore->m_pBlockBitmaps = new DWORD*[pStore->m_pHeader->BlockSize / sizeof(BLOCKADDRESS)];
  
  for (SIZE_T i=0; i<pStore->m_pHeader->BlockSize / sizeof(BLOCKADDRESS); i++)
  {
    if (pStore->m_pBlockAllocationIndex0[i] != 0)
      pStore->m_pBlockBitmaps[i] = (DWORD*)pStore->GetBlock(pStore->m_pBlockAllocationIndex0[i]);
    else
      pStore->m_pBlockBitmaps[i] = 0;
  }

  return pStore;
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Flush()
{
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::SetMetaData(LPCTSTR Key, LPCTSTR Value)
{
  m_MetaData.Add(Key, Value);
}
//---------------------------------------------------------------------------------------------------
String DataStore::GetMetaData(LPCTSTR Key)
{
  if (m_MetaData.Contains(Key))
    return m_MetaData[Key];
  else
    return String::Empty;
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::RemoveMetaData(LPCTSTR Key)
{
  if (m_MetaData.Contains(Key))
    m_MetaData.Remove(Key);
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::RemoveAllMetaData()
{
  m_MetaData.RemoveAll();
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Resize(BLOCKADDRESS BlockCount)
{
  if (BlockCount <= m_pHeader->BlockCount)
    return;

  ULONGLONG NewSize = (ULONGLONG)BlockCount * m_pHeader->BlockSize;

  m_pView = 0;
  m_pHeader = 0;

  m_pFile->Resize(NewSize);
  m_pView = m_pFile->CreateView();

  m_pHeader = (STOREHEADER*)m_pView->GetBasePointer();

  m_pBlockAllocationIndex0 = (BLOCKADDRESS*)GetBlock(m_pHeader->BlockAllocationIndex0);
  for (SIZE_T i=0; i<m_pHeader->BlockSize / sizeof(BLOCKADDRESS); i++)
  {
    if (m_pBlockAllocationIndex0[i] != 0)
      m_pBlockBitmaps[i] = (DWORD*)GetBlock(m_pBlockAllocationIndex0[i]);
    else
      m_pBlockBitmaps[i] = 0;
  }

  m_pHeader->FreeBlockCount += (BlockCount-m_pHeader->BlockCount);
  m_pHeader->BlockCount = BlockCount;

  for (SIZE_T i=0; i<m_Tables.Length(); i++)
    m_Tables[i]->Remap();

  for (SIZE_T i=0; i<m_RemappingPointers.Length(); i++)
    *m_RemappingPointers[i].First = (VOID*)((LPBYTE)m_pView->GetBasePointer() + m_RemappingPointers[i].Second);

}
//---------------------------------------------------------------------------------------------------
DataStore::BLOCKADDRESS DataStore::AllocateBlock(BOOL InitializeToZero)
{
  return AllocateBlock(InitializeToZero, TRUE, TRUE);
}
//---------------------------------------------------------------------------------------------------
DataStore::BLOCKADDRESS DataStore::AllocateBlock(BOOL InitializeToZero, BOOL IncreaseBlockSpace, BOOL IncreaseBitmapSpace)
{
  if (m_pHeader->FreeBlockCount == 0 && IncreaseBlockSpace)
    Resize(m_pHeader->BlockCount + m_pHeader->BlockCount / 8);        
  
  for (SIZE_T i=0; i<m_pHeader->BlockSize/sizeof(BLOCKADDRESS); i++)
  {
    if (m_pBlockAllocationIndex0[i+1] == 0 && IncreaseBitmapSpace)
    {
      m_pBlockAllocationIndex0[i+1] = AllocateBlock(TRUE, FALSE, FALSE);
      m_pBlockBitmaps[i+1] = (DWORD*)GetBlock(m_pBlockAllocationIndex0[i+1]);
    }

    DWORD Bit = 0;
    for (BLOCKADDRESS j=0; j<m_pHeader->BlockSize / sizeof(DWORD); j++)
    {
      if (BitScanForward(&Bit, ~m_pBlockBitmaps[i][j]))
      {
        m_pBlockBitmaps[i][j] = (m_pBlockBitmaps[i][j] | 1<<Bit);
        m_pHeader->FreeBlockCount--;
        BLOCKADDRESS BlockNumber = (BLOCKADDRESS)(i*m_pHeader->BlockSize*8 + j*sizeof(DWORD)*8 + Bit);
        
        if (InitializeToZero)
          ZeroMemory((LPBYTE)m_pView->GetBasePointer() + BlockNumber*m_pHeader->BlockSize, m_pHeader->BlockSize);
        
        return BlockNumber;
      }
    }
  }

  throw OutOfMemoryException(this, TEXT("Unable to Allocate a new block"));
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::FreeBlock(BLOCKADDRESS Block)
{
  BLOCKADDRESS i = Block / (m_pHeader->BlockSize*8);
  Block -= i;
  
  BLOCKADDRESS j = Block / 32;
  Block -= j;
  
  BLOCKADDRESS k = Block;

  m_pBlockBitmaps[i][j] &= ~(1<<k);
  m_pHeader->FreeBlockCount++;
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Lock(BLOCKADDRESS Block)
{
  m_pView->Lock(Block*m_pHeader->BlockSize, m_pHeader->BlockSize);
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::Unlock(BLOCKADDRESS Block)
{
  m_pView->Unlock(Block*m_pHeader->BlockSize, m_pHeader->BlockSize);
}
//---------------------------------------------------------------------------------------------------
Ptr<DataStore::Table> DataStore::OpenTable(Table::TABLEHEADER* pHeader)
{
  Table* pTable = new Table(this, pHeader);
  m_Tables.Add(pTable);
  return pTable;
}
//---------------------------------------------------------------------------------------------------
DataStore::DataStore() 
{
  m_pHeader = 0;
  m_pBlockBitmaps = 0;
  m_pBlockAllocationIndex0 = 0;
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::LoadMetaData()
{
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::SaveMetaData()
{
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::RegisterAutoRemapPointer(VOID** ppPointer)
{
  PTRDIFF_T Offset = (LPBYTE)(*ppPointer) - (LPBYTE)m_pView->GetBasePointer();
  if (Offset < 0)
    throw InvalidOperationException(this, "The pointer provided is not within the DataStore");
  if ((SIZE_T)Offset > m_pView->Length())
    throw InvalidOperationException(this, "The pointer provided is not within the DataStore");
    
  m_RemappingPointers.Add(Pair<VOID**,PTRDIFF_T>(ppPointer, Offset));
}
//---------------------------------------------------------------------------------------------------
VOID DataStore::RemoveAutoRemapPointer(VOID** ppPointer)
{
  for (SIZE_T i=0; i<m_RemappingPointers.Length(); i++)
  {
    if (m_RemappingPointers[i].First == ppPointer)
    {
      m_RemappingPointers.Erase(i);
      break;
    }
  }
}
//---------------------------------------------------------------------------------------------------
