/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/NASABot/Source/Trunk/Robot/Src/Test.cpp $
 * Author:       $Author: matthew $
 * Revision:     $Revision: 214 $
 * Last Updated: $Date: 2007-08-27 13:43:09 -0400 (Mon, 27 Aug 2007) $
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

#include "System/ConsoleApplication.hpp"
using Bolitho::System::ConsoleApplication;

#include "IO/File.hpp"
using Bolitho::IO::File;
using Bolitho::IO::FileMode;

#include "IO/TextStream.hpp"
using Bolitho::IO::TextStream;

#include "IO/StdIO.hpp"
using Bolitho::StdIn;

class TestApplication : public ConsoleApplication
{
public:
  INT Main();

};


ImplementApplicationEntryPoint(TestApplication);


using Bolitho::StdOut;

#include "RobotScanning.hpp"
#include "MotorEncoder.hpp"

#include "Graphics/Image.hpp"
using Bolitho::Graphics::Image;
using Bolitho::Graphics::Color;

#include "Math/Math.hpp"
using Bolitho::Math::PI;

struct FACE
{
  int V0;
  int V1;
  int V2;
};

struct COLOR
{
  BYTE r;
  BYTE g;
  BYTE b;
};

SIZE_T Index(SIZE_T i, SIZE_T j, SIZE_T w, SIZE_T h)
{
  Assert((i*w + j) < (w*h));
  return i*w + j;
}

using Bolitho::Math::Abs;
using Bolitho::Max;
using Bolitho::Min;

INT TestApplication::Main()
{
  Trace0Enter("");

  RangeImage RI;
  RI.Read(File::Open("D:\\Projects\\NASA\\Code\\Robot\\Logs\\Scan.1240.2", FileMode::OPEN));

  Image I;
  I.Resize(RI.Width(), RI.Height());

  FLOAT MinValue = MAXFLOAT;
  FLOAT MaxValue = MINFLOAT;
  for (SIZE_T i=0; i<RI.Height(); i++)
  {
    for (SIZE_T j=0; j<RI.Width(); j++)
    {
      if (RI(i,j) < MinValue)
        MinValue = RI(i,j);
      if (RI(i,j) > MaxValue)
        MaxValue = RI(i,j);
    }
  }

  for (SIZE_T i=0; i<RI.Height(); i++)
  {
    for (SIZE_T j=0; j<RI.Width(); j++)
    {
      FLOAT b = RI(i,j);
      b = (b-MinValue) / (MaxValue-MinValue);
      b *= 255.0f;
      I(i,j) = Color((BYTE)b, (BYTE)b, (BYTE)b);
    }
  }
    
  I.Save("D:\\Scan.bmp");

  Trace("%d %d", RI.Width(), RI.Height());

  Array<Vector3<FLOAT>> Points = RI.ConvertToPointset();
  Array<FACE> Faces;
  //Vertices.Resize((I.Width()-1)*(I.Height()-1)*2);

  for (SIZE_T i=0; i<RI.Height()-1; i++)
  {
    for (SIZE_T j=0; j<RI.Width()-1; j++)
    {
      FACE F;

      F.V1 = Index(i,j,RI.Width(), RI.Height());
      F.V0 = Index(i,j+1,RI.Width(), RI.Height());
      F.V2 = Index(i+1,j,RI.Width(), RI.Height());

      FLOAT MinD;
      FLOAT MaxD;

      MinD = Min(RI(i,j), RI(i,j+1), RI(i+1,j));
      MaxD = Max(RI(i,j), RI(i,j+1), RI(i+1,j));

      if ((MaxD-MinD) < 1000)
        Faces.Add(F);

      F.V1 = Index(i,j+1,RI.Width(), RI.Height());
      F.V0 = Index(i+1,j+1,RI.Width(), RI.Height());
      F.V2 = Index(i+1,j,RI.Width(), RI.Height());

      MinD = Min(RI(i,j+1), RI(i+1,j+1), RI(i+1,j));
      MaxD = Max(RI(i,j+1), RI(i+1,j+1), RI(i+1,j));

      if ((MaxD-MinD) < 1000)
        Faces.Add(F);

    }
  }

  Ptr<File> pFile = File::Open("D:\\Scan.ply", FileMode::CREATE);
  TextStream T(pFile);

  T.Write("ply\n");
  T.Write("format ascii 1.0\n");
  T.Write(String::Format("element vertex %d\n", Points.Length()));
  T.Write("property float x\n");
  T.Write("property float y\n");
  T.Write("property float z\n");
  /*T.Write("property uchar r\n");
  T.Write("property uchar g\n");
  T.Write("property uchar b\n");
  */
  T.Write(String::Format("element face %d\n", Faces.Length() ));
  T.Write("property list uchar int vertex_indices\n");
  T.Write("end_header\n");
  T.Write("\n");

  for (SIZE_T i=0; i<Points.Length(); i++)
    T.Write(String::Format("%f %f %f\n", Points[i][0], Points[i][1], Points[i][2] ));

  for (SIZE_T i=0; i<Faces.Length(); i++)
    T.Write(String::Format("3 %d %d %d\n", Faces[i].V0, Faces[i].V1, Faces[i].V2));
  
//  pFile->Write(&Points[0], sizeof(Vector3<FLOAT>) * Points.Length());
//  pFile->Write(&Vertices[0], sizeof(VERTEX) * Vertices.Length());

  T.Close();

  Trace0Return("%d",0);
  return 0;
}


