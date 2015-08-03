/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Map.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 265 $
 * Last Updated: $Date: 2007-11-09 13:14:44 -0500 (Fri, 09 Nov 2007) $
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
#include "Map.hpp"

#include "IO/BinaryStream.hpp"
using Bolitho::IO::BinaryStream;


//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,Map,Object);
//----------------------------------------------------------------------------------------------------------------------------
Map::Map(UINT Width, UINT Height, FLOAT Resolution)
{
  m_Width = Width;
  m_Height = Height;
  m_Resolution = Resolution;
  m_Cells.Resize(Width*Height);
  m_MinHeight = MAXFLOAT;
  m_MaxHeight = MINFLOAT;
}
//----------------------------------------------------------------------------------------------------------------------------
CONST MapCell& Map::operator() (UINT x, UINT y) CONST
{
  return m_Cells[y*m_Width + x];
}
//----------------------------------------------------------------------------------------------------------------------------
MapCell& Map::operator() (UINT x, UINT y)
{
  return m_Cells[y*m_Width + x];
}
//----------------------------------------------------------------------------------------------------------------------------
CONST MapCell& Map::GetCell(UINT x, UINT y) CONST
{
  return m_Cells[y*m_Width + x];
}
//----------------------------------------------------------------------------------------------------------------------------
VOID Map::SetCell(UINT x, UINT y, CONST MapCell& C)
{
  m_Cells[y*m_Width + x] = C;
}
//----------------------------------------------------------------------------------------------------------------------------
VOID Map::Read(Ptr<Stream> pStream)
{
  BinaryStream B(pStream);
  DWORD Version;

  B >> Version; // Should be 0x0101
  B >> m_Width;
  B >> m_Height;
  B >> m_Resolution;
  B >> m_MinHeight;
  B >> m_MaxHeight;

  m_Cells.Resize(m_Width*m_Height);
  B.Read(&m_Cells[0], m_Width*m_Height*sizeof(MapCell));

}
//----------------------------------------------------------------------------------------------------------------------------
VOID Map::Write(Ptr<Stream> pStream)
{
  BinaryStream B(pStream);
  DWORD Version = 0x0101;

  B << Version;
  B << m_Width;
  B << m_Height;
  B << m_Resolution;
  B << m_MinHeight;
  B << m_MaxHeight;

  B.Write(&m_Cells[0], m_Width*m_Height*sizeof(MapCell));
}
//----------------------------------------------------------------------------------------------------------------------------

