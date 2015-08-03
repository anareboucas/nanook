/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: cisstConfig.h.in,v 1.15 2007/10/15 20:41:40 kapoor Exp $
  
  Author(s):	Anton Deguet
  Created on: 2004-02-18

  (C) Copyright 2004-2007 Johns Hopkins University (JHU), All Rights
  Reserved.
  
--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#ifndef _cisstConfig_h
#define _cisstConfig_h

#define ON 1
#define OFF 0

// Path to build root (to find some configuration files)
#define CISST_BUILD_ROOT "C:/code/CISST/build"

// Path to build files
#define CISST_BUILD_LINK_DIRECTORIES "C:/code/CISST/build/libs/lib"

// Was cisst compiled as shared libraries or static
#define CISST_BUILD_SHARED_LIBS OFF 

// use xereces and xalan for XML/XPath API
#define CISST_HAS_XERCES_XALAN 

// cnetlib, erc-cisst distribution of code from netlib.org
#define CISST_HAS_CNETLIB ON

// LAPACK3E, erc-cisst distribution of code from netlib.org
#define CISST_HAS_CISSTNETLIB OFF

// RTAI Realtime extension for Linux
#define CISST_HAS_LINUX_RTAI OFF

// ServoToGo card
#define CISST_HAS_STG 

// ServoToGo card with Windows Driver
#define CISST_HAS_WINSTG 

// JHU LoPoMoCo card 
#define CISST_HAS_LOPOMOCO 

// Motion Engineering Inc. card
#define CISST_HAS_MEI 

// NiDAQ based ATI Force/Torque sensor
#define CISST_HAS_ATIDAQFT 

// ISA ATI Force/Torque sensor
#define CISST_HAS_ATIISAFT 


#endif // _cisstConfig_h


// ****************************************************************************
//                              Change History
// ****************************************************************************
//
//  $Log: cisstConfig.h.in,v $
//  Revision 1.15  2007/10/15 20:41:40  kapoor
//  Added support for LIBXML2.
//
//  Revision 1.14  2007/04/26 19:33:57  anton
//  All files in libraries: Applied new license text, separate copyright and
//  updated dates, added standard header where missing.
//
//  Revision 1.13  2006/11/20 20:33:19  anton
//  Licensing: Applied new license to cisstCommon, cisstVector, cisstNumerical,
//  cisstInteractive, cisstImage and cisstOSAbstraction.
//
//  Revision 1.12  2006/10/31 02:58:22  anton
//  cisstConfig.h: Added flag to reflect if libs are compiled static or dynamic.
//
//  Revision 1.11  2005/11/20 21:35:18  kapoor
//  LAPACK: Added CISST_HAS_CISSTCNETLIB flag to cmake, which can be used to
//  compile code using the newer LAPACK3E. CLAPACK is still supported. See
//  ticket #193.
//
//  Revision 1.10  2005/09/26 21:23:30  anton
//  cisstCommon: Updated CMN_ASSERT and cmnThrow.  Variables are not defined in
//  cisstConfig.h anymore (user could not override).  CMN_ASSERT can now be set
//  to throw an exception (user can set preference with CMake).
//
//  Revision 1.9  2005/09/26 15:41:46  anton
//  cisst: Added modelines for emacs and vi.
//
//  Revision 1.8  2005/09/23 23:57:36  anton
//  cmnThrow: Added CMake option to force an abort() instead of throw.
//
//  Revision 1.7  2005/07/06 02:35:28  anton
//  cisstConfig.h.in: Added variable CISST_BUILD_ROOT.
//
//  Revision 1.6  2005/05/19 19:29:00  anton
//  cisst libs: Added the license to cisstCommon and cisstVector
//
//  Revision 1.5  2004/11/11 20:32:58  anton
//  cisstConfig.h: Added flag for CISST_HAS_CNETLIB
//
//  Revision 1.4  2004/10/07 18:26:34  anton
//  CMake build process: Added a #define CISST_BUILD_LINK_DIRECTORIES so that
//  build executable can define a path to dynamically load libraries (e.g.
//  Python modules).  Also added CISST_SWIG_INCLUDE_DIR so that one can
//  import the .i files for SWIG.
//
//  Revision 1.3  2004/05/27 19:33:26  anton
//  Added CISST_HAS_WINSTG
//
//  Revision 1.2  2004/05/27 17:09:25  anton
//  Incorporated code from Ankur for the ATI, MEI, STG and LoPoMoCo devices.
//  The build process will have to be redone whenever will will add dynamic
//  loading.
//
//  Revision 1.1  2004/02/18 22:13:48  anton
//  Creation
//
//
// ****************************************************************************

