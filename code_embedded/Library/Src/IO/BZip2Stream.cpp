/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/BZip2Stream.cpp $
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

#if defined(USE_BZIP2)

using namespace Bolitho;
using namespace Bolitho::IO;

//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,BZip2CompressionStream,Stream);
ImplementRuntimeType(Bolitho::IO,BZip2DecompressionStream,Stream);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BZip2CompressionStream::BZip2CompressionStream( Ptr<Stream> pStream, INT CompressionLevel, BOOL AutoClose )
{
  m_pStream = pStream;
  ZeroMemory( &m_State, sizeof(bz_stream) );
  m_AutoClose = AutoClose;

  BZ2_bzCompressInit( &m_State, CompressionLevel, 0, 0 );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BZip2CompressionStream::~BZip2CompressionStream()
{
  Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BZip2CompressionStream::Flush()
{
  Buffer<BYTE> WriteBuffer = Buffer<BYTE>(4096);

  m_State.avail_in = 0;
  m_State.next_in = 0;

  int Result = 0;
  do
  {
    m_State.avail_out = (unsigned int)WriteBuffer.Length();
    m_State.next_out = (char*)&WriteBuffer[0];
    Result = BZ2_bzCompress( &m_State, BZ_FLUSH );
    m_pStream->Write( &WriteBuffer[0], WriteBuffer.Length() - m_State.avail_out );
  }
  while( Result == BZ_RUN_OK && m_State.avail_out == 0 );

  if (m_pStream)
    return m_pStream->Flush();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BZip2CompressionStream::Close()
{
  Buffer<BYTE> WriteBuffer = Buffer<BYTE>(4096);
  SIZE_T CompressedTotal = 0;

  m_State.avail_in = 0;
  m_State.next_in = 0;

  int Result = 0;
  do
  {
    m_State.avail_out = (unsigned int)WriteBuffer.Length();
    m_State.next_out = (char*)&WriteBuffer[0];
    Result = BZ2_bzCompress( &m_State, BZ_FINISH );
    CompressedTotal += m_pStream->Write( &WriteBuffer[0], WriteBuffer.Length() - m_State.avail_out );
  }
  while( Result != BZ_STREAM_END );

  BZ2_bzCompressEnd( &m_State );
  
  m_pStream->Flush();
  if (m_AutoClose)
    m_pStream->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T BZip2CompressionStream::Write( LPCVOID pData, SIZE_T N )
{
  Buffer<BYTE> WriteBuffer = Buffer<BYTE>(4096);
  
  m_State.avail_in = (unsigned int)N;
  m_State.next_in = (char*)pData;

  int Result = 0;
  do
  {
    m_State.avail_out = (unsigned int)WriteBuffer.Length();
    m_State.next_out = (char*)&WriteBuffer[0];
    Result = BZ2_bzCompress( &m_State, BZ_RUN );
    m_pStream->Write( &WriteBuffer[0], WriteBuffer.Length() - m_State.avail_out );
  }
  while( Result == BZ_RUN_OK && m_State.avail_out == 0 );

  return N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T BZip2CompressionStream::Read( LPVOID pData, SIZE_T N )
{
  throw InvalidOperationException( this, TEXT("Cannot read from a DeflateStream") );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL BZip2CompressionStream::EndOfStream()
{
  if (m_pStream)
    return m_pStream->EndOfStream();
  else
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
BZip2DecompressionStream::BZip2DecompressionStream( Ptr<Stream> pStream, BOOL AutoClose ) :
  m_ReadBuffer(4096)
{
  m_pStream = pStream;
  ZeroMemory( &m_State, sizeof(bz_stream) );
  m_AutoClose = AutoClose;

  BZ2_bzDecompressInit( &m_State, 0, 0 );

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BZip2DecompressionStream::~BZip2DecompressionStream()
{
  Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BZip2DecompressionStream::Flush()
{
  if (m_pStream)
    return m_pStream->Flush();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID BZip2DecompressionStream::Close()
{
  BZ2_bzDecompressEnd( &m_State );

  m_pStream->Flush();
  if (m_AutoClose)
    m_pStream->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T BZip2DecompressionStream::Write( LPCVOID pData, SIZE_T N )
{
  throw InvalidOperationException( this, TEXT("Cannot write to an InflateStream") );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T BZip2DecompressionStream::Read( LPVOID pData, SIZE_T N )
{
  m_State.avail_out = (unsigned int)N;
  m_State.next_out = (char*)pData;
  
  int Result = 0;
  do
  {
    if (m_State.avail_in == 0)
    {
      m_State.avail_in = (unsigned int)m_pStream->Read( &m_ReadBuffer[0], m_ReadBuffer.Length() );
      m_State.next_in = (char*)&m_ReadBuffer[0];
      if (m_State.avail_in == 0)
        break;
    }
    Result = BZ2_bzDecompress( &m_State );
  }
  while( Result == BZ_OK && m_State.avail_out > 0 );

  return N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL BZip2DecompressionStream::EndOfStream()
{
  if (m_pStream)
    return m_pStream->EndOfStream();
  else
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

#endif