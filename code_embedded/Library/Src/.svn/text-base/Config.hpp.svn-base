/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Config.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1323 $
 * Last Updated: $Date: 2007-12-03 13:00:09 -0500 (Mon, 03 Dec 2007) $
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

#define USE_COM
#define USE_SOCKETS

#define PLATFORM_WINDOWS_SERVER2008 0x0601
#define PLATFORM_WINDOWS_VISTA 0x0600
#define PLATFORM_WINDOWS_SERVER2003 0x0502
#define PLATFORM_WINDOWS_XP 0x0501
#define PLATFORM_WINDOWS_2000 0x0500

#define PLATFORM_LINUX_26 0x0260
#define PLATFORM_LINUX_24 0x0240

#if defined(_WIN32) || defined(_WIN64)
  #define PLATFORM_WINDOWS PLATFORM_WINDOWS_2000
#elif defined(_M_CEE)
  #define PLATFORM_CLR
#elif defined(__FreeBSD__)
  #define PLATFORM_FREEBSD
#elif defined(__APPLE__)
  #define PLATFORM_MACOS
#else
  #define PLATFORM_LINUX PLATFORM_LINUX_26
#endif

#if defined(_MSC_VER)
  #define COMPILER_MSVC _MSC_VER
  #if _MSC_VER == 1500
    #define COMPILER_MSVC_90
  #endif
  #if _MSC_VER == 1400
    #define COMPILER_MSVC_80
  #endif
  #if _MSC_VER == 1310
    #define COMPILER_MSVC_71
  #endif
  #if _MSC_VER == 1300
    #define COMPILER_MSVC_70
  #endif
  #if _MSC_VER == 1200
    #define COMPILER_MSVC_60
  #endif

	#if defined(_M_IX86)
		#define PLATFORM_32BIT
		#define ARCH_I386
    #define ARCH_LITTLEENDIAN
	#endif
	#if defined(_M_X64)
		#define PLATFORM_64BIT
		#define ARCH_AMD64
    #define ARCH_LITTLEENDIAN
	#endif

#elif defined(__GNUC__)
  #define COMPILER_GCC __GNUC__
  #define COMPILER_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

	#if defined(__i386__)
		#define PLATFORM_32BIT
		#define ARCH_I386
    #define ARCH_LITTLEENDIAN
	#endif
	#if defined(__x86_64__)
		#define PLATFORM_64BIT
		#define ARCH_AMD64
    #define ARCH_LITTLEENDIAN
  #endif

  #if defined(__PPC__)
    #defined PLATFORM_32BIT
    #define ARCH_PPC
    #define ARCH_BIGENDIAN
  #endif
#endif

#if defined(PLATFORM_LINUX) || defined(PLATFORM_FREEBSD)
  #define LIBRARY_API
  #define TEXT(quote) quote

	#undef USE_DIRECTX9
	#undef USE_UI
	#undef USE_COM
	#undef USE_ADO
#endif

#if defined(_DEBUG) && !defined(DEBUG)
  #define DEBUG 1
#endif

#if !defined(DEBUG) && !defined(RELEASE) || defined(NDEBUG)
  #define RELEASE 1
#endif

#define USE_TYPESYSTEM

#if defined(COMPILER_MSVC_80)
  #if !defined(_CRT_SECURE_NO_DEPRECATE) 
    #define _CRT_SECURE_NO_DEPRECATE
  #endif
  #if !defined(_SCL_SECURE_NO_DEPRECATE) 
    #define _SCL_SECURE_NO_DEPRECATE
  #endif
#endif

#if defined(USE_SSE2) && !defined(USE_SSE)
  #define USE_SSE
#endif

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <memory>

#if defined(PLATFORM_LINUX)
	#define USE_UNICODE
	#include <wchar.h>
	#include <sys/mman.h>
  #include <sys/time.h>
  #include <sys/times.h> 
  #include <sys/stat.h>
  #include <fcntl.h> 
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/sendfile.h>
  #include <sys/param.h>
  #include <stdlib.h>
	#include <errno.h>
	#include <pthread.h>
	#include <sys/signal.h>
	#include <sched.h> 
	#include <dlfcn.h>
	#include <semaphore.h>

  #if defined(USE_SOCKETS)
    #include <netdb.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
  #endif
  
#endif

#if defined(PLATFORM_FREEBSD)

#endif

#if defined(PLATFORM_WINDOWS)
  #define USE_UNICODE

	#include <crtdefs.h>

	#define _WINSOCKAPI_
	#define NOMINMAX
	#define _WIN32_WINNT PLATFORM_WINDOWS
	#include <windows.h>
  #define TaskDialog _TaskDialog
	#include <commctrl.h>
  #undef TaskDialog
	#include <shlwapi.h>
	#include <psapi.h>
	#include <dbghelp.h>

  #if defined(LIBRARYAPI_EXPORT) 
		#define LIBRARY_API __declspec(dllexport)
	#elif defined(LIBRARYAPI_STATIC)
    #define LIBRARY_API
    #if !defined(LIBRARYAPI_BUILD)
    #if defined(DEBUG) && defined(UNICODE)
    #pragma comment(lib, "LibraryStaticDW.lib")
    #elif defined(DEBUG) && !defined(UNICODE)
    #pragma comment(lib, "LibraryStaticDA.lib")
    #elif defined(RELEASE) && defined(UINCODE)
    #pragma comment(lib, "LibraryStaticW.lib")
    #elif defined(RELEASE) && !defined(UINCODE)
    #pragma comment(lib, "LibraryStaticA.lib")
    #endif
    #endif
	#elif defined(LIBRARYAPI_IMPORT)
    #define LIBRARY_API __declspec(dllimport)
    #if !defined(LIBRARYAPI_BUILD)
    #if defined(DEBUG) && defined(UNICODE)
    #pragma comment(lib, "LibraryDW.lib")
    #elif defined(DEBUG) && !defined(UNICODE)
    #pragma comment(lib, "LibraryDA.lib")
    #elif defined(RELEASE) && defined(UINCODE)
    #pragma comment(lib, "LibraryW.lib")
    #elif defined(RELEASE) && !defined(UINCODE)
    #pragma comment(lib, "LibraryA.lib")
    #endif
    #endif
  #else
    #error "Must define LIBRARYAPI_STATIC, LIBRARYAPI_EXPORT or LIBRARYAPI_IMPORT"
	#endif

  #if defined(USE_SOCKETS)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <wspiapi.h>
    #include <mswsock.h> 

    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "mswsock.lib")
  #endif

  #pragma comment(lib, "kernel32.lib")
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "gdi32.lib")
	#pragma comment(lib, "ole32.lib")
	#pragma comment(lib, "oleaut32.lib")
	#pragma comment(lib, "psapi.lib")
	#pragma comment(lib, "advapi32.lib")
	#pragma comment(lib, "comctl32.lib")
	#pragma comment(lib, "comdlg32.lib")
	#pragma comment(lib, "dbghelp.lib")
	#pragma comment(lib, "version.lib")
	#pragma comment(lib, "shlwapi.lib")

	#pragma warning( disable: 4251 )
	#pragma warning( disable: 4996 )
#endif

#undef ERROR

#include "Types.hpp"
