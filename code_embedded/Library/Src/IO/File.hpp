/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/File.hpp $
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

#pragma once

#include "IO/IOException.hpp"
#include "Time/DateTime.hpp"

namespace Bolitho
{
  namespace IO
  {
    class LIBRARY_API FileException : public Exception
    {
      DeclareRuntimeType();
    public:
      FileException(CONST Object* pSource, CONST Exception* pInnerException ) : Exception(pSource, pInnerException)
      {}
    };

#if defined(PLATFORM_WINDOWS)
    struct FileAttribute
    { enum { NORMAL = FILE_ATTRIBUTE_NORMAL, DIRECTORY = FILE_ATTRIBUTE_DIRECTORY, ARCHIVE = FILE_ATTRIBUTE_ARCHIVE, READONLY = FILE_ATTRIBUTE_READONLY, SYSTEM = FILE_ATTRIBUTE_SYSTEM, HIDDEN = FILE_ATTRIBUTE_HIDDEN, COMPRESSED = FILE_ATTRIBUTE_COMPRESSED, OFFLINE = FILE_ATTRIBUTE_OFFLINE, SPARSE = FILE_ATTRIBUTE_SPARSE_FILE, ENCRYPTED = FILE_ATTRIBUTE_ENCRYPTED, TEMPORARY = FILE_ATTRIBUTE_TEMPORARY, REPARSEPOINT = FILE_ATTRIBUTE_REPARSE_POINT }; };
    
    #undef DELETE
    struct FileShare
    { enum { NONE = 0, READ = FILE_SHARE_READ, WRITE = FILE_SHARE_WRITE, READWRITE = READ | WRITE, DELETE = FILE_SHARE_DELETE }; };
    
    struct FileAccess
    { enum { READ = GENERIC_READ, WRITE = GENERIC_WRITE, READWRITE = GENERIC_READ | GENERIC_WRITE }; };
    
    struct FileFlags
    { enum { DELETEONCLOSE = FILE_FLAG_DELETE_ON_CLOSE, WRITETHROUGH = FILE_FLAG_WRITE_THROUGH, NOBUFFERING = FILE_FLAG_NO_BUFFERING, SEQUENTIAL = FILE_FLAG_SEQUENTIAL_SCAN, RANDOM = FILE_FLAG_RANDOM_ACCESS }; };
    
    struct FileMode
    { enum { CREATE = CREATE_ALWAYS, CREATENEW = CREATE_NEW, OPEN = OPEN_EXISTING, OPENORCREATE = OPEN_ALWAYS, TRUNCATE = TRUNCATE_EXISTING, APPEND = 6 }; };
#endif
    
#if defined(PLATFORM_LINUX)
    struct FileAttribute
    { enum { NORMAL = S_IFREG, DIRECTORY = S_IFDIR, ARCHIVE = 0, READONLY = 0, SYSTEM = 0, HIDDEN = 0, COMPRESSED = 0, OFFLINE = 0, SPARSE = 0, ENCRYPTED = 0, TEMPORARY = 0, REPARSEPOINT = 0 }; };
    
    struct FileShare 
    { enum { NONE = 0, READ = 0, WRITE = 0, READWRITE = READ | WRITE, DELETE = 0 }; };
    
    struct FileAccess
    { enum { READ = O_RDONLY, WRITE = O_WRONLY, READWRITE = O_RDWR }; };
    
    struct FileFlags
    { enum { DELETEONCLOSE = 0x8000000, WRITETHROUGH = O_DSYNC, NOBUFFERING = 0, SEQUENTIAL = 0, RANDOM = 0 }; };
    
    struct FileMode
    { enum { CREATE = O_CREAT, CREATENEW = O_CREAT | O_EXCL , OPEN = 0, OPENORCREATE = O_CREAT, TRUNCATE = O_TRUNC, APPEND = O_APPEND }; };
#endif

    struct LIBRARY_API FileInfo
    {
      FileInfo();
      FileInfo(LPCTSTR Filename);

      String Filename;
      String Directory;

      DateTime CreatedOn;
      DateTime ModifiedOn;
      DateTime AccessedOn;

      LONGLONG Length;
      DWORD Attributes;

      BOOL IsDirectory() CONST
      { return Attributes & FileAttribute::DIRECTORY; }
    };


    class File;

    /* Represents a memory-mapping of a file */
    class LIBRARY_API FileView : public Object
    {
      DeclareRuntimeType();

      friend class File;

    public:
      virtual ~FileView();

      /* The base address of the mapped region */
      LPVOID GetBaseAddress()
      { return m_pBaseAddress; }
      LPCVOID GetBaseAddress() CONST
      { return m_pBaseAddress; }

      /* The length of the mapped region */
      SIZE_T Length() CONST
      { return m_Length; }

      /* Lock/Unlock a region of the file into memory */
      VOID Lock(SIZE_T Offset, SIZE_T Length) CONST;
      VOID Unlock(SIZE_T Offset, SIZE_T Length) CONST;

      /* Flushes a porition of the view to disk */
      VOID Flush();
      VOID Flush(SIZE_T Offset, SIZE_T Length);

      /* Returns the page access used to map the view */
      DWORD GetPageAccess() CONST
      { return m_PageAccess; }
    private:
      FileView(Ptr<File> pFile, ULONGLONG Offset, SIZE_T Length, DWORD Access);
      
