/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Graphics/MeshRender.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1168 $
 * Last Updated: $Date: 2007-09-29 11:47:34 -0400 (Sat, 29 Sep 2007) $
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
#include "Debug/Trace.hpp"
#include "Graphics/OpenGL/Context.hpp"
#include "Math/Geometry/Types.hpp"

using namespace Bolitho;
using namespace Bolitho::Math;
using namespace Bolitho::Math::Geometry;
using namespace Bolitho::Graphics;
using namespace Bolitho::Graphics::OpenGL;


//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Image> Mesh::Render( UINT Width, UINT Height, CONST Math::Matrix4<FLOAT> Viewpoint )
{
	Ptr<Context> pContext = new Context( 32, 8, 24, 0 );
	Ptr<FrameBuffer> pFrameBuffer = pContext->CreateFrameBuffer();
	Ptr<RenderBuffer> pColorBuffer = pContext->CreateRenderBuffer( GL_RGBA, Width, Height );
	Ptr<RenderBuffer> pDepthBuffer = pContext->CreateRenderBuffer( GL_DEPTH_COMPONENT24, Width, Height );

	pContext->SetClearColor( ColorF::FromName(TEXT("White")) );
	pContext->SetClearDepth( 1.0f );

	pContext->SetGlobalAmbient( ColorF( 0.02f, 0.02f, 0.05f, 0.05f ) );

	pContext->Light0.SetAmbient( ColorF( 0.0f, 0.0f, 0.0f, 0.0f ) );
	pContext->Light0.SetDiffuse( ColorF( 0.85f, 0.85f, 0.8f, 0.85f ) );
	pContext->Light0.SetSpecular( ColorF( 0.85f, 0.85f, 0.8f, 0.85f ) );

	pContext->EnableLighting();
	pContext->Light0.Enable();

	pContext->Material.SetAmbient( GL_FRONT_AND_BACK, ColorF(1,1,1,1) );
	pContext->Material.SetDiffuse( GL_FRONT_AND_BACK, ColorF(1,1,1,1) );
	pContext->Material.SetSpecular( GL_FRONT_AND_BACK, ColorF(0.18f, 0.18f, 0.18f, 0.18f) );
	pContext->Material.SetShininess( GL_FRONT_AND_BACK, 64 );
	
	pContext->EnableDepthTest();

	pFrameBuffer->Bind();
	pFrameBuffer->Attach( GL_DEPTH_ATTACHMENT_EXT, pDepthBuffer );
	pFrameBuffer->Attach( GL_COLOR_ATTACHMENT0_EXT, pColorBuffer );

	glViewport( 0, 0, Width, Height );

	pContext->Clear();

	glPushMatrix();
	glMultMatrixf( Viewpoint.Transpose() );

	UINT PositionOffset = VertexData.GetAttributeOffset(VertexAttribute::POSITION3D);
	UINT NormalOffset = VertexData.GetAttributeOffset(VertexAttribute::NORMAL3D);
	
	for (UINT i=0; i<TriangleData.GetBlockCount(); i++)
	{
    Mesh::TRIANGLEBLOCKINFO* pTriangleInfo = TriangleData.GetBlockHeader(i);
		UINT MinVertex = pTriangleInfo->MinVertexBlock;
		UINT MaxVertex = pTriangleInfo->MaxVertexBlock;

	  INDEXEDTRIANGLE* pTriangleData = (INDEXEDTRIANGLE*)TriangleData.GetBlockData(i);

    glBegin(GL_TRIANGLES);
    for (UINT j=0; j<TriangleData.GetBlockRowCount(i); j++)
    {
      Vector3<FLOAT>& V0 = VertexData.GetAttribute<Vector3<FLOAT>>(pTriangleData[j].Vertex[0], PositionOffset);
      Vector3<FLOAT>& V1 = VertexData.GetAttribute<Vector3<FLOAT>>(pTriangleData[j].Vertex[1], PositionOffset);
      Vector3<FLOAT>& V2 = VertexData.GetAttribute<Vector3<FLOAT>>(pTriangleData[j].Vertex[2], PositionOffset);

      Vector3<FLOAT>& N0 = VertexData.GetAttribute<Vector3<FLOAT>>(pTriangleData[j].Vertex[0], NormalOffset);
      Vector3<FLOAT>& N1 = VertexData.GetAttribute<Vector3<FLOAT>>(pTriangleData[j].Vertex[1], NormalOffset);
      Vector3<FLOAT>& N2 = VertexData.GetAttribute<Vector3<FLOAT>>(pTriangleData[j].Vertex[2], NormalOffset);

      glNormal3fv(N0);
      glVertex3fv(V0);
      glNormal3fv(N1);
      glVertex3fv(V1);
      glNormal3fv(N2);
      glVertex3fv(V2);
    }
	  glEnd();

    TriangleData.Unlock(i,i+1);
		VertexData.Unlock(MinVertex, MaxVertex);
  }

	glPopMatrix();

  Ptr<Image> pImage = pContext->ReadFrameBuffer();
	pFrameBuffer->Unbind();

	return pImage;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
