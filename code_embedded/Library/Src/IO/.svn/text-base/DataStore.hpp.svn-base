/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/DataStore.hpp $
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




#pragma once

#include "IO/File.hpp"
#include "IO/MemoryMappedFile.hpp"
#include "Collections/Map.hpp"
#include "Utility/Nullable.hpp"

namespace Bolitho
{
  namespace IO
  {
      class LIBRARY_API DataStoreException : public UserException
      {
        DeclareRuntimeType();
      public:
        DataStoreException( CONST Object* Sender, LPCTSTR Description ) : UserException(Sender)
        {
          m_Description = CharTraits<TCHAR>::Duplicate(Description);
        }

        DataStoreException( CONST DataStoreException& E ) : UserException(E)
        {
          m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
        }

        virtual ~DataStoreException()
        {
          if (m_Description)
            FreeMemory(m_Description);
        }

        virtual String Description() CONST
        {
          return m_Description;
        }

      private:
        TCHAR* m_Description;
      };

      /* A block-based data store */
      class LIBRARY_API DataStore : public Object
      {
      public:
        typedef UINT32 BLOCKADDRESS;

        DeclareRuntimeType();
      public:
        /* A table that presents a simple collection of blocks */
        class Table : public Object
        {
          friend class DataStore;
        public:
          class LIBRARY_API Stream : public IO::Stream
          {
          public:
            Stream(Ptr<Table> pTable);

            virtual VOID Flush();
            virtual VOID Close();

            virtual SIZE_T Write(LPCVOID pData, SIZE_T N);
            virtual SIZE_T Read(LPVOID pData, SIZE_T N);

            virtual BOOL EndOfStream();

          private:
            Ptr<Table> m_pTable;
            BLOCKADDRESS m_CurrentBlock;
            SIZE_T m_BlockSize;
            SIZE_T m_CurrentOffset;
          };

          struct TABLEHEADER
          {
            BLOCKADDRESS BlockCount;
            BLOCKADDRESS BlockIndex0;
          };

          Table(DataStore* pDataStore, TABLEHEADER* pHeader);

          /* Resizes the table */
          VOID Resize(BLOCKADDRESS BLOCKADDRESS);

          /* Gets a pointer to the data block */
          LPCVOID GetBlock(BLOCKADDRESS Address) CONST;
          LPVOID GetBlock(BLOCKADDRESS Address);

          /* Locks a block in the table */
          VOID Lock(BLOCKADDRESS Address);
          /* Unlocks a block in the table */
          VOID Unlock(BLOCKADDRESS Address);

          /* Translates a table address into a data store address */
          BLOCKADDRESS Translate(BLOCKADDRESS TableAddress) CONST;

          /* Initializes the Table after memory remppaing has occurred */
          VOID Remap();

          /* Returns the size of the table in blocks */
          BLOCKADDRESS GetBlockCount() CONST
          { return m_pHeader->BlockCount; }

        protected:
          /* Updates a directory mapping */
          VOID UpdateDirectory(BLOCKADDRESS TableAddress, BLOCKADDRESS PhysicalAddress) CONST;

          mutable BLOCKADDRESS m_TranslationLookup[2];
          BLOCKADDRESS* m_pBlockIndex0;
          BLOCKADDRESS** m_pBlockIndex1;
          TABLEHEADER* m_pHeader;
          PTRDIFF_T m_HeaderOffset;          
          DataStore* m_pDataStore;
        };
       
        struct STOREHEADER
        {
          DWORD DataStoreVersion;
          BLOCKADDRESS BlockAllocationIndex0;
          BLOCKADDRESS BlockCount;
          BLOCKADDRESS FreeBlockCount;
          BLOCKADDRESS BlockSize;
          Table::TABLEHEADER MetaDataTable;
          BYTE UserData[1024];
        };

				virtual ~DataStore();

