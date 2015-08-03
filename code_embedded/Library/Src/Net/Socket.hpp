/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Net/Socket.hpp $
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

#if defined(USE_SOCKETS)

#include "Net/IPEndPoint.hpp"
#include "IO/File.hpp"

namespace Bolitho
{
  namespace Net
  {
		struct AddressFamily
		{ enum { InterNetwork = AF_INET, InterNetworkV6 = AF_INET6 }; };

		struct SocketType
		{ enum { Stream = SOCK_STREAM, Datagram = SOCK_DGRAM }; };

		struct ProtocolType
		{ enum { Tcp = IPPROTO_TCP, Udp = IPPROTO_UDP }; };

    class LIBRARY_API SocketException : public Exception
    {
      DeclareRuntimeType();
    public:
      SocketException(CONST Object* pSource, CONST Exception* pInnerException ) : Exception(pSource, pInnerException)
      {}
    };
    
    class LIBRARY_API Socket : public Object
    {
      DeclareRuntimeType();
    public:
      Socket( DWORD AddressFamily, DWORD SocketType, DWORD ProtocolType );
      virtual ~Socket();
      
      VOID Connect( IPEndPoint EndPoint );
      
      VOID Listen();
      VOID Bind( IPEndPoint EndPoint );
      Ptr<Socket> Accept();
      
      VOID Close();
      
      SIZE_T Recieve( LPVOID pBuffer, SIZE_T Length, BOOL WaitForCompleteBuffer = FALSE );
      SIZE_T Send( LPCVOID pBuffer, SIZE_T Length );
      
      VOID TransmitFile( Ptr<IO::File> pFile, SIZE_T FileLength, LPVOID pHead, SIZE_T HeadLength, LPVOID pTail, SIZE_T TailLength );
      
      CONST IPEndPoint& EndPoint() CONST
      { return m_EndPoint; }
    private:
      Socket(SOCKETHANDLE S);
      SOCKETHANDLE m_Socket;
      
      IPEndPoint m_EndPoint;
    };
  }
}

#endif
 

