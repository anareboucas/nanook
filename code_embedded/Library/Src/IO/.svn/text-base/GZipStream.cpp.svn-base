/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/GZipStream.cpp $
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
#include "GZipStream.hpp"

#if defined(USE_ZLIB)

using namespace Bolitho::IO;
using namespace Bolitho;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,GZipDecompressionStream,Stream);
ImplementRuntimeType(Bolitho::IO,GZipCompressionStream,Stream);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
GZipCompressionStream::GZipCompressionStream( Ptr<Stream> pStream, INT CompressionLevel, BOOL AutoClose )
{
  m_pStream = pStream;
  ZeroMemory( &m_ZLibState, sizeof(z_stream) );
  m_AutoClose = AutoClose;

  deflateInit( &m_ZLibState, CompressionLevel );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
GZipCompressionStream::~GZipCompressionStream()
{
  Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID GZipCompressionStream::Flush()
{
  Buffer<BYTE> WriteBuffer = Buffer<BYTE>(4096);

  m_ZLibState.avail_in = 0;
  m_ZLibState.next_in = 0;

  int Result = 0;
  do
  {
    m_ZLibState.avail_out = (uInt)WriteBuffer.Length();
    m_ZLibState.next_out = (Bytef*)&WriteBuffer[0];
    Result = deflate( &m_ZLibState, Z_FULL_FLUSH );
    m_pStream->Write( &WriteBuffer[0], WriteBuffer.Length() - m_ZLibState.avail_out );
  }
  while( Result == Z_OK && m_ZLibState.avail_out == 0 );

  if (m_pStream)
    return m_pStream->Flush();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID GZipCompressionStream::Close()
{
  Buffer<BYTE> WriteBuffer = Buffer<BYTE>(4096);
  SIZE_T CompressedTotal = 0;

  m_ZLibState.avail_in = 0;
  m_ZLibState.next_in = 0;

  int Result = 0;
  do
  {
    m_ZLibState.avail_out = (uInt)WriteBuffer.Length();
    m_ZLibState.next_out = (Bytef*)&WriteBuffer[0];
    Result = deflate( &m_ZLibState, Z_FINISH );
    CompressedTotal += m_pStream->Write( &WriteBuffer[0], WriteBuffer.Length() - m_ZLibState.avail_out );
  }
  while( Result != Z_STREAM_END );

  deflateEnd( &m_ZLibState );
  
  m_pStream->Flush();
  if (m_AutoClose)
    m_pStream->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T GZipCompressionStream::Write( LPCVOID pData, SIZE_T N )
{
  Buffer<BYTE> WriteBuffer = Buffer<BYTE>(4096);
  
  m_ZLibState.avail_in = (uInt)N;
  m_ZLibState.next_in = (Bytef*)pData;

  int Result = 0;
  do
  {
    m_ZLibState.avail_out = (uInt)WriteBuffer.Length();
    m_ZLibState.next_out = (Bytef*)&WriteBuffer[0];
    Result = deflate( &m_ZLibState, Z_NO_FLUSH );
    m_pStream->Write( &WriteBuffer[0], WriteBuffer.Length() - m_ZLibState.avail_out );
  }
  while( Result == Z_OK && m_ZLibState.avail_out == 0 );

  return N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T GZipCompressionStream::Read( LPVOID pData, SIZE_T N )
{
  throw InvalidOperationException( this, TEXT("Cannot read from a DeflateStream") );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL GZipCompressionStream::EndOfStream()
{
  if (m_pStream)
    return m_pStream->EndOfStream();
  else
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
GZipDecompressionStream::GZipDecompressionStream( Ptr<Stream> pStream, BOOL AutoClose ) :
  m_ReadBuffer(4096)
{
  m_pStream = pStream;
  ZeroMemory( &m_ZLibState, sizeof(z_stream) );
  m_AutoClose = AutoClose;

  inflateInit( &m_ZLibState );

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
GZipDecompressionStream::~GZipDecompressionStream()
{
  Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID GZipDecompressionStream::Flush()
{
  if (m_pStream)
    return m_pStream->Flush();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID GZipDecompressionStream::Close()
{
  inflateEnd( &m_ZLibState );

  m_pStream->Flush();
  if (m_AutoClose)
    m_pStream->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T GZipDecompressionStream::Write( LPCVOID pData, SIZE_T N )
{
  throw InvalidOperationException( this, TEXT("Cannot write to an InflateStream") );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T GZipDecompressionStream::Read( LPVOID pData, SIZE_T N )
{
  m_ZLibState.avail_out = (uInt)N;
  m_ZLibState.next_out = (Bytef*)pData;
  
  int Result = 0;
  do
  {
    if (m_ZLibState.avail_in == 0)
    {
      m_ZLibState.avail_in = (uInt)m_pStream->Read( &m_ReadBuffer[0], m_ReadBuffer.Length() );
      m_ZLibState.next_in = (Bytef*)&m_ReadBuffer[0];
      if (m_ZLibState.avail_in == 0)
        break;
    }
    Result = inflate( &m_ZLibState, Z_NO_FLUSH );
  }
  while( Result == Z_OK && m_ZLibState.avail_out > 0 );

  return N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL GZipDecompressionStream::EndOfStream()
{
  if (m_pStream)
    return m_pStream->EndOfStream();
  else
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#endif