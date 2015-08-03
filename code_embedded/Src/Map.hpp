/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Map.hpp $
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

#include "Memory/Buffer.hpp"
using Bolitho::Buffer;

#include "IO/Stream.hpp"
using Bolitho::IO::Stream;

namespace NASA
{
  /** Represents the data stored in each map cell */
  struct MapCell
  {
    MapCell()
    { this->Height = 0; this->Flags = 0; }
    MapCell(FLOAT Height, DWORD Flags)
    { this->Height = Height; this->Flags = Flags; }
    
    FLOAT Height;
    DWORD Flags;
  };

  /** Represents a map on a 2D domain.  */
  class Map : public Object
  {
    DeclareRuntimeType();
  public:
    Map(UINT Width, UINT Height, FLOAT Resolution);

    /** Returns the width of the map, in cells */
    UINT Width() CONST
    { return m_Width; }
    /** Returns the height of the map, in cells */
    UINT Height() CONST
    { return m_Height; }

    /** Returns the size of a cell in real world units */
    FLOAT GetResolution() CONST
    { return m_Resolution; }

    /** Cell access */
    CONST MapCell& operator() (UINT x, UINT y) CONST;
    MapCell& operator() (UINT x, UINT y);

    /** Get the value of a cell */
    CONST MapCell& GetCell(UINT x, UINT y) CONST;
    /** Set the values of a cell */
    VOID SetCell(UINT x, UINT y, CONST MapCell& C);

    /** The width of the map in world units */
    FLOAT WorldWidth()
    { return m_Width * m_Resolution; }
    /** The height of the map in world units */
    FLOAT WorldHeight()
    { return m_Height * m_Resolution; }

    FLOAT& MaximumHeightValue()
    { return m_MaxHeight; }

    FLOAT& MinimumHeightValue()
    { return m_MinHeight; }

    /** Computes the map cell coordinates of a world coordinate */
    BOOL ComputeMapCell(FLOAT WorldX, FLOAT WorldY, UINT& MapX, UINT& MapY)
    {
      if (WorldX < (-WorldWidth() / 2.0f) || WorldX > (WorldWidth() / 2.0f))
        return FALSE;
      if (WorldY < (-WorldHeight() / 2.0f) || WorldY > (WorldHeight() / 2.0f))
        return FALSE;

      MapX = (UINT)((WorldX + (WorldWidth() / 2.0f)) / m_Resolution);
      MapY = (UINT)((WorldY + (WorldHeight() / 2.0f)) / m_Resolution);

      return TRUE;
    }

    /** I/O Support */
    VOID Read(Ptr<Stream> pStream);
    VOID Write(Ptr<Stream> pStream);

  protected:
    Buffer<MapCell> m_Cells;
    UINT m_Width;
    UINT m_Height;
    FLOAT m_Resolution;
    FLOAT m_MinHeight;
    FLOAT m_MaxHeight;

  };
}
