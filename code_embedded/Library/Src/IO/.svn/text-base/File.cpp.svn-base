/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/File.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1319 $
 * Last Updated: $Date: 2007-12-03 12:57:07 -0500 (Mon, 03 Dec 2007) $
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
#include "IO/TextStream.hpp"
#include "Utility/Flags.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//------------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,FileException,Exception);
ImplementRuntimeType(Bolitho::IO,File,Stream);
//------------------------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
FileInfo::FileInfo()
{
  Length = 0;
  Attributes = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FileInfo::FileInfo(LPCTSTR F)
{
  String FullFilename = Path::FullPath(F);

  WIN32_FIND_DATA FindData;
	ZeroMemory(&FindData, sizeof(FindData));

  HANDLE hFind = FindFirstFile(FullFilename, &FindData);
  if (hFind == INVALID_HANDLE_VALUE)
    SystemException::ThrowError();
  
  Directory = Path::Directory(FullFilename);
	Filename = FindData.cFileName;
	
	AccessedOn = DateTime(FindData.ftLastAccessTime);
	CreatedOn = DateTime(FindData.ftCreationTime);
	ModifiedOn = DateTime(FindData.ftLastWriteTime);

	Attributes = FindData.dwFileAttributes;
	Length = MAKEULONGLONG(FindData.nFileSizeLow,FindData.nFileSizeHigh);

  FindClose(hFind);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<File> File::OpenTemporary()
{
  TCHAR TempName[MAX_PATH + 1];
  if (GetTempFileName(TEXT("."), "MGB", 0, TempName))
    SystemException::ThrowError();
  return File::Open(TempName, FileMode::OPEN, FileAccess::READWRITE, FileShare::NONE, FileAttribute::TEMPORARY | FileFlags::DELETEONCLOSE);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL File::Exists(LPCTSTR Filename)
{
  WIN32_FIND_DATA FindData;
  HANDLE hFind = FindFirstFile(Filename, &FindData);
  if (hFind != INVALID_HANDLE_VALUE)
  {
    FindClose(hFind);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL File::IsDirectory(LPCTSTR Filename)
{
  WIN32_FIND_DATA FindData;
  HANDLE hFind = FindFirstFile(Filename, &FindData);
  if (hFind != INVALID_HANDLE_VALUE)
  {
    FindClose(hFind);
    return (FindData.dwFileAttributes & FileAttribute::DIRECTORY) != 0;
  }
  return FALSE;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Delete(LPCTSTR Filename)
{
  DeleteFile(Filename);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Copy(LPCTSTR Source, LPCTSTR Dest)
{
  CopyFile(Source, Dest, TRUE);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Copy(Ptr<File> pSource, ULONGLONG SourceOffset, ULONGLONG Length, Ptr<File> pDestination, ULONGLONG DestOffset)
{
  CONST ULONGLONG BLOCKSIZE = 1048576;
  BYTE* pBlock = new BYTE[BLOCKSIZE];

  pSource->Seek(SourceOffset);
  pDestination->Seek(DestOffset);

  ULONGLONG CopyLength = 0;
  while (CopyLength < Length)
  {
    ULONGLONG ThisBlock = Min(Length-CopyLength, BLOCKSIZE);
    pSource->Read(pBlock, ThisBlock);
    pDestination->Write(pBlock, ThisBlock);
    CopyLength += ThisBlock;
  }
  
  delete[] pBlock;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Move(LPCTSTR Source, LPCTSTR Dest)
{
  MoveFile(Source, Dest);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Link(LPCTSTR Name, LPCTSTR Dest, BOOL HardLink)
{
  if (HardLink)
  {
    if (!CreateHardLink(Name, Dest, NULL))
      SystemException::ThrowError();
  }
#if PLATFORM_WINDOWS >= PLATFORM_WINDOWS_VISTA
  else
  {
    if (!CreateSymbolicLink(Name, Dest, IsDirectory(Name) ? SYMBOLIC_LINK_FLAG_DIRECTORY : 0))
      SystemException::ThrowError();
  }
#endif

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<File> File::Open(LPCTSTR Filename, DWORD CreateDisposition, DWORD FileAccess, DWORD FileShare, DWORD Flags)
{
  BOOL Append = FALSE;
  if (CreateDisposition == FileMode::APPEND)
  {
    Append = TRUE;
    CreateDisposition = FileMode::OPENORCREATE;
  }

	HANDLE hFile = CreateFile(Filename, FileAccess, FileShare, NULL, CreateDisposition, Flags, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
    SystemException::ThrowError();
	File* pFile = new File(hFile);
	
  if (Append)
    pFile->Seek(pFile->Length());
  
  return pFile;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<File> File::TryOpen(LPCTSTR Filename, DWORD CreateDisposition, DWORD FileAccess, DWORD FileShare, DWORD Flags)
{
  BOOL Append = FALSE;
  if (CreateDisposition == FileMode::APPEND)
  {
    Append = TRUE;
    CreateDisposition = FileMode::OPENORCREATE;
  }

	HANDLE hFile = CreateFile(Filename, FileAccess, FileShare, NULL, CreateDisposition, Flags, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
    return Ptr<File>::Null;
	File* pFile = new File(hFile);
	
  if (Append)
    pFile->Seek(pFile->Length());
  
  return pFile;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
DateTime File::LastModifiedOn() CONST
{
  FILETIME Modified;
  GetFileTime(m_hFile, NULL, NULL, &Modified);
  return DateTime(Modified);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
DateTime File::LastAccessedOn() CONST
{
  FILETIME Accessed;
  GetFileTime(m_hFile, NULL, &Accessed, NULL);
  return DateTime(Accessed);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
DateTime File::CreatedOn() CONST
{
  FILETIME Created;
  GetFileTime(m_hFile, &Created, NULL, NULL);
  return DateTime(Created);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG File::Length()
{
  DWORD Type = ::GetFileType(m_hFile);
  if (Type == FILE_TYPE_CHAR)
    return 0;

  if (m_BufferMode == BufferMode::WRITE)
  {
    RawWrite(&m_Buffer[0], m_BufferLength);
    m_BufferMode = BufferMode::READ;
    m_BufferLength = 0;
    m_BufferOffset = 0;
  }

  LARGE_INTEGER Sz;
  
  if (!GetFileSizeEx(m_hFile, &Sz))
    throw FileException(this, SystemException::GetError(this));

	return (ULONGLONG)Sz.QuadPart;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG File::Position()
{
  DWORD Type = ::GetFileType(m_hFile);
  if (Type == FILE_TYPE_CHAR)
    return 0;

	ULONGLONG Pos = 0;
	LARGE_INTEGER Z;
	Z.QuadPart = 0;
	if (!SetFilePointerEx(m_hFile, Z, (LARGE_INTEGER*)&Pos, FILE_CURRENT))
    throw FileException(this, SystemException::GetError(this));
  if (m_BufferLength > 0)
    Pos = Pos - m_BufferLength + m_BufferOffset;
	return Pos;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG File::Seek(ULONGLONG NewPosition)
{
  DWORD Type = ::GetFileType(m_hFile);
  if (Type == FILE_TYPE_CHAR)
    return 0;

  if (m_BufferMode == BufferMode::WRITE)
  {
    RawWrite(&m_Buffer[0], m_BufferLength);
    m_BufferMode = BufferMode::READ;
    m_BufferLength = 0;
    m_BufferOffset = 0;
  }

  m_BufferLength = 0;
  m_BufferMode = BufferMode::NONE;
  m_BufferOffset = 0;

  ULONGLONG Pos = 0;
	LARGE_INTEGER Z;
	Z.QuadPart = NewPosition;
	if (!SetFilePointerEx(m_hFile, Z, (LARGE_INTEGER*)&Pos, FILE_BEGIN))
    throw FileException(this, SystemException::GetError(this));
	return Pos;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Flush()
{
  if (m_BufferMode == BufferMode::WRITE)
  {
    RawWrite(&m_Buffer[0], m_BufferLength);
    m_BufferOffset = 0;
    m_BufferLength = 0;
    m_BufferMode = BufferMode::NONE;
  }
  
  FlushFileBuffers(m_hFile);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Close()
{
  if (m_BufferMode == BufferMode::WRITE)
  {
    RawWrite(&m_Buffer[0], m_BufferLength);
    m_BufferOffset = 0;
    m_BufferLength = 0;
    m_BufferMode = BufferMode::NONE;
  }

  if (m_hFile != 0)
  {
    CloseHandle(m_hFile);
    m_hFile = 0;
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL File::EndOfStream()
{
  DWORD Type = GetFileType(m_hFile);
  if (Type == FILE_TYPE_CHAR)
    return false;
  return Position() >= Length();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG File::Resize(ULONGLONG NewLength)
{
  Seek(NewLength);
  SetEndOfFile(m_hFile);
  return NewLength;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T File::RawWrite(LPCVOID pData, SIZE_T N)
{
  DWORD WriteBytes = 0;
  if (!WriteFile(m_hFile, pData, (DWORD)N, &WriteBytes, 0))
    throw FileException(this, SystemException::GetError(this));

  return WriteBytes;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T File::RawRead(LPVOID pData, SIZE_T N)
{
  DWORD ReadBytes = 0;
  if (!ReadFile(m_hFile, pData, (DWORD)N, &ReadBytes, 0))
    throw FileException(this, SystemException::GetError(this));

  return ReadBytes;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<FileView> File::MapView(ULONGLONG Offset, SIZE_T Length, DWORD PageAccess)
{
  return new FileView(this, Offset, Length, PageAccess);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<FileView> File::MapView(DWORD PageAccess)
{
  return new FileView(this, 0, 0, PageAccess);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Memory/VirtualMemory.hpp"
//------------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO, FileView, Object);
//------------------------------------------------------------------------------------------------------------------------------------------------------
FileView::FileView(Ptr<File> pFile, ULONGLONG Offset, SIZE_T Length, DWORD Access)
{
  m_hMapping = 0;
  m_pBaseAddress = 0;
  
  if (pFile->Length() > 0)
  {
    DWORD MaximumSizeHigh = (DWORD)(pFile->Length() >> 32);
    DWORD MaximumSizeLow = (DWORD)(pFile->Length() & 0xffffffff);

    m_hMapping = CreateFileMapping(pFile->Handle(), NULL, Access, MaximumSizeHigh, MaximumSizeLow, NULL);
    if (m_hMapping == 0)
      throw FileException(this, SystemException::GetError(this));

    DWORD DesiredAccess = 0;
    if (Access == PageAccess::READ)
      DesiredAccess = FILE_MAP_READ;
    else if (Access == PageAccess::READWRITE)
      DesiredAccess = FILE_MAP_WRITE;
    else if (Access == PageAccess::WRITECOPY)
      DesiredAccess = FILE_MAP_COPY;
    else
      throw InvalidArgumentException(this, "The parameter \'Access\' must be PageAccess::READ, PageAccess::READWRITE or PageAccess::WRITECOPY");

    m_pBaseAddress = MapViewOfFile(m_hMapping, DesiredAccess, (DWORD)(Offset >> 32), (DWORD)(Offset & 0xffffffff), Length);
    if (m_pBaseAddress == 0)
      throw FileException(this, SystemException::GetError(this));

    if (Length == 0)
      Length = (SIZE_T)pFile->Length();
  }

  m_Length = Length;
  m_pFile = pFile;
  m_PageAccess = Access;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FileView::~FileView()
{
  if (m_pBaseAddress)
    UnmapViewOfFile(m_pBaseAddress);
  m_pBaseAddress = 0;

  if (m_hMapping)
    CloseHandle(m_hMapping);
  m_hMapping = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileView::Lock(SIZE_T Offset, SIZE_T Length) CONST
{
  if (!VirtualLock((LPBYTE)m_pBaseAddress + Offset,Length))
    SystemException::ThrowError(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileView::Unlock(SIZE_T Offset, SIZE_T Length) CONST
{
  VirtualUnlock((LPBYTE)m_pBaseAddress + Offset, Length);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
#endif

//------------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_LINUX)
Ptr<File> File::OpenTemporary()
{
  CHAR TmpName[TMP_MAX];
  CopyMemory(&TmpName[0], "XXXXXX", 7);
  FILEHANDLE hFile = mkstemp64(&TmpName[0]);
  if (hFile == -1)
    throw FileException(0, SystemException::GetError(0));
  
  unlink(TmpName);
  return new File(hFile);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL File::Exists(LPCTSTR Filename)
{
  struct stat S;
  ZeroMemory(&S, sizeof(S));
  return stat(Filename, &S) == 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Delete(LPCTSTR Filename)
{
  if (unlink(Filename))
    throw FileException(0, SystemException::GetError(0));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Copy(LPCTSTR Source, LPCTSTR Dest)
{
  FILEHANDLE hSource = open64(Source, O_RDONLY);
  FILEHANDLE hDest = open64(Dest, O_WRONLY);

  if (hSource == -1)
    throw FileException(0, SystemException::GetError(0));
  if (hDest == -1)
    throw FileException(0, SystemException::GetError(0));
  
  struct stat S;
  ZeroMemory(&S, sizeof(S));
  if (stat(Source, &S))
    throw FileException(0, SystemException::GetError(0));
  
  if (sendfile(hDest, hSource, 0, S.st_size))
    throw FileException(0, SystemException::GetError(0));
  
  close(hDest);
  close(hSource);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Move(LPCTSTR Source, LPCTSTR Dest)
{
  if (rename(Source, Dest))
    throw FileException(0, SystemException::GetError(0));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<File> File::Open(LPCTSTR Filename, DWORD CreateDisposition, DWORD FileAccess, DWORD FileShare, DWORD Flags)
{
  FILEHANDLE hFile = open64(Filename, CreateDisposition | FileAccess | FileShare | Flags, S_IRUSR | S_IWUSR);
  if (hFile == -1)
    throw FileException(0, SystemException::GetError(0));
  
  if (IsFlagSet(Flags, FileFlags::DELETEONCLOSE))
    unlink(Filename);
  
  return new File(hFile);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
DateTime File::LastModifiedOn() CONST
{
  struct stat S;
  ZeroMemory(&S, sizeof(S));
  if (fstat(m_hFile, &S))
    throw FileException(this, SystemException::GetError(this));
  
  return DateTime(S.st_mtime);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
DateTime File::LastAccessedOn() CONST
{
  struct stat S;
  ZeroMemory(&S, sizeof(S));
  if (fstat(m_hFile, &S))
    throw FileException(this, SystemException::GetError(this));
  
  return DateTime(S.st_atime);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
DateTime File::CreatedOn() CONST
{
  struct stat S;
  ZeroMemory(&S, sizeof(S));
  if (fstat(m_hFile, &S))
    throw FileException(this, SystemException::GetError(this));
  
  return DateTime(S.st_ctime);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG File::Length()
{
  if (m_BufferMode == BufferMode::WRITE)
  {
    RawWrite(&m_Buffer[0], m_BufferLength);
    m_BufferMode = BufferMode::READ;
    m_BufferLength = 0;
    m_BufferOffset = 0;
  }

  struct stat64 S;
  ZeroMemory(&S, sizeof(S));
  if (fstat64(m_hFile, &S))
    throw FileException(this, SystemException::GetError(this));
  
  return S.st_size;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG File::Position()
{
  return lseek64(m_hFile, 0, SEEK_CUR);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG File::Seek(ULONGLONG NewPosition)
{
  if (m_BufferMode == BufferMode::WRITE)
  {
    RawWrite(&m_Buffer[0], m_BufferLength);
    m_BufferMode = BufferMode::READ;
    m_BufferLength = 0;
    m_BufferOffset = 0;
  }

  return lseek64(m_hFile, NewPosition, SEEK_SET);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Flush()
{
  if (m_BufferMode == BufferMode::WRITE)
    {
      RawWrite(&m_Buffer[0], m_BufferLength);
      m_BufferOffset = 0;
      m_BufferLength = 0;
      m_BufferMode = BufferMode::NONE;
    }
  
  if (fsync(m_hFile))
    throw FileException(this, SystemException::GetError(this));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::Close()
{
  if (m_hFile != 0)
    {
      Flush();
      close(m_hFile);
      m_hFile = 0;
    }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL File::EndOfStream()
{
  return Position() >= Length();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
ULONGLONG File::Resize(ULONGLONG NewLength)
{
  if (ftruncate64(m_hFile, NewLength))
    throw FileException(this, SystemException::GetError(this));
  return NewLength;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T File::RawWrite(LPCVOID pData, SIZE_T N)
{
  return write(m_hFile, pData, N);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T File::RawRead(LPVOID pData, SIZE_T N)
{
  return read(m_hFile, pData, N);
}

#include "Memory/VirtualMemory.hpp"
//------------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO, FileView, Object);
//------------------------------------------------------------------------------------------------------------------------------------------------------
FileView::FileView(Ptr<File> pFile, ULONGLONG Offset, SIZE_T Length, DWORD Access)
{
  DWORD Flags = MAP_SHARED;
  FILEHANDLE File = -1;
  
  if (Access & PageAccess::WRITECOPY)
    Flags = MAP_PRIVATE;
  
  if (pFile == 0)
    Flags |= MAP_ANONYMOUS;
  else
    File = pFile->Handle();
  
  if (Length == 0)
    Length = (SIZE_T)pFile->Length();

  m_pBaseAddress = mmap64(0, Length, Access, Flags, File, Offset);
  
  if (m_pBaseAddress == MAP_FAILED)
    SystemException::ThrowError(this);

  m_Length = Length;
  m_pFile = pFile;
  m_PageAccess = Access;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
FileView::~FileView()
{
  if (m_pBaseAddress)
    munmap(m_pBaseAddress, m_Length);
  m_pBaseAddress = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileView::Lock(SIZE_T Offset, SIZE_T Length) CONST
{
  if (mlock((LPBYTE)m_pBaseAddress + Offset, Length))
    SystemException::ThrowError(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID FileView::Unlock(SIZE_T Offset, SIZE_T Length) CONST
{
  munlock((LPBYTE)m_pBaseAddress + Offset, Length);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<FileView> File::MapView(ULONGLONG Offset, SIZE_T Length, DWORD PageAccess)
{
  return new FileView(this, Offset, Length, PageAccess);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<FileView> File::MapView(DWORD PageAccess)
{
  return new FileView(this, 0, 0, PageAccess);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
#endif


//------------------------------------------------------------------------------------------------------------------------------------------------------
Buffer<BYTE> File::ReadAllBytes(LPCTSTR Filename)
{
  CONST SIZE_T ChunkSize = 65536;
  Buffer<BYTE> Buf = Buffer<BYTE>(ChunkSize);

  Ptr<File> F = File::Open(Filename, FileMode::OPEN, FileAccess::READ);

  SIZE_T ReadSize = 0;
  SIZE_T CurrentPosition = 0;

  while ((ReadSize = F->Read(&Buf[CurrentPosition], ChunkSize)) > 0)
  {
    CurrentPosition += ReadSize;
    Buf.Resize(CurrentPosition + ChunkSize);
  }
  Buf.Resize(CurrentPosition);

  return Buf;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
Array<String> File::ReadAllLines(LPCTSTR Filename)
{
  Array<String> Lines;
  Ptr<File> F = File::Open(Filename, FileMode::OPEN, FileAccess::READ);
  TextStream* pTS = new TextStream(F);
  
  while (!pTS->EndOfStream())
    Lines.Add(pTS->ReadLine());

  delete pTS;

  return Lines;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
String File::ReadAllText(LPCTSTR Filename)
{
  CONST SIZE_T ChunkSize = 65536;
  Buffer<TCHAR> Buf = Buffer<TCHAR>(ChunkSize);

  Ptr<File> F = File::Open(Filename, FileMode::OPEN, FileAccess::READ);

  SIZE_T ReadSize = 0;
  SIZE_T CurrentPosition = 0;

  while ((ReadSize = F->Read(&Buf[CurrentPosition], ChunkSize*sizeof(TCHAR))) > 0)
  {
    CurrentPosition += ReadSize;
    Buf.Resize(CurrentPosition + ChunkSize);
  }
  Buf.Resize(CurrentPosition + 1);
  Buf[CurrentPosition] = 0;

  return String(Buf);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
File::File(FILEHANDLE hFile)
{
  m_hFile = hFile;
  m_BufferLength = 0;
  m_BufferMode = BufferMode::NONE;
	m_BufferOffset = 0;

  SetBufferSize(65536);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
File::~File()
{
  Close();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T File::Write(LPCVOID pData, SIZE_T N)
{
  if (N == 0)
    return 0;

  SIZE_T Sz = 0;
  
  if (m_BufferMode != BufferMode::WRITE)
  {
    m_BufferMode = BufferMode::WRITE;
    m_BufferLength = 0;
    m_BufferOffset = 0;
  }
  
  // If the request is larger than the buffer, do not buffer
  if (m_Buffer.Length() - m_BufferOffset < N)
  {
    if (m_BufferLength > 0)
    {
      RawWrite(&m_Buffer[0], m_BufferLength);
      m_BufferOffset = 0;
      m_BufferLength = 0;
    }

    if (N > m_Buffer.Length())
    {
      RawWrite(pData, N);
      m_BufferMode = BufferMode::NONE;
    }
    else
    {
      CopyMemory(&m_Buffer[m_BufferOffset], (LPCBYTE)pData, N);
      m_BufferOffset += N;
      m_BufferLength += N;
      Sz = N;
    }
  }
  // Otherwise, if buffer is in write mode, and the request can be written entirely to the buffer
  else if (m_BufferMode == BufferMode::WRITE && m_Buffer.Length() - m_BufferOffset >= N)
  {
    CopyMemory(&m_Buffer[m_BufferOffset], (LPCBYTE)pData, N);
    m_BufferOffset += N;
    m_BufferLength += N;
    Sz = N;
  }

	return Sz;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T File::Read(LPVOID pData, SIZE_T N)
{
  if (N == 0)
    return 0;
	
  SIZE_T Sz = 0;

  // If the Buffer has unwritten data, flush it.
  if (m_BufferMode == BufferMode::WRITE)
  {
    RawWrite(&m_Buffer[0], m_BufferLength);
    m_BufferMode = BufferMode::READ;
    m_BufferLength = 0;
    m_BufferOffset = 0;
  }

  // If request is larger than buffer, do not buffer.
  if (N > m_Buffer.Length())
  {
    Sz = RawRead(pData, N);
    m_BufferMode = BufferMode::NONE;
    m_BufferOffset = 0;
  }
  // Otherwise, if buffer is in read mode, and the request can be satisfied entirely from the buffer
  else if (m_BufferMode == BufferMode::READ && m_BufferLength - m_BufferOffset >= N)
  {
    CopyMemory((LPBYTE)pData, &m_Buffer[m_BufferOffset], N);
    m_BufferOffset += N;
    Sz = N;
  }
  // Otherwise, if buffer is in read mode, and the request can be satisfied partially from what is in the buffer.
  else if (m_BufferMode == BufferMode::READ && m_BufferLength - m_BufferOffset < N && m_BufferLength - m_BufferOffset > 0)
  {
    SIZE_T L = m_BufferLength - m_BufferOffset;
    CopyMemory((LPBYTE)pData, &m_Buffer[m_BufferOffset], L);
    
    m_BufferLength = RawRead(&m_Buffer[0], m_Buffer.Length());;

    L = N - L;
    if (L > m_BufferLength)
      L = m_BufferLength;
    
    CopyMemory((LPBYTE)pData + (N - L), &m_Buffer[0], L);
    m_BufferOffset = L;
    Sz = N;
  }
  // Otherwise, the request could be satisfied, but not from what is in the buffer
  else
  {
    m_BufferLength = RawRead(&m_Buffer[0], m_Buffer.Length());
    m_BufferMode = BufferMode::READ;

    if (N > m_BufferLength)
      N = m_BufferLength;
    CopyMemory((LPBYTE)pData, &m_Buffer[0], N);
    Sz = N;

    m_BufferOffset = N;
  }

  return Sz;
}
//------------------------------------------------------------------------------------------------------------------------------
VOID File::SetBufferSize(SIZE_T BufferSize)
{
  Flush();

  if (BufferSize == 0)
    m_Buffer.Free();
  else
    m_Buffer.Resize(BufferSize);
  
  m_BufferMode = BufferMode::NONE;
  m_BufferLength = 0;
  m_BufferOffset = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
VOID File::MoveBlock(ULONGLONG FromOffset, ULONGLONG ToOffset, ULONGLONG Length)
{
  CONST ULONGLONG BLOCKSIZE = 1048576; 
  LPBYTE pBuffer = new BYTE[BLOCKSIZE];

  if (FromOffset == ToOffset)
    return;

  if (m_BufferMode == BufferMode::WRITE)
  {
    RawWrite(&m_Buffer[0], m_BufferLength);
    m_BufferMode = BufferMode::READ;
    m_BufferLength = 0;
    m_BufferOffset = 0;
  }


  if (FromOffset < ToOffset)
  {
    ULONGLONG Offset = 0;
    while (Offset < Length)
    {
      ULONGLONG ThisBlock = Min(BLOCKSIZE, Length-Offset);
      Offset += ThisBlock;

      Seek(FromOffset + Length - Offset);
      RawRead(pBuffer, ThisBlock);
      Seek(ToOffset + Length - Offset);
      RawWrite(pBuffer, ThisBlock);
    }
  }
  else if (FromOffset > ToOffset)
  {
    ULONGLONG Offset = 0;
    while (Offset < Length)
    {
      ULONGLONG ThisBlock = Min(BLOCKSIZE, Length-Offset);
      Seek(FromOffset + Offset);
      RawRead(pBuffer, ThisBlock);
      Seek(ToOffset + Offset);
      RawWrite(pBuffer, ThisBlock);

      Offset += ThisBlock;
    }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

