/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Path.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 255 $
 * Last Updated: $Date: 2007-11-04 07:28:02 -0500 (Sun, 04 Nov 2007) $
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

#include "Robot.hpp"
#include "Path.hpp"

#include "IO/BinaryStream.hpp"
using Bolitho::IO::BinaryStream;

//---------------------------------------------------------------------------------------------------
Path::Path()
{
}
//---------------------------------------------------------------------------------------------------
SIZE_T Path::Count() CONST
{
  return m_Waypoints.Length();
}
//---------------------------------------------------------------------------------------------------
VOID Path::Resize(SIZE_T N)
{
  m_Waypoints.Resize(N);
}
//---------------------------------------------------------------------------------------------------
PathWaypoint Path::GetWaypoint(SIZE_T i)
{
  return m_Waypoints[i];
}
//---------------------------------------------------------------------------------------------------
VOID Path::SetWaypoint(SIZE_T i, PathWaypoint C)
{
  m_Waypoints[i] = C;
}
//---------------------------------------------------------------------------------------------------
VOID Path::Read(Ptr<Stream> pStream)
{
  BinaryStream B(pStream);

  UINT32 Version;
  B >> Version;

  m_Waypoints.Resize(B.ReadUInt32());
  
  if (m_Waypoints.Length() > 0)
    B.Read(&m_Waypoints[0], m_Waypoints.Length()*sizeof(PathWaypoint));
}
//---------------------------------------------------------------------------------------------------
VOID Path::Write(Ptr<Stream> pStream) CONST
{
  BinaryStream B(pStream);
  UINT32 Version = 0x101;
  B << Version;
  B << (UINT32)m_Waypoints.Length();

  if (m_Waypoints.Length() > 0)
    B.Write(&m_Waypoints[0], m_Waypoints.Length()*sizeof(PathWaypoint));
}
//---------------------------------------------------------------------------------------------------


