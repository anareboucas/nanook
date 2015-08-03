/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RangeImage.hpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 267 $
 * Last Updated: $Date: 2007-11-09 15:10:51 -0500 (Fri, 09 Nov 2007) $
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

#include "Time/DateTime.hpp"
using Bolitho::DateTime;

#include "Math/Vector2.hpp"
using Bolitho::Math::Vector2F;

#include "Math/Vector3.hpp"
using Bolitho::Math::Vector3F;

#include "Math/Matrix3.hpp"
using Bolitho::Math::Matrix3F;

#include "Map.hpp"

namespace NASA
{
  /* Represents a 'range image' -- a 2D domain of distance information */
  class RangeImage : public Object
  {
    DeclareRuntimeType();
  public:
    RangeImage();
    RangeImage(UINT SequenceNumber, UINT Width, UINT Height);

    /* Gets the value from an element */
    FLOAT Get(UINT i, UINT j) CONST;
    /* Set the value for an element */
    VOID Set(UINT i, UINT j, FLOAT v);
    CONST FLOAT& operator() (UINT i, UINT j) CONST;
    FLOAT& operator() (UINT i, UINT j);


    /* Gets a pointer to the start of a scan line */
    FLOAT* GetScanLine(UINT i);
    CONST FLOAT* GetScanLine(UINT i) CONST;
    CONST FLOAT* operator[] (UINT i) CONST;
    FLOAT* operator[] (UINT i);

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
    Vector2F& ScannerPosition()
    { return m_ScannerPosition; }

    /* The orientation of the scanner at the time of aquisition */
    Vector2F& ScannerPrimaryAxis()
    { return m_ScannerAxis; }

    /* The rotational orientation of the scanner at the time of aqusition */
    Matrix3F& ScannerRotation()
    { return m_ScannerRotation; }

    /* The horizontal field of view of the scanner */
    FLOAT& HorizontalFieldOfView()
    { return m_HorizontalFieldOfView; }

    /* The vertical field of view of the scanner */
    FLOAT& VerticalFieldOfView()
    { return m_VerticalFieldOfView; }

    /* The angle of the center of the horizontal sweep, relative to the forward direction */
    FLOAT& HorizontalBias()
    { return m_HorizontalBias; }

    /* The angle of the center of the vertical sweep, relative to the horizon */
    FLOAT& VerticalBias()
    { return m_VerticalBias; }

    FLOAT& MaximumRange()
    { return m_MaximumRange; }

    /* I/O Support */
    VOID Read(Ptr<Stream> pStream);
    VOID Write(Ptr<Stream> pStream);

    /* Converts a single point into 3-space */
    Vector3F ConvertToPoint(UINT i, UINT j) CONST;
    /* Converts the range image to a pointset */
    Array<Vector3F> ConvertToPointset(FLOAT MaxDistance) CONST;

    /* Splats into the map */
    VOID Splat(Ptr<Map> pMap, FLOAT MaxmimumHeight) CONST;

	static const char WIDTH_KEY = 0x0;
	static const char HEIGHT_KEY = 0x1;
	static const char POSITION_X_KEY = 0x2;
	static const char POSITION_Y_KEY = 0x3;
	static const char ORIENTATION_X_KEY = 0x4;
	static const char ORIENTATION_Y_KEY = 0x5;
	static const char HORIZONTAL_FOV_KEY = 0x6;
	static const char VERTICAL_FOV_KEY = 0x7;
	static const char ACQUISITION_TIME_KEY = 0x8;
	static const char SEQUENCE_NUMBER_KEY = 0x9;
	static const char MAX_RANGE_VALUE_KEY = 0xa;
	static const char VERTICAL_BIAS_KEY = 0xb;
	static const char HORIZONTAL_BIAS_KEY = 0xc;
	static const char SCANNER_MIRROR_THETA_KEY = 0xd;
	static const char SCANNER_MIRROR_R_KEY = 0xe;

  protected:
    Buffer<FLOAT> m_RangeValues;
    UINT m_Width;
    UINT m_Height;

    Vector2F m_ScannerPosition;
    Vector2F m_ScannerAxis;
    Matrix3F m_ScannerRotation;
    FLOAT m_HorizontalFieldOfView;
    FLOAT m_HorizontalBias;
    FLOAT m_VerticalFieldOfView;
    FLOAT m_VerticalBias;
    FLOAT m_MaximumRange;
    DateTime m_AquiredOn;
    UINT m_SequenceNumber;
	FLOAT m_MirrorTheta;
	FLOAT m_MirrorR;


  };
}

