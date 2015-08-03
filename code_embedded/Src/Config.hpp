/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Config.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 298 $
 * Last Updated: $Date: 2008-01-25 12:18:42 -0500 (Fri, 25 Jan 2008) $
 * 
 * 
 * Copyright (c) 2005-2007, Matthew G Bolitho
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of The Johns Hopkins University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***************************************************************************************************/

#pragma once

#define LIBRARYAPI_MINIMAL
#define LIBRARYAPI_STATIC
#define USE_SOCKETS

#include "CodeLibrary.hpp"

#include "Debug/Trace.hpp"
using Bolitho::Trace;
using Bolitho::Trace0;
using Bolitho::Trace1;
using Bolitho::Trace2;
using Bolitho::Trace3;
using Bolitho::Trace4;
using Bolitho::TraceLevel;

using Bolitho::Ptr;
using Bolitho::Object;
using Bolitho::Array;
using Bolitho::String;
using Bolitho::StringPair;
using Bolitho::StringA;
using Bolitho::StringW;

using Bolitho::Exception;

namespace NASA {}
using namespace NASA;

/* To enable simualtion mode, define the following preprocessor symbols */
#if 0
#define DEVICECONTROL_SIMULATION
#define MOTORCONTROLLER_SIMULATION 
#define MOTORENCODER_SIMULATION 
#define LADARMOTORCONTROLLER_SIMULATION 
#define LADAR_SIMULATION

#define ALLOW_MOTORCONTROLLER_FAILURE 1
#define ALLOW_MOTORENCODER_FAILURE 1
#define ALLOW_LADARMOTORCONTROLLER_FAILURE 1
#define ALLOW_LADAR_FAILURE 1
#endif


/* Define the symbol SPACECUBE to compile for the space cube */
//#define SPACECUBE

