/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/RangeImage.cpp $
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

#include "Robot.hpp"
#include "RangeImage.hpp"

#include "IO/BinaryStream.hpp"
using Bolitho::IO::BinaryStream;

#include "Math/Math.hpp"
using Bolitho::Math::Sin;
using Bolitho::Math::Cos;
using Bolitho::Math::DEGREESTORADIANS;

#include "Time/DateTime.hpp"
using Bolitho::DateTime;

using Bolitho::InvalidOperationException;

//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,RangeImage,Object);
//----------------------------------------------------------------------------------------------------------------------------
RangeImage::RangeImage()
{
  m_Width = 0;
  m_Height = 0;
  m_SequenceNumber = 0;

  m_HorizontalFieldOfView = 0.0f;
  m_HorizontalBias = 0.0f;
  m_VerticalFieldOfView = 0.0f;
  m_VerticalBias = 0.0f;
  m_MirrorTheta = 0;
  m_MirrorR = 0;

}
//----------------------------------------------------------------------------------------------------------------------------
RangeImage::RangeImage(UINT SequenceNumber, UINT Width, UINT Height)
{
  m_SequenceNumber = SequenceNumber;
  m_Width = Width;
  m_Height = Height;
  m_RangeValues.Resize(m_Width * m_Height);
  m_AquiredOn = DateTime::Now();

  m_HorizontalFieldOfView = 0.0f;
  m_HorizontalBias = 0.0f;
  m_VerticalFieldOfView = 0.0f;
  m_VerticalBias = 0.0f;
  m_MirrorTheta = 0;
  m_MirrorR = 0;
}
//----------------------------------------------------------------------------------------------------------------------------
CONST FLOAT& RangeImage::operator() (UINT i, UINT j) CONST
{
  Assert(i < m_Height);
  Assert(j < m_Width);

  return m_RangeValues[i*m_Width + m_Width-j-1];
}
//----------------------------------------------------------------------------------------------------------------------------
FLOAT& RangeImage::operator() (UINT i, UINT j)
{
  Assert(i < m_Height);
  Assert(j < m_Width);

  return m_RangeValues[i*m_Width + m_Width-j-1];
}
//----------------------------------------------------------------------------------------------------------------------------
CONST FLOAT* RangeImage::operator[] (UINT i) CONST
{
  Assert(i < m_Height);
  
  return &m_RangeValues[i*m_Width];
}
//----------------------------------------------------------------------------------------------------------------------------
FLOAT* RangeImage::operator[] (UINT i)
{
  Assert(i < m_Height);
  
  return &m_RangeValues[i*m_Width];
}
//----------------------------------------------------------------------------------------------------------------------------
FLOAT RangeImage::Get(UINT i, UINT j) CONST
{
  Assert(i < m_Height);
  Assert(j < m_Width);

  return m_RangeValues[i*m_Width + m_Width-j-1];
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RangeImage::Set(UINT i, UINT j, FLOAT v)
{
  Assert(i < m_Height);
  Assert(j < m_Width);

  m_RangeValues[i*m_Width + m_Width-j-1] = v;
}
//----------------------------------------------------------------------------------------------------------------------------
FLOAT* RangeImage::GetScanLine(UINT i)
{
  Assert(i < m_Height);
  
  return &m_RangeValues[i*m_Width];
}
//----------------------------------------------------------------------------------------------------------------------------
CONST FLOAT* RangeImage::GetScanLine(UINT i) CONST
{
  Assert(i < m_Height);
  
  return &m_RangeValues[i*m_Width];
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RangeImage::Read(Ptr<Stream> pStream)
{
  BinaryStream B(pStream);
  DWORD Version;

  B >> Version;
  if (Version == 0x0104){

  B >> m_Width;
  B >> m_Height;

  B >> m_ScannerPosition;
  B >> m_ScannerAxis;
  B >> m_HorizontalFieldOfView;
  B >> m_VerticalFieldOfView;
  B >> m_AquiredOn;
  B >> m_SequenceNumber;
  B >> m_MaximumRange;

  m_VerticalBias = 0;
  m_HorizontalBias = 0;
  m_MirrorTheta = 0;
  m_MirrorR = 0;


  m_RangeValues.Resize(m_Width*m_Height);
  B.Read(&m_RangeValues[0], m_Width*m_Height*sizeof(FLOAT));
	return;

  }
  if( Version == 0x0200)
  {
	DWORD length;
	B>>length;
	unsigned int bytesRead = 8;
	
	float temp;
	while(bytesRead<length){
		BYTE key;
		B>>key;
		bytesRead++;
		switch(key){
			case WIDTH_KEY:
				B>>m_Width;
				bytesRead+=4;
				break;
			case HEIGHT_KEY:
				B>>m_Height;
				bytesRead+=4;
				break;
			case POSITION_X_KEY:
				B>>temp;
				bytesRead+=4;
				m_ScannerPosition[0]=temp;
				break;
			case POSITION_Y_KEY:
				B>>temp;
				bytesRead+=4;
				m_ScannerPosition[1]=temp;
				break;
			case ORIENTATION_X_KEY:
				B>>temp;
				bytesRead+=4;
				m_ScannerAxis[0]=temp;
				break;
			case ORIENTATION_Y_KEY:
				B>>temp;
				bytesRead+=4;
				m_ScannerAxis[1]=temp;
				break;
			case HORIZONTAL_FOV_KEY:
				B>>m_HorizontalFieldOfView;
				bytesRead+=4;
				break;
			case VERTICAL_FOV_KEY:
				B>>m_VerticalFieldOfView;
				bytesRead+=4;
				break;
			case ACQUISITION_TIME_KEY:
				B>>m_AquiredOn;
				bytesRead+=8;
				break;
			case SEQUENCE_NUMBER_KEY:
				B>>m_SequenceNumber;
				bytesRead+=4;
				break;
			case MAX_RANGE_VALUE_KEY:
				B>>m_MaximumRange;
				bytesRead+=4;
				break;
			case VERTICAL_BIAS_KEY:
				B>>m_VerticalBias;
				bytesRead+=4;
				break;
			case HORIZONTAL_BIAS_KEY:
				B>>m_HorizontalBias;
				bytesRead+=4;
				break;
		
		}
		


	}


	m_RangeValues.Resize(m_Width*m_Height);
	B.Read(&m_RangeValues[0], m_Width*m_Height*sizeof(FLOAT));
	return;

  }


}
//----------------------------------------------------------------------------------------------------------------------------
VOID RangeImage::Write(Ptr<Stream> pStream)
{
  BinaryStream B(pStream);
  DWORD Version = 0x0200;

  B << Version;
  B << (DWORD)77;// number of bytes

  B << (BYTE) WIDTH_KEY;
  B << m_Width;
  B << (BYTE) HEIGHT_KEY;
  B << m_Height;
  B << (BYTE) POSITION_X_KEY;
  B << m_ScannerPosition.X();
  B << (BYTE) POSITION_Y_KEY;
  B << m_ScannerPosition.Y();

  B << (BYTE) ORIENTATION_X_KEY;
  B << m_ScannerAxis.X();
  B << (BYTE) ORIENTATION_Y_KEY;
  B << m_ScannerAxis.Y();
  B << (BYTE) HORIZONTAL_FOV_KEY;
  B << m_HorizontalFieldOfView;
  B << (BYTE) VERTICAL_FOV_KEY;
  B << m_VerticalFieldOfView;

  B << (BYTE) ACQUISITION_TIME_KEY;
  B << m_AquiredOn;
  B << (BYTE) SEQUENCE_NUMBER_KEY;
  B << m_SequenceNumber;
  B << (BYTE) MAX_RANGE_VALUE_KEY;
  B << m_MaximumRange;
  B << (BYTE) VERTICAL_BIAS_KEY;
  B << m_VerticalBias;

  B << (BYTE) HORIZONTAL_BIAS_KEY;
  B << m_HorizontalBias;

  B.Write(&m_RangeValues[0], m_Width*m_Height*sizeof(FLOAT));
}
//----------------------------------------------------------------------------------------------------------------------------
Vector3F RangeImage::ConvertToPoint(UINT i, UINT j) CONST
{
  FLOAT Theta = ((i - (m_Height * 0.5f)) / m_Height * m_VerticalFieldOfView + m_VerticalBias) * DEGREESTORADIANS;
  FLOAT Phi = ((j - (m_Width * 0.5f)) / m_Width * m_HorizontalFieldOfView + m_HorizontalBias) * DEGREESTORADIANS;
  FLOAT r = Get(i, j);

  Vector3F Point;

  Point[0] = Sin(Phi) * r;
  Point[1] = (r*Sin(Theta-m_MirrorTheta)+m_MirrorR* Sin(Theta)) * Cos(Phi);
  Point[2] = (r*Cos(Theta-m_MirrorTheta)+m_MirrorR* Cos(Theta)) * Cos(Phi);

  return Point;
}
//----------------------------------------------------------------------------------------------------------------------------
Array<Vector3F> RangeImage::ConvertToPointset(FLOAT MaxDistance) CONST
{
  Array<Vector3F> Points;
  Points.Resize(m_Width * m_Height);

  SIZE_T k = 0;
  for (UINT i = 0; i < m_Height; i++)
  {
    FLOAT Theta = ((i - (m_Height * 0.5f)) / m_Height * m_VerticalFieldOfView + m_VerticalBias) * DEGREESTORADIANS;
    for (UINT j = 0; j < m_Width; j++)
    {
      FLOAT Phi = ((j - (m_Width * 0.5f)) / m_Width * m_HorizontalFieldOfView + m_HorizontalBias) * DEGREESTORADIANS;
      FLOAT r = Get(i, j);

      if (r < MaxDistance)
      {
        Vector3F P;
        P[0] = Sin(Phi) * r;
		P[1] = (r*Sin(Theta-m_MirrorTheta)+m_MirrorR* Sin(Theta)) * Cos(Phi);
		P[2] = (r*Cos(Theta-m_MirrorTheta)+m_MirrorR* Cos(Theta)) * Cos(Phi);
       
        P = m_ScannerRotation * P;

        P[0] += m_ScannerPosition[0];
        P[2] -= m_ScannerPosition[1];

        Points[k] = P;

        k++;
      }
    }
  }

  return Points;
}
//----------------------------------------------------------------------------------------------------------------------------
VOID RangeImage::Splat(Ptr<Map> pMap, FLOAT MaximumHeight) CONST
{
  Trace0Enter("");
  UINT SplatCount = 0;

  Array<Vector3F> Points = ConvertToPointset(m_MaximumRange - 2.0f);

  TraceInfo(this, "Point Count: %d", Points.Length());

  for (UINT i = 0; i < Points.Length(); i++)
  {
    UINT MapCoordX;
    UINT MapCoordY;

    if (pMap->ComputeMapCell(Points[i].X(), -Points[i].Z(), MapCoordX, MapCoordY))
    {
      float V = pMap->GetCell(MapCoordX, MapCoordY).Height;

      if (Points[i].Y() > MaximumHeight)
        continue;

      SplatCount++;

      if (Points[i].Y() > V || pMap->GetCell(MapCoordX, MapCoordY).Flags == 0)
        pMap->SetCell(MapCoordX, MapCoordY, MapCell(Points[i].Y(), 1));
      
      if (Points[i].Y() > pMap->MaximumHeightValue())
        pMap->MaximumHeightValue() = Points[i].Y();
      if (Points[i].Y() < pMap->MinimumHeightValue())
        pMap->MinimumHeightValue() = Points[i].Y();

    }
  }

  TraceInfo(this, "Splat Count: %d", SplatCount);
}
