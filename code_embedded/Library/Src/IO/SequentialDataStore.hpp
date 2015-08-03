/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/SequentialDataStore.hpp $
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

#include "IO/File.hpp"
#include "Memory/Memory.hpp"
#include "Memory/Md5.hpp"
#include "Com/Guid.hpp"
#include "Utility/Nullable.hpp"

namespace Bolitho
{
  namespace IO
  {
    class LIBRARY_API SequentialDataStoreException : public Exception
    {
      DeclareRuntimeType();
    public:
      SequentialDataStoreException(CONST Object* pSource, CONST Exception* pInnerException) : Exception(pSource, pInnerException)
      {}
    };

    /* Represents a data store that is only written to sequentially */
    template<class STOREHEADER=NULL_T, class BLOCKHEADER=NULL_T>
    class SequentialDataStore : public Object
    {
      DeclareRuntimeType();
    public:
      typedef BLOCKHEADER* LPBLOCKHEADER;
      typedef CONST BLOCKHEADER* LPCBLOCKHEADER;
      typedef STOREHEADER* LPSTOREHEADER;
      typedef CONST STOREHEADER* LPCSTOREHEADER;

      /* Format of the store header */
      struct STORE
      {
        ULONG FirstBlock;
        STOREHEADER Header;
      };

      /* The format of a block */
      struct BLOCK
      {
        Md5Hash Checksum;
        DWORD TypeId;
        ULONG Length;
        BLOCKHEADER Header;
      };

      /* Index information about a block */
      struct BlockInfo
      {
        ULONGLONG Address;
        ULONG Index;
        BLOCK Block;
      };

      /* Open an existing data store */
      static Ptr<SequentialDataStore> Open(LPCTSTR Filename);
      /* Create a new data store */
      static Ptr<SequentialDataStore> Create(LPCTSTR Filename, LPCSTOREHEADER pStoreInfo);
      
      /* Adds a block of data to the store */
      ULONG AddBlock(DWORD TypeId, LPCBLOCKHEADER Header, LPVOID pData, SIZE_T Length);

      /* Gets the number of blocks */
      SIZE_T GetBlockCount() CONST
      { return m_Index.Length(); }

      /* Gets the index information for a block */
      CONST BlockInfo& GetBlockInfo(ULONG i) CONST
      { return m_Index[i]; }

      /* Gets the data for a block */
      Buffer<BYTE> GetBlockData(ULONG i) CONST;

      /* Verifys the data for a block */
      BOOL VerifyBlockData(ULONG i) CONST;

      /* Returns the size of the data store on disk */
      ULONGLONG Length() CONST
      { return m_pFile->Length(); }

      /* Verifies all blocks in the data store */
      BOOL VerifyAllBlocks(Array<ULONG>& FailedBlocks) CONST;
      
      CONST STORE& GetStoreInfo() CONST
      { return m_StoreInfo; }

    private:
      /* Builds the index of blocks */
      VOID BuildIndex();

      /* The store header information */
      STORE m_StoreInfo;

      /* The file used to back the data store */
      mutable Ptr<File> m_pFile;

      /* The index of the blocks */
      Array<BlockInfo> m_Index;

      /* The address for the next block */
      ULONGLONG m_NextBlockAddress;

    };

    #include "SequentialDataStoreImpl.hpp"

  }
}