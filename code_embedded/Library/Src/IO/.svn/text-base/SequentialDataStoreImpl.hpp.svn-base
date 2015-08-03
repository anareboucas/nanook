/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/SequentialDataStoreImpl.hpp $
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





//---------------------------------------------------------------------------------------------------
ImplementTemplate2RuntimeType(Bolitho::IO,SequentialDataStore,Object);
//---------------------------------------------------------------------------------------------------
template<class STOREHEADER, class BLOCKHEADER>
Ptr<SequentialDataStore<STOREHEADER,BLOCKHEADER>> SequentialDataStore<STOREHEADER,BLOCKHEADER>::Open(LPCTSTR Filename)
{
  SequentialDataStore<STOREHEADER,BLOCKHEADER>* pStore = new SequentialDataStore<STOREHEADER,BLOCKHEADER>();
  pStore->m_pFile = File::Open(Filename, FileMode::OPEN, FileAccess::READWRITE);
  pStore->m_pFile->Read(&pStore->m_StoreInfo, sizeof(STORE));

  pStore->BuildIndex();

  return pStore;
}
//---------------------------------------------------------------------------------------------------
template<class STOREHEADER, class BLOCKHEADER>
Ptr<SequentialDataStore<STOREHEADER,BLOCKHEADER>> SequentialDataStore<STOREHEADER,BLOCKHEADER>::Create(LPCTSTR Filename, LPCSTOREHEADER pStoreInfo)
{
  SequentialDataStore<STOREHEADER,BLOCKHEADER>* pStore = new SequentialDataStore<STOREHEADER,BLOCKHEADER>();
  pStore->m_pFile = File::Open(Filename, FileMode::CREATE, FileAccess::READWRITE);
  
  pStore->m_StoreInfo.FirstBlock = sizeof(STORE);
  if (pStoreInfo != 0)
    CopyMemory(&pStore->m_StoreInfo.Header, pStoreInfo, sizeof(STORE));
  
  pStore->m_pFile->Write(&pStore->m_StoreInfo, sizeof(STORE));  
  pStore->m_NextBlockAddress = sizeof(STORE);
  
  return pStore;
}
//---------------------------------------------------------------------------------------------------
template<class STOREHEADER, class BLOCKHEADER>
VOID SequentialDataStore<STOREHEADER,BLOCKHEADER>::BuildIndex()
{
  m_Index.Resize(0);

  SIZE_T i = 0;
  ULONGLONG Address = m_StoreInfo.FirstBlock;

  BLOCK Block;
  do
  {
    m_pFile->Seek(Address);
    m_pFile->Read(&Block, sizeof(Block));
    
    BlockInfo Info;
    Info.Address = Address;
    Info.Index = i++;
    Info.Block = Block;
    
    m_Index.Add(Info);

    Address += sizeof(Block) + Block.Length;
  }
  while(Address < m_pFile->Length());

  m_NextBlockAddress = Address;
}
//---------------------------------------------------------------------------------------------------
template<class STOREHEADER, class BLOCKHEADER>
ULONG SequentialDataStore<STOREHEADER,BLOCKHEADER>::AddBlock(DWORD TypeId, LPCBLOCKHEADER pHeader, LPVOID pData, SIZE_T Length)
{
  BLOCK Block;
  ZeroMemory(&Block, sizeof(Block));

  Block.Length = Length;
  Block.TypeId = TypeId;
  if (pHeader)
    CopyMemory(&Block.Header, pHeader, sizeof(BLOCKHEADER));

  Md5 H;
  H.Update(&Block.TypeId, sizeof(Block) - sizeof(Block.Checksum));
  H.Update(pData, Length);

  Block.Checksum = H.Value();

  m_pFile->Seek(m_NextBlockAddress);
  m_pFile->Write(&Block, sizeof(Block));
  m_pFile->Write(pData, Length);
    
  BlockInfo Info;
  Info.Address = m_NextBlockAddress;
  Info.Index = m_Index.Length();
  Info.Block = Block;

  m_Index.Add(Info);

  m_NextBlockAddress += sizeof(Block) + Length;

  return Info.Index;
}
//---------------------------------------------------------------------------------------------------
template<class STOREHEADER, class BLOCKHEADER>
Buffer<BYTE> SequentialDataStore<STOREHEADER,BLOCKHEADER>::GetBlockData(ULONG i) CONST
{
  BlockInfo Info = GetBlockInfo(i);

  m_pFile->Seek(Info.Address);
  m_pFile->Read(&Info.Block, sizeof(BLOCK));
  
  Buffer<BYTE> Data;
  Data.Resize(Info.Block.Length);

  m_pFile->Read(Data, Data.Length());

  Md5 H;
  H.Update(&Info.Block.TypeId, sizeof(Info.Block) - sizeof(Info.Block.Checksum));
  H.Update(Data, Data.Length());

  if (H.Value() != Info.Block.Checksum)
    throw SequentialDataStoreException(this, new InvalidOperationException(this, "The block checksum failed"));

  return Data;
}
//---------------------------------------------------------------------------------------------------
template<class STOREHEADER, class BLOCKHEADER>
BOOL SequentialDataStore<STOREHEADER,BLOCKHEADER>::VerifyBlockData(ULONG i) CONST
{
  BlockInfo Info = GetBlockInfo(i);

  m_pFile->Seek(Info.Address);
  m_pFile->Read(&Info.Block, sizeof(BLOCK));
  
  Buffer<BYTE> Data;
  Data.Resize(Info.Block.Length);

  m_pFile->Read(Data, Data.Length());

  Md5 H;
  H.Update(&Info.Block.TypeId, sizeof(Info.Block) - sizeof(Info.Block.Checksum));
  H.Update(Data, Data.Length());

  return (H.Value() == Info.Block.Checksum);
}
//---------------------------------------------------------------------------------------------------
template<class STOREHEADER, class BLOCKHEADER>
BOOL SequentialDataStore<STOREHEADER,BLOCKHEADER>::VerifyAllBlocks(Array<ULONG>& FailedBlocks) CONST
{
  FailedBlocks.Resize(0);

  for (SIZE_T i=0; i<m_Index.Length(); i++)
    if (!VerifyBlockData(i))
      FailedBlocks.Add(i);

  return FailedBlocks.Length() == 0;
}
//---------------------------------------------------------------------------------------------------