        /* Create an in-memory datastore */
        static Ptr<DataStore> Create(BLOCKADDRESS BlockSize);
        /* Create a persistant datastore */
        static Ptr<DataStore> Create(LPCTSTR Filename, BLOCKADDRESS BlockSize);
        /* Open a persistant datastore */
        static Ptr<DataStore> Open(LPCTSTR Filename);

        /* Returns a pointer to the user-data section of the header block */
        template<class USERSTRUCT>
        USERSTRUCT* GetHeaderStruct()
        { return (USERSTRUCT*)&m_pHeader->UserData; }

        /* Returns a pointer to the user-data section of the header block */
        LPVOID GetHeaderData()
        { return &m_pHeader->UserData; }

        /* Flushes all dirty data to disk */
        VOID Flush();

        /* Adds a metadata value to the mesh */
        VOID SetMetaData(LPCTSTR Key, LPCTSTR Value);

        /* Gets a metadata value */
        String GetMetaData(LPCTSTR Key);

        /* Removes a metadata value */
        VOID RemoveMetaData(LPCTSTR Key);

        /* Removes all metadata */
        VOID RemoveAllMetaData();

        /* Resizes the block store.
         * If BLOCKADDRESS is less than the current size of the store, blocks will be freed if not already allocated.
         */
        VOID Resize(BLOCKADDRESS BLOCKADDRESS );
        /* Allocates a new block.  Increases the size of the store if needed */
        BLOCKADDRESS AllocateBlock(BOOL InitializeToZero = FALSE);
        /* Frees a block */
        VOID FreeBlock(BLOCKADDRESS Block );

        /* The total number of blocks in the data store */
        BLOCKADDRESS GetBlockCount() CONST
        { return m_pHeader->BlockCount; }

        /* The number of free blocks in the data store */
        BLOCKADDRESS GetFreeBlockCount() CONST
        { return m_pHeader->FreeBlockCount; }

        /* The size of a block in bytes */
        BLOCKADDRESS GetBlockSize() CONST
        { return m_pHeader->BlockSize; }

        /* Returns a pointer to the memory location corresponding to a given block */
        LPVOID GetBlock(BLOCKADDRESS Block)
        { return (LPBYTE)m_pView->GetBasePointer() + Block*m_pHeader->BlockSize; }
        LPCVOID GetBlock(BLOCKADDRESS Block) CONST
        { return (LPCBYTE)m_pView->GetBasePointer() + Block*m_pHeader->BlockSize; }

        /* Pins a block in memory */
        VOID Lock(BLOCKADDRESS Block);
        /* Unpins a block in memory */
        VOID Unlock(BLOCKADDRESS Block);

        /* Opens a data table */
        Ptr<Table> OpenTable(Table::TABLEHEADER* pHeader);

        /* Registers a user pointer to be automatically remapped */
        VOID RegisterAutoRemapPointer(VOID** ppPointer);
        VOID RemoveAutoRemapPointer(VOID** ppPointer);

      protected:
        /* The latest data store version number */
        static CONST DWORD DATASTOREVERSION = 0x0101;

        DataStore();

        /* A pointer to the user point that is updated whenever remapping occurs */
        Array<Pair<VOID**,PTRDIFF_T>> m_RemappingPointers;

        /* The Mesh file */
        Ptr<IO::MemoryMappedFile> m_pFile;
        Ptr<IO::MemoryMappedView> m_pView;

        /* Internal Allocation */
        BLOCKADDRESS AllocateBlock(BOOL InitializeToZero, BOOL IncreaseStoreSpace, BOOL IncreaseBitmapSpace);


        /* Header Information */
        STOREHEADER* m_pHeader;
        /* Block Allocation Bitmap */
        BLOCKADDRESS* m_pBlockAllocationIndex0;
        DWORD** m_pBlockBitmaps;

        /* Data Tables */
        Array<Ptr<Table>> m_Tables;

        /* A collection of key/value pairs to store custom metadata */
        Map<String,String> m_MetaData;

        VOID LoadMetaData();
        VOID SaveMetaData();
      };

      #include "DataStoreImpl.hpp"

    }
}
