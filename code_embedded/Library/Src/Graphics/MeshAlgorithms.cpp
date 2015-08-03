/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Graphics/MeshAlgorithms.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1305 $
 * Last Updated: $Date: 2007-11-28 12:13:51 -0500 (Wed, 28 Nov 2007) $
 * 
 * 
 * Copyright (c) 2004-2007, Matthew G Bolitho
 * All rights reserved.
 *
 *
 * Microsoft Reference License (Ms-RL)
 *   
 * This license governs use of the accompanying software. If you use the software, you accept 
 * this license. If you do not accept the license, do not use the software.
 *
 * 1. Definitions
 *  - The terms "reproduce," "reproduction" and "distribution" have the same meaning here as under 
 *    U.S. copyright law.
 *  - "You" means the licensee of the software.
 *  - "Your company" means the company you worked for when you downloaded the software.
 *  - "Reference use" means use of the software within your company as a reference, in read only 
 *    form, for the sole purposes of debugging your products, maintaining your products, or 
 *    enhancing the interoperability of your products with the software, and specifically excludes 
 *    the right to distribute the software outside of your company.
 *  - "Licensed patents" means any Licensor patent claims which read directly on the software as 
 *    distributed by the Licensor under this license.
 * 
 * 2. Grant of Rights
 *  (A) Copyright Grant- Subject to the terms of this license, the Licensor grants you a non-transferable, 
 *      non-exclusive, worldwide, royalty-free copyright license to reproduce the software for reference use.
 *  (B) Patent Grant- Subject to the terms of this license, the Licensor grants you a non-transferable,
 *      non-exclusive, worldwide, royalty-free patent license under licensed patents for reference use.
 * 
 * 3. Limitations
 *  (A) No Trademark License - This license does not grant you any rights to use the Licensor's name
 *      logo, or trademarks.
 *  (B) If you begin patent litigation against the Licensor over patents that you think may apply 
 *      to the software (including a cross-claim or counterclaim in a lawsuit), your license to the 
 *      software ends automatically.
 *  (C) The software is licensed "as-is." You bear the risk of using it. The Licensor gives no express 
 *      warranties, guarantees or conditions. You may have additional consumer rights under your local 
 *      laws which this license cannot change. To the extent permitted under your local laws, the 
 *      Licensor excludes the implied warranties of merchantability, fitness for a particular purpose 
 *      and non-infringement.
 *
 ***************************************************************************************************/

#include "CodeLibrary.hpp"

#include "Graphics/Mesh.hpp"
#include "IO/Path.hpp"
#include "Utility/Flags.hpp"
#include "Debug/Trace.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Math::Geometry;
using namespace Bolitho::Graphics;
using namespace Bolitho::IO;

