/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Image.hpp $
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

#include "Graphics/Color.hpp"
typedef Bolitho::Graphics::ColorBGR24 Color;

#include "Time/DateTime.hpp"
using Bolitho::DateTime;

#include "Math/Vector2.hpp"
using Bolitho::Math::Vector2F;

#include "Math/Vector3.hpp"
using Bolitho::Math::Vector3F;

using Bolitho::IO::Stream;
using Bolitho::Buffer;

namespace NASA
{
  /* Represents a 2d array of color values */
  class Image : public Object
  {
    DeclareRuntimeType();
  public:
    Image();
    Image(UINT SequenceNumber, UINT Width, UINT Height);

    /* Gets the value from an element */
    Color Get(UINT i, UINT j) CONST;
    /* Set the value for an element */
    VOID Set(UINT i, UINT j, Color v);
    CONST Color& operator() (UINT i, UINT j) CONST;
    Color& operator() (UINT i, UINT j);


    /* Gets a pointer to the start of a scan line */
    Color* GetScanLine(UINT i);
    CONST Color* GetScanLine(UINT i) CONST;
    CONST Color* operator[] (UINT i) CONST;
    Color* operator[] (UINT i);

    /* The height of the range image */
    UINT Width() CONST
    { return m_Width; }

    /* The width of the range image */
    UINT Height() CONST
    { return m_Height; }

    /* The time that the scan was taken */
    DateTime AquiredOn() CONST
    { return m_AquiredOn; }

    /* The scan sequence number */
    UINT SequenceNumber() CONST
    { return m_SequenceNumber; }

    /* The location of the scanner at the time of aquisition */
    Vector2F CameraPosition() CONST
    { return m_CameraPosition; }

    /* The orientation of the scanner at the time of aquisition */
    Vector2F CameraPrimaryAxis() CONST
    { return m_CameraAxis; }

    /* The horizontal field of view of the scanner */
    FLOAT HorizontalFieldOfView() CONST
    { return m_HorizontalFieldOfView; }

    /* The vertical field of view of the scanner */
    FLOAT VerticalFieldOfView() CONST
    { return m_VerticalFieldOfView; }

    /* I/O Support */
    VOID Read(Ptr<Stream> pStream);
    VOID Write(Ptr<Stream> pStream);

  protected:
    Buffer<Color> m_ColorValues;
    UINT m_Width;
    UINT m_Height;

    Vector2F m_CameraPosition;
    Vector2F m_CameraAxis;
    FLOAT m_HorizontalFieldOfView;
    FLOAT m_VerticalFieldOfView;
    DateTime m_AquiredOn;
    UINT m_SequenceNumber;
  };


}