/*
#include "LADAR.hpp"

VOID TestLADAR()
{
  Ptr<LADARController> pLADAR = new LADARController();
  pLADAR->Initialize("COM1", 38400);

  UINT i = 0; 
  while (TRUE)
  {
    Trace("Beginning Scan %d", i);
    RangeImage Scan = pLADAR->ReadRangeImage();
    Trace("Saving Scan %d", i);
    Scan.Write( File::Open(String::Format("%d.Scan", i++), FileMode::CREATE) );

    StdIn.ReadLine();
  }
}


#include "Graphics/Raster.hpp"
using Bolitho::Graphics::Raster;
using Bolitho::Graphics::Pen;
using Bolitho::Graphics::Brush;
using Bolitho::Graphics::PointStyle;
using Bolitho::PointF;

#include "Map.hpp"

class Grid
{
public:
  Grid()
  {
    m_pCells = new INT[32*32];
    ZeroMemory(m_pCells, sizeof(INT)*32*32);
  }

  INT* m_pCells;

  SIZE_T Width() CONST
  { return 32; }
  SIZE_T Height() CONST
  { return 32; }

  INT& operator()(SIZE_T x, SIZE_T y)
  { return m_pCells[x + 32*y]; }

};


struct Point
{
  SIZE_T m_Count;

  Point()
  {
    m_Count = 0;
  }

};

#include "Math/Geometry/Octree.hpp"
using Bolitho::Math::Geometry::Octree;
using Bolitho::Math::Geometry::OctreeSpatialIndex;
using Bolitho::TreeNode;
using Bolitho::Math::Vector3;

struct SplatFunction
{
  static BOOL NeedToSplit(TreeNode<Point>* pNode)
  {
    return pNode->NodeData.m_Count > 0;
  }

  static VOID Split(TreeNode<Point>* pNode, CONST Vector3<FLOAT>& ExtentMin, CONST Vector3<FLOAT>& ExtentMax)
  {
  }

  static VOID Splat(TreeNode<Point>* pNode,  CONST Vector3<FLOAT>& ExtentMin, CONST Vector3<FLOAT>& ExtentMax, CONST Vector3<FLOAT>& P)
  {
    pNode->NodeData.m_Count++;
  }

};

VOID TestOctree()
{
  OctreeSpatialIndex<4,SIZE_T> O( Vector3<FLOAT>(0,0,0), Vector3<FLOAT>(1,1,1) );

  Array<Vector3<FLOAT>> A;
  for (SIZE_T i=0; i<10; i++)
    A.Add(Vector3<FLOAT>::Random());

  for (SIZE_T i=0; i<A.Length(); i++)
    O.AddPoint(A[i], i);
}

VOID TestRaster()
{
  Grid M;
  Raster<Grid,INT> R(M);

  //R.DrawPoint(PointF(15,15), Pen<INT>(1,12.0f), PointStyle::ROUND); 

  R.FillTriangle( PointF(4,4), PointF(4,22), PointF(18,4), Brush<INT>(2) );

  for (SIZE_T i=0; i<32; i++)
  {
    for (SIZE_T j=0; j<32; j++)
    {
      printf("%1d ", M(i,j)); 
    }
    printf("\n");
  }
}
*/