      LPVOID m_pBaseAddress;
      SIZE_T m_Length;
      DWORD m_PageAccess;

      MEMORYMAPPEDFILEHANDLE m_hMapping;
      Ptr<File> m_pFile;
    };

    /* Represents a file */
    class LIBRARY_API File : public SeekableStream
    {
      DeclareRuntimeType();
      
      friend class MemoryMappedFile;
      friend class MemoryMappedView;
    public:
      /* Creates a temprary file and returns an open File object representing it */
      static Ptr<File> OpenTemporary();
      
      /* Determines if a file exists */
      static BOOL Exists(LPCTSTR Filename);
      /* Determines if a file is a directory, or other type of file */
      static BOOL IsDirectory(LPCTSTR Filename);
      /* Deletes a file */
      static VOID Delete(LPCTSTR Filename);
      /* Copys a file */
      static VOID Copy(LPCTSTR Source, LPCTSTR Dest);
      static VOID Copy(Ptr<File> pSource, ULONGLONG SourceOffset, ULONGLONG Length, Ptr<File> pDestination, ULONGLONG DestOffset);
      /* Moves a file */
      static VOID Move(LPCTSTR Source, LPCTSTR Dest);
      /* Creates a soft/hard link to a file */
      static VOID Link(LPCTSTR Name, LPCTSTR Dest, BOOL HardLink = TRUE);
      
      /* Reads an array of N structs from a file */
      template<class T>
      static VOID ReadStruct( LPCTSTR Filename, ULONGLONG Offset, T* pStruct, SIZE_T N = 1 )
      {
	      Ptr<File> pFile = File::Open( Filename, FileMode::OPEN, FileAccess::READ );
        pFile->Seek(Offset);
        pFile->Read( pStruct, sizeof(T)*N );
      }

      /* Writes an array of N structs to a file */
      template<class T>
      static VOID WriteStruct( LPCTSTR Filename, ULONGLONG Offset, CONST T* pStruct, SIZE_T N = 1 )
      {
        Ptr<File> pFile = File::Open( Filename, FileMode::OPENORCREATE, FileAccess::WRITE );
        pFile->Seek(Offset);
        pFile->Write( pStruct, sizeof(T)*N );
      }

      /* Reads and returns all bytes in a file */
      static Buffer<BYTE> ReadAllBytes( LPCTSTR Filename );
      /* Reads and returns an array of text lines in a file */
      static Array<String> ReadAllLines( LPCTSTR Filename );
      /* Reads and returns all text in a file */
      static String ReadAllText( LPCTSTR Filename );

      /* Opens a file */
      static Ptr<File> Open( LPCTSTR Filename, DWORD Mode = FileMode::OPENORCREATE, DWORD Access = FileAccess::READWRITE, DWORD Share = FileShare::READWRITE, DWORD Flags = 0 );
      static Ptr<File> TryOpen( LPCTSTR Filename, DWORD Mode = FileMode::OPENORCREATE, DWORD Access = FileAccess::READWRITE, DWORD Share = FileShare::READWRITE, DWORD Flags = 0 );
      
      virtual ~File();
      
      /* Returns the date/time the file was last modified */
      DateTime LastModifiedOn() CONST;
      /* Returns the date/time the file was last accessed */
      DateTime LastAccessedOn() CONST;
      /* Returns the date/time the file was created */
      DateTime CreatedOn() CONST;
      
      /* Returns the length of the file */
      ULONGLONG Length();
      /* Resizes the file */
      ULONGLONG Resize( ULONGLONG NewLength );
      /* Returns the current file pointer location */
      virtual ULONGLONG Position();
      /* Sets the current file pointer */
      virtual ULONGLONG Seek(ULONGLONG NewPosition);

      /* Flushes all unwritten data to disk */
      virtual VOID Flush();
      /* Closes the file */
      virtual VOID Close();

      /* Writes data to the file at the current file pointer location */
      virtual SIZE_T Write(LPCVOID pData, SIZE_T N);
      /* Reads data from the file at the current file pointer location */
      virtual SIZE_T Read(LPVOID pData, SIZE_T N);

      /* Returns TRUE if the file pointer is at or beyond the end of the file */
      virtual BOOL EndOfStream();

      /* Sets the size of the internal read/write buffer */
      VOID SetBufferSize(SIZE_T BufferSize);

      /* Returns the OS handle for the file */
      FILEHANDLE Handle() CONST
      { return m_hFile; }

      /* Maps a view of the file into memory */
      Ptr<FileView> MapView(DWORD PageAccess);
      Ptr<FileView> MapView(ULONGLONG Offset, SIZE_T Length, DWORD PageAccess);

      /* Moves a large block of data within the file */
      VOID MoveBlock(ULONGLONG FromOffset, ULONGLONG ToOffset, ULONGLONG Length);

    private:
      File(FILEHANDLE hFile);

      String m_Filename;
      FILEHANDLE m_hFile;

      SIZE_T RawWrite( LPCVOID pData, SIZE_T N );
      SIZE_T RawRead( LPVOID pData, SIZE_T N );

      Buffer<BYTE> m_Buffer;
      SIZE_T m_BufferOffset;
      SIZE_T m_BufferLength;
      
      struct BufferMode { enum { READ, WRITE, NONE }; };
      DWORD m_BufferMode;
    };

  }
}
