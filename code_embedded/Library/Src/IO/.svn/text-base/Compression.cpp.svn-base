/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/Compression.cpp $
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

#include "IO/BZip2Stream.hpp"
#include "IO/GZipStream.hpp"
#include "IO/Compression.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,DecompressionStream,Stream);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DecompressionStream::DecompressionStream( DWORD Type, Ptr<Stream> pStream, BOOL AutoClose )
{
  switch(Type)
  {
	case CompressionType::NONE:
    m_pImpl = pStream;
    break;
	case CompressionType::GZIP:
#if defined(USE_ZLIB)
    m_pImpl = new GZipDecompressionStream( pStream, AutoClose );
#else
		throw InvalidOperationException(this, TEXT("The CodeLibrary was not configured with USE_ZLIB.  COMPRESSION_GZIP support is unavailable") );
#endif
    break;
	case CompressionType::BZIP2:
#if defined(USE_BZIP2)
		m_pImpl = new BZip2DecompressionStream( pStream, AutoClose );
#else
		throw InvalidOperationException(this, TEXT("The CodeLibrary was not configured with USE_BZIP2.  COMPRESSION_BZIP2 support is unavailable") );
#endif

    break;
  };
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DecompressionStream::~DecompressionStream()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DecompressionStream::Flush()
{
  m_pImpl->Flush();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID DecompressionStream::Close()
{
  m_pImpl->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T DecompressionStream::Write( LPCVOID pData, SIZE_T N )
{
  return m_pImpl->Write( pData, N );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T DecompressionStream::Read( LPVOID pData, SIZE_T N )
{
  return m_pImpl->Read( pData, N );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL DecompressionStream::EndOfStream()
{
  return m_pImpl->EndOfStream();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,CompressionStream,Stream);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CompressionStream::CompressionStream( DWORD Type, Ptr<Stream> pStream, INT CompressionLevel, BOOL AutoClose )
{
  switch(Type)
  {
  case CompressionType::NONE:
    m_pImpl = pStream;
    break;
  case CompressionType::GZIP:
#if defined(USE_ZLIB)
		m_pImpl = new GZipCompressionStream( pStream, CompressionLevel, AutoClose );
#else
		throw InvalidOperationException(this, TEXT("The CodeLibrary was not configured with USE_ZLIB.  GZip support is unavailable") );
#endif
    break;
  case CompressionType::BZIP2:
#if defined(USE_BZIP2)
    m_pImpl = new BZip2CompressionStream( pStream, CompressionLevel, AutoClose );
#else
		throw InvalidOperationException(this, TEXT("The CodeLibrary was not configured with USE_BZIP2.  BZip2 support is unavailable") );
#endif
    break;
  };
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CompressionStream::~CompressionStream()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID CompressionStream::Flush()
{
  m_pImpl->Flush();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID CompressionStream::Close()
{
  m_pImpl->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T CompressionStream::Write( LPCVOID pData, SIZE_T N )
{
  return m_pImpl->Write( pData, N );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T CompressionStream::Read( LPVOID pData, SIZE_T N )
{
  return m_pImpl->Read( pData, N );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CompressionStream::EndOfStream()
{
  return m_pImpl->EndOfStream();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Buffer<BYTE> Bolitho::CompressData(LPCVOID pData, SIZE_T N, DWORD Flags)
{
  if (Flags == CompressionType::NONE)
    return Buffer<BYTE>((LPCBYTE)pData, N);

  Buffer<BYTE> CompressedData;

  if (Flags == CompressionType::GZIP)
  {
    #if defined(USE_ZLIB)
    CONST SIZE_T BlockSize = 65536;

    z_stream ZLib;
    ZeroMemory( &ZLib, sizeof(z_stream) );

    deflateInit( &ZLib, Z_DEFAULT_COMPRESSION );

    ZLib.avail_in = (uInt)N;
    ZLib.next_in  = (Bytef*)pData;

    int Result = 0;
    do
    {
      if (CompressedData.Length() - ZLib.total_out < BlockSize)
        CompressedData.Resize( CompressedData.Length() + BlockSize );
      ZLib.avail_out = (uInt)CompressedData.Length() - ZLib.total_out;
      ZLib.next_out = (Bytef*)&CompressedData[(SIZE_T)ZLib.total_out];
      Result = deflate( &ZLib, Z_NO_FLUSH );
    }
    while( Result == Z_OK && ZLib.avail_in > 0 );

    ZLib.avail_in = 0;
    ZLib.next_in = 0;

    do
    {
      if (CompressedData.Length() - ZLib.total_out < BlockSize)
        CompressedData.Resize( CompressedData.Length() + BlockSize );
      ZLib.avail_out = (uInt)CompressedData.Length() - ZLib.total_out;
      ZLib.next_out = (Bytef*)&CompressedData[(SIZE_T)ZLib.total_out];
      Result = deflate( &ZLib, Z_FINISH );
    }
    while( Result != Z_STREAM_END );

    deflateEnd( &ZLib );
    CompressedData.Resize( ZLib.total_out );
    #endif
  }

  if (Flags == CompressionType::BZIP2)
  {
    #if defined(USE_BZIP2)
    CONST SIZE_T BlockSize = 65536;

    bz_stream BZLib;
    ZeroMemory( &BZLib, sizeof(bz_stream) );

    BZ2_bzCompressInit(&BZLib, 5, 0, 0);

    BZLib.avail_in = (uInt)N;
    BZLib.next_in  = (char*)pData;

    int Result = 0;
    do
    {
      if (CompressedData.Length() - BZLib.total_out_lo32 < BlockSize)
        CompressedData.Resize( CompressedData.Length() + BlockSize );
      BZLib.avail_out = (uInt)CompressedData.Length() - BZLib.total_out_lo32;
      BZLib.next_out = (char*)&CompressedData[(SIZE_T)BZLib.total_out_lo32];
      Result = BZ2_bzCompress(&BZLib, BZ_RUN);
    }
    while(Result == BZ_RUN_OK && BZLib.avail_in > 0);

    BZLib.avail_in = 0;
    BZLib.next_in = 0;

    do
    {
      if (CompressedData.Length() - BZLib.total_out_lo32 < BlockSize)
        CompressedData.Resize( CompressedData.Length() + BlockSize );
      BZLib.avail_out = (uInt)CompressedData.Length() - BZLib.total_out_lo32;
      BZLib.next_out = (char*)&CompressedData[(SIZE_T)BZLib.total_out_lo32];
      Result = BZ2_bzCompress(&BZLib, BZ_FINISH);
    }
    while(Result != BZ_STREAM_END);

    BZ2_bzCompressEnd(&BZLib);
    CompressedData.Resize(BZLib.total_out_lo32);
    #endif
  }

  return CompressedData;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Buffer<BYTE> Bolitho::DecompressData(LPCVOID pData, SIZE_T N, DWORD Flags)
{
  if (Flags == CompressionType::NONE)
    return Buffer<BYTE>((LPCBYTE)pData, N);
  
  Buffer<BYTE> DecompressedData;

  if (Flags == CompressionType::GZIP)
  {
    #if defined(USE_ZLIB)
    CONST SIZE_T BlockSize = 65536;

    z_stream ZLib;
    ZeroMemory( &ZLib, sizeof(z_stream) );

    inflateInit( &ZLib );

    ZLib.avail_in = (uInt)N;
    ZLib.next_in  = (Bytef*)pData;

    int Result = 0;
    do
    {
      if (DecompressedData.Length() - ZLib.total_out < BlockSize)
        DecompressedData.Resize(DecompressedData.Length() + BlockSize);
      ZLib.avail_out = (uInt)DecompressedData.Length() - ZLib.total_out;
      ZLib.next_out = (Bytef*)&DecompressedData[ZLib.total_out];
      Result = inflate(&ZLib, Z_NO_FLUSH);
    }
    while( Result != Z_STREAM_END && ZLib.avail_in > 0 );

    inflateEnd(&ZLib);
    DecompressedData.Resize(ZLib.total_out);  
    #endif
  }

  if (Flags == CompressionType::BZIP2)
  {
    #if defined(USE_BZIP2)
    CONST SIZE_T BlockSize = 65536;

    bz_stream BZLib;
    ZeroMemory( &BZLib, sizeof(bz_stream) );

    BZ2_bzDecompressInit(&BZLib, 0, 0);

    BZLib.avail_in = (uInt)N;
    BZLib.next_in  = (char*)pData;

    int Result = 0;
    do
    {
      if (DecompressedData.Length() - BZLib.total_out_lo32 < BlockSize)
        DecompressedData.Resize( DecompressedData.Length() + BlockSize );
      BZLib.avail_out = (uInt)DecompressedData.Length() - BZLib.total_out_lo32;
      BZLib.next_out = (char*)&DecompressedData[BZLib.total_out_lo32];
      Result = BZ2_bzDecompress(&BZLib);
    }
    while( Result != BZ_STREAM_END && BZLib.avail_in > 0 );

    BZ2_bzDecompressEnd(&BZLib);
    DecompressedData.Resize(BZLib.total_out_lo32);  
    #endif
  }

  return DecompressedData;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

