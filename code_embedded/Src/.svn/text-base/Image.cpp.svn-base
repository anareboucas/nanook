/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Image.cpp $
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

#include "Robot.hpp"
#include "Image.hpp"

using Bolitho::IO::BinaryStream;

//----------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(NASA,Image,Object);
//----------------------------------------------------------------------------------------------------------------------------
Image::Image()
{
  m_Width = 0;
  m_Height = 0;
  m_SequenceNumber = 0;
}
//----------------------------------------------------------------------------------------------------------------------------
Image::Image(UINT SequenceNumber, UINT Width, UINT Height)
{
  m_SequenceNumber = SequenceNumber;
  m_Width = Width;
  m_Height = Height;
  m_ColorValues.Resize(m_Width * m_Height);
}
//----------------------------------------------------------------------------------------------------------------------------
CONST Color& Image::operator() (UINT i, UINT j) CONST
{
  Assert(i < m_Height);
  Assert(j < m_Width);

  return m_ColorValues[i*m_Width + j];
}
//----------------------------------------------------------------------------------------------------------------------------
Color& Image::operator() (UINT i, UINT j)
{
  Assert(i < m_Height);
  Assert(j < m_Width);

  return m_ColorValues[i*m_Width + j];
}
//----------------------------------------------------------------------------------------------------------------------------
CONST Color* Image::operator[] (UINT i) CONST
{
  Assert(i < m_Height);
  
  return &m_ColorValues[i*m_Width];
}
//----------------------------------------------------------------------------------------------------------------------------
Color* Image::operator[] (UINT i)
{
  Assert(i < m_Height);
  
  return &m_ColorValues[i*m_Width];
}
//----------------------------------------------------------------------------------------------------------------------------
Color Image::Get(UINT i, UINT j) CONST
{
  Assert(i < m_Height);
  Assert(j < m_Width);

  return m_ColorValues[i*m_Width + j];
}
//----------------------------------------------------------------------------------------------------------------------------
VOID Image::Set(UINT i, UINT j, Color v)
{
  Assert(i < m_Height);
  Assert(j < m_Width);

  m_ColorValues[i*m_Width + j] = v;
}
//----------------------------------------------------------------------------------------------------------------------------
Color* Image::GetScanLine(UINT i)
{
  Assert(i < m_Height);
  
  return &m_ColorValues[i*m_Width];
}
//----------------------------------------------------------------------------------------------------------------------------
CONST Color* Image::GetScanLine(UINT i) CONST
{
  Assert(i < m_Height);
  
  return &m_ColorValues[i*m_Width];
}
//----------------------------------------------------------------------------------------------------------------------------
VOID Image::Read(Ptr<Stream> pStream)
{
  BinaryStream B(pStream);

  B >> m_Width;
  B >> m_Height;

  B >> m_CameraPosition;
  B >> m_CameraAxis;
  B >> m_HorizontalFieldOfView;
  B >> m_VerticalFieldOfView;
  B >> m_AquiredOn;
  B >> m_SequenceNumber;

  m_ColorValues.Resize(m_Width*m_Height);
  B.Read(&m_ColorValues[0], m_Width*m_Height*sizeof(Color));
}
//----------------------------------------------------------------------------------------------------------------------------
VOID Image::Write(Ptr<Stream> pStream)
{
  BinaryStream B(pStream);
  B << m_Width;
  B << m_Height;

  B << m_CameraPosition;
  B << m_CameraAxis;
  B << m_HorizontalFieldOfView;
  B << m_VerticalFieldOfView;
  B << m_AquiredOn;
  B << m_SequenceNumber;

  B.Write(&m_ColorValues[0], m_Width*m_Height*sizeof(Color));
}
//----------------------------------------------------------------------------------------------------------------------------

