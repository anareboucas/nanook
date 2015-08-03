/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Memory/Memory.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1310 $
 * Last Updated: $Date: 2007-12-02 12:48:02 -0500 (Sun, 02 Dec 2007) $
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

#include "Utility/Traits.hpp"

INLINE PVOID AllocateMemory( SIZE_T Length )
{ return calloc( 1, Length ); }
INLINE VOID FreeMemory( PVOID p )
{ free(p); }

INLINE INT CompareMemory(LPCVOID A, LPCVOID B, SIZE_T Length)
{ return memcmp( A, B, Length ); }

template<class T>
INLINE VOID CopyObjects(T* Destination, CONST T* Source, SIZE_T N)
{
  if (Bolitho::Traits<T>::IsPrimitive)
    memmove( Destination, Source, sizeof(T)*N );
  else
    for (SIZE_T i=0; i<N; i++)
      Destination[i] = Source[i];
}

template<class T>
INLINE VOID InitializeObjects(T* Destination, SIZE_T N)
{
  if (Bolitho::Traits<T>::IsPrimitive)
    memset( Destination, 0, sizeof(T)*N );
  else
    for (SIZE_T i=0; i<N; i++)
      Destination[i] = T();
}

#if defined(PLATFORM_LINUX)
	INLINE VOID CopyMemory( LPVOID Dest, LPCVOID Source, SIZE_T Length )
	{ memcpy(Dest, Source, Length); }

	INLINE VOID MoveMemory( LPVOID Dest, LPCVOID Source, SIZE_T Length )
	{ memmove(Dest, Source, Length); }

	INLINE VOID FillMemory( LPVOID Dest, SIZE_T Length, BYTE Fill )
	{ memset(Dest, Fill, Length); }

	INLINE VOID ZeroMemory( LPVOID Dest, SIZE_T Length )
	{ memset(Dest, 0, Length); }
#endif

namespace Bolitho
{
  LIBRARY_API VOID SwapByteOrder( PUINT16 x );
  LIBRARY_API VOID SwapByteOrder( PINT16 x );
  LIBRARY_API VOID SwapByteOrder( PUINT32 x );
  LIBRARY_API VOID SwapByteOrder( PINT32 x );
  LIBRARY_API VOID SwapByteOrder( PUINT64 x );
  LIBRARY_API VOID SwapByteOrder( PINT64 x );
  LIBRARY_API VOID SwapByteOrder( PFLOAT x );
  LIBRARY_API VOID SwapByteOrder( PDOUBLE x );

	#include "MemoryImpl.hpp"
}

