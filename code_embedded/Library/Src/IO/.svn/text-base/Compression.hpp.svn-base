/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/IO/Compression.hpp $
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

namespace Bolitho
{
  struct CompressionType
  {
    enum { NONE, GZIP, BZIP2, DEFAULT = GZIP };
  };

  LIBRARY_API Buffer<BYTE> CompressData(LPCVOID pData, SIZE_T N, DWORD Flags = CompressionType::DEFAULT);
  LIBRARY_API Buffer<BYTE> DecompressData(LPCVOID pData, SIZE_T N, DWORD Flags = CompressionType::DEFAULT);

  namespace IO
  {
    class LIBRARY_API DecompressionStream : public Stream
    {
      DeclareRuntimeType();
    public:
      DecompressionStream( DWORD Type, Ptr<Stream> pStream, BOOL AutoClose = TRUE );
      virtual ~DecompressionStream();

      virtual VOID Flush();
      virtual VOID Close();

      virtual SIZE_T Write( LPCVOID pData, SIZE_T N );
      virtual SIZE_T Read( LPVOID pData, SIZE_T N );

      virtual BOOL EndOfStream();
    protected:
      Ptr<Stream> m_pImpl;
    };

    class LIBRARY_API CompressionStream : public Stream
    {
      DeclareRuntimeType();
    public:
      CompressionStream( DWORD Type, Ptr<Stream> pStream, INT CompressionLevel = -1, BOOL AutoClose = TRUE );
      virtual ~CompressionStream();

      virtual VOID Flush();
      virtual VOID Close();

      virtual SIZE_T Write( LPCVOID pData, SIZE_T N );
      virtual SIZE_T Read( LPVOID pData, SIZE_T N );

      virtual BOOL EndOfStream();

    protected:
      Ptr<Stream> m_pImpl;
    };

  }
}
