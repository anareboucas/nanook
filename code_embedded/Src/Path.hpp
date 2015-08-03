/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Path.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 260 $
 * Last Updated: $Date: 2007-11-07 15:21:03 -0500 (Wed, 07 Nov 2007) $
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

#include "Collections/Array.hpp"
using Bolitho::Array;

#include "IO/Stream.hpp"
using Bolitho::IO::Stream;

#include "Math/Vector2.hpp"
using Bolitho::Math::Vector2F;

namespace NASA
{
  struct PathWaypoint
  {
    PathWaypoint()
    { ScanMode = 0; }
    PathWaypoint(Vector2F W)
    { Waypoint = W; ScanMode = 0; }
    PathWaypoint(Vector2F W, DWORD S)
    { Waypoint = W; ScanMode = S; }
    
    Vector2F Waypoint;
    DWORD ScanMode;
  };

  /* Represents a path through a 2D space, defined by a series of waypoints */
  class Path : public Object
  {
  public:
    Path();

    /* Returns the number of waypoints in the path */
    SIZE_T Count() CONST;
    /* Sets the number of waypoints in the path */
    VOID Resize(SIZE_T N);

    /* Adds a new waypoint */
    VOID Add(CONST PathWaypoint& Waypoint)
    { m_Waypoints.Add(Waypoint); }

    /* Returns the i-th waypoint */
    PathWaypoint GetWaypoint(SIZE_T i);
    /* Sets the i-th waypoint */
    VOID SetWaypoint(SIZE_T i, PathWaypoint C);

    PathWaypoint& operator[] (SIZE_T i)
    { return m_Waypoints[i]; }
    CONST PathWaypoint& operator[] (SIZE_T i) CONST
    { return m_Waypoints[i]; }

    /* I/O */
    VOID Read(Ptr<Stream> pStream);
    VOID Write(Ptr<Stream> pStream) CONST;

  protected:
    Array<PathWaypoint> m_Waypoints;

  };
}