//----------------------------------------------------------------------------------------------------------------------------------------------------
#include "Miniball.hpp"
VOID UpdateMiniBall3D( ULONGLONG i, Miniball<3>& MiniBall, Vector3F& P )
{
  Point<3> MbP;
  MbP[0] = P[0];
  MbP[1] = P[1];
  MbP[2] = P[2];

  MiniBall.check_in( MbP );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Sphere3<FLOAT> Mesh::ComputeBoundingSphere( BOOL ForceRecompute, IProgress* pProgress )
{
  AxisAlignedBox3<FLOAT> BB = ComputeBoundingBox( ForceRecompute );
  
  m_BoundingSphere.Center() = BB.Center();
  m_BoundingSphere.Radius() = 0.5f * BB.Diagonal().Length();

  return m_BoundingSphere;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID UpdateBoundingBox(UINT i, Vector3F& P, AxisAlignedBox3<FLOAT>& B)
{
  if (P[0] < B.Min()[0])
    B.Min()[0] = P[0];
  if (P[1] < B.Min()[1])
    B.Min()[1] = P[1];
  if (P[2] < B.Min()[2])
    B.Min()[2] = P[2];

  if (P[0] > B.Max()[0])
    B.Max()[0] = P[0];
  if (P[1] > B.Max()[1])
    B.Max()[1] = P[1];
  if (P[2] > B.Max()[2])
    B.Max()[2] = P[2];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
AxisAlignedBox3<FLOAT> Mesh::ComputeBoundingBox( BOOL ForceRecompute, IProgress* pProgress )
{
  if (m_BoundingBoxStatus > 0 && !ForceRecompute)
    return m_BoundingBox;
  
  m_BoundingBox.Min() = Vector3F(  RealTraits<FLOAT>::MaxValue,  RealTraits<FLOAT>::MaxValue,  RealTraits<FLOAT>::MaxValue );
  m_BoundingBox.Max() = Vector3F( -RealTraits<FLOAT>::MaxValue, -RealTraits<FLOAT>::MaxValue, -RealTraits<FLOAT>::MaxValue );
  
  VertexData.Iterate<Vector3F>(UpdateBoundingBox, VertexAttribute::POSITION3, m_BoundingBox);

  return m_BoundingBox;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID UpdateCenterOfMass(UINT i, Vector3F& Position, Vector3D& CenterOfMass)
{
	CenterOfMass += Vector3<DOUBLE>(Position(0), Position(1), Position(2));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Math::Vector3<FLOAT> Mesh::ComputeCenterOfMass( BOOL ForceRecompute, IProgress* pProgress )
{
	if (m_CenterOfMassStatus > 0 && !ForceRecompute)
		return m_CenterOfMass;

	Vector3<DOUBLE> CenterOfMass;
	VertexData.Iterate<Vector3F>(UpdateCenterOfMass, VertexAttribute::POSITION3, CenterOfMass);
	CenterOfMass /= (DOUBLE)VertexData.GetCount();

  m_CenterOfMass = CenterOfMass;

	return m_CenterOfMass;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID UpdateCovariance(UINT i, Vector3F& P, Matrix3D& Covariance, Vector3F& CenterOfMass)
{
	Vector3<DOUBLE> Position = P - CenterOfMass;
	Covariance(0,0) += Position(0)*Position(0);
	Covariance(0,1) += Position(0)*Position(1);
	Covariance(0,2) += Position(0)*Position(2);
	Covariance(1,0) += Position(1)*Position(0);
	Covariance(1,1) += Position(1)*Position(1);
	Covariance(1,2) += Position(1)*Position(2);
	Covariance(2,0) += Position(2)*Position(0);
	Covariance(2,1) += Position(2)*Position(1);
	Covariance(2,2) += Position(2)*Position(2);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Math::Matrix3<FLOAT> Mesh::ComputeCovariance( BOOL ForceRecompute, IProgress* pProgress )
{
	if (m_CovarianceStatus > 0 && !ForceRecompute)
		return m_Covariance;
	ComputeCenterOfMass(ForceRecompute);

	Matrix3<DOUBLE> Covariance;
  Covariance.SetZero();
	VertexData.Iterate<Vector3F>(UpdateCovariance, VertexAttribute::POSITION3, Covariance, m_CenterOfMass);
  Covariance /= (DOUBLE)VertexData.GetCount();
  
  m_Covariance = Covariance;
	return m_Covariance;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID FlipTriangle( ULONGLONG i, INDEXEDTRIANGLE& T )
{
	UINT Temp = T.Vertex[1];
	T.Vertex[1] = T.Vertex[2];
	T.Vertex[2] = Temp;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::FlipTriangles(IProgress* pProgress )
{
  /*
	if (pProgress)
    pProgress->BeginStep(TEXT("Flipping Triangles"), 0, TriangleData.GetBlockCount());

	//TriangleData.Iterate( CreateFuncPtr(&FlipTriangle), TriangleAttribute::VERTICES );

	if (pProgress)
		pProgress->EndStep();
    */
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
UINT Mesh::RemoveLargeTriangles( FLOAT LargestArea, IProgress* pProgress )
{
  /*
  UINT Count = 0;

  if (pProgress)
    pProgress->BeginStep(TEXT("Removing Triangles"), 0, TriangleData.GetBlockCount() );

	UINT VertexPositionOffset = VertexData.GetAttributeOffset(VertexAttribute::POSITION3D);

	UINT MinVertexBlock = 0;
	UINT MaxVertexBlock = 0;
	INDEXEDTRIANGLE LastGood;

  for (UINT i=0; i<TriangleData.GetBlockCount(); i++)
	{
    DataTable<TRIANGLEBLOCKINFO,TRIANGLETABLEINFO>::BLOCKHEADER* pHeader = TriangleData.GetBlockHeader(i);

		if (pProgress)
      pProgress->UpdateStep(i);

    MinVertexBlock = pHeader->MinVertexBlock;
		MaxVertexBlock = pHeader->MaxVertexBlock;
		
    for (UINT j=0; j<pHeader->Rows; j++)
		{
			INDEXEDTRIANGLE& T = TriangleData.GetRow<INDEXEDTRIANGLE>(i,j);
						
			Vector3<FLOAT>& V0 = VertexData.GetAttribute<Vector3<FLOAT>>(T.Vertex[0], VertexPositionOffset);
			Vector3<FLOAT>& V1 = VertexData.GetAttribute<Vector3<FLOAT>>(T.Vertex[1], VertexPositionOffset);
			Vector3<FLOAT>& V2 = VertexData.GetAttribute<Vector3<FLOAT>>(T.Vertex[2], VertexPositionOffset);

			Vector3<FLOAT> A = V0-V1, B = V1-V2, C = V2-V0;
			
			Vector3<FLOAT> Normal0 = A.Cross(B);
			Vector3<FLOAT> Normal1 = B.Cross(C);
			Vector3<FLOAT> Normal2 = C.Cross(A);

			FLOAT Area = (Normal0.Length() + Normal1.Length() + Normal2.Length()) / 3.0f;
			
			if (Area > LargestArea)
			{
				T = LastGood;
				T.Vertex[1] = T.Vertex[0];
				T.Vertex[2] = T.Vertex[0];
        Count++;	
      }
			else
			{
				LastGood = T;
			}

		}
	
		TriangleData.Unlock(i,i+1);
		VertexData.Unlock(MinVertexBlock, MaxVertexBlock);
	}

  if (pProgress)
    pProgress->EndStep();

	//BuildBlockInformation(pProgress);

  return Count;
  */
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Histogram1<FLOAT> Mesh::ComputeTriangleAreaDistribution(UINT BinCount, IProgress* pProgress)
{
  /*
	Histogram<FLOAT> H = ComputeTriangleAreaDistribution(BinCount, 0, 0, pProgress);
	return ComputeTriangleAreaDistribution(BinCount, H.MinimumValue(), H.MaximumValue(), pProgress);
  */
  return Histogram1<FLOAT>(0, 0, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Histogram1<FLOAT> Mesh::ComputeTriangleAreaDistribution(UINT BinCount, FLOAT Min, FLOAT Max, IProgress* pProgress)
{
/*
  Histogram<FLOAT> H(BinCount, 1, Min, Max);

  if (pProgress)
    pProgress->BeginStep(TEXT("Computing Triangle Area Distribution"), 0, TriangleData.GetBlockCount());

	UINT VertexPositionOffset = VertexData.GetAttributeOffset(VertexAttribute::POSITION3D);

	UINT MinVertexBlock = 0;
	UINT MaxVertexBlock = 0;

	for (UINT i=0; i<TriangleData.GetBlockCount(); i++)
	{
    DataTable<TRIANGLEBLOCKINFO,TRIANGLETABLEINFO>::BLOCKHEADER* pHeader = TriangleData.GetBlockHeader(i);

		if (pProgress)
      pProgress->UpdateStep(i);

    MinVertexBlock = pHeader->MinVertexBlock;
    MaxVertexBlock = pHeader->MaxVertexBlock;
		
    for (UINT j=0; j<pHeader->Rows; j++)
		{
			INDEXEDTRIANGLE& T = TriangleData.GetRow<INDEXEDTRIANGLE>(i,j);
						
			Vector3<FLOAT>& V0 = VertexData.GetAttribute<Vector3<FLOAT>>(T.Vertex[0], VertexPositionOffset);
			Vector3<FLOAT>& V1 = VertexData.GetAttribute<Vector3<FLOAT>>(T.Vertex[1], VertexPositionOffset);
			Vector3<FLOAT>& V2 = VertexData.GetAttribute<Vector3<FLOAT>>(T.Vertex[2], VertexPositionOffset);

			Vector3<FLOAT> A = V0-V1, B = V1-V2, C = V2-V0;
			
			Vector3<FLOAT> Normal0 = A.Cross(B);
			Vector3<FLOAT> Normal1 = B.Cross(C);
			Vector3<FLOAT> Normal2 = C.Cross(A);

			FLOAT Area = (Normal0.Length() + Normal1.Length() + Normal2.Length()) / 3.0f;
			H.Add(Area);
		}
	
		TriangleData.Unlock(i,i+1);
		VertexData.Unlock(MinVertexBlock, MaxVertexBlock);
	}

  if (pProgress)
    pProgress->EndStep();

	return H;
*/
  return Histogram1<FLOAT>(0, 0, 1);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Mesh::ComputeNormals(IProgress* pProgress)
{
  if (pProgress)
    pProgress->BeginTask("Computing Normals");

  if (!VertexData.HasAttribute(VertexAttribute::NORMAL3))
  {
    VertexData.AddAttribute(VertexAttribute::NORMAL3, pProgress);
  }
  else
  {
	  UINT VertexNormalOffset = VertexData.GetAttributeOffset(VertexAttribute::NORMAL3);

    if (pProgress)
      pProgress->BeginStep("Clearing Normals", 0, VertexData.GetCount());

    VertexData.MapView(PageAccess::READWRITE);
    for (UINT i=0; i<VertexData.GetCount(); i++)
    {
      Vector3F& V = VertexData.Attribute<Vector3F>(i, VertexNormalOffset);
      V.SetZero();
    }
    VertexData.Unmap();

    if (pProgress)
      pProgress->EndStep();
  }

	UINT VertexPositionOffset = VertexData.GetAttributeOffset(VertexAttribute::POSITION3);
	UINT VertexNormalOffset = VertexData.GetAttributeOffset(VertexAttribute::NORMAL3);
  UINT TriangleVerticesOffset = TriangleData.GetAttributeOffset(TriangleAttribute::VERTICES);

  TriangleData.MapView(PageAccess::READ);
  VertexData.MapView(PageAccess::READWRITE);
	for (UINT i=0; i<TriangleData.GetCount(); i++)
	{
    INDEXEDTRIANGLE& T = TriangleData.Attribute<INDEXEDTRIANGLE>(i, TriangleVerticesOffset);

    Vector3F& V0 = VertexData.Attribute<Vector3F>(T.Vertex[0], VertexPositionOffset);
    Vector3F& V1 = VertexData.Attribute<Vector3F>(T.Vertex[1], VertexPositionOffset);
    Vector3F& V2 = VertexData.Attribute<Vector3F>(T.Vertex[2], VertexPositionOffset);

    Vector3F A = V0-V1, B = V1-V2, C = V2-V0;

    Vector3F Normal0 = A.Cross(B);
    Vector3F Normal1 = B.Cross(C);
    Vector3F Normal2 = C.Cross(A);

    if (Normal0.Length() < 1E-10 || Normal1.Length() < 1E-10 || Normal2.Length() < 1E-10)
      continue;

    FLOAT Area = (Normal0.Length() + Normal1.Length() + Normal2.Length()) / 3.0f;

    Vector3<FLOAT> Normal = Normal0 + Normal1 + Normal2;
    Normal.Normalize();

    VertexData.Attribute<Vector3F>(T.Vertex[0], VertexNormalOffset) += (Normal * Area);
    VertexData.Attribute<Vector3F>(T.Vertex[1], VertexNormalOffset) += (Normal * Area);
    VertexData.Attribute<Vector3F>(T.Vertex[2], VertexNormalOffset) += (Normal * Area);
  }
  TriangleData.Unmap();

	for (UINT i=0; i<VertexData.GetCount(); i++)
  {
    Vector3F& V = VertexData.Attribute<Vector3F>(i, VertexNormalOffset);
    V.Normalize();    
  }
  VertexData.Unmap();

  if (pProgress)
    pProgress->EndStep();

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
