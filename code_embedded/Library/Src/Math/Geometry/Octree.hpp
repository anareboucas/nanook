/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Math/Geometry/Octree.hpp $
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





#pragma once

#include "Collections/Tree.hpp"
#include "Math/Vector3.hpp"

namespace Bolitho
{
  namespace Math
  {
    namespace Geometry
    {
      template<class NodeData, class Allocator=DefaultAllocator<TreeNode<NodeData>>, class ThreadingModel=SingleThreadedModel>
      class Octree : public Tree<8,NodeData,Allocator,ThreadingModel>
      {
        DeclareRuntimeType();
      public:
        Octree(Vector3<FLOAT> ExtentMinimum, Vector3<FLOAT> ExtentMaximum);
      
        /* Find the finest node that contains a given point.  
         * Returns NULL if no node contains the point
         */
        TreeNode<NodeData>* FindNode(Vector3<FLOAT> P);

        /* Finds/Creates nodes that contain a given point */
        template<class NodeSplitFunction>
        TreeNode<NodeData>* CreateNode(Vector3<FLOAT> P);
  
		/**
		 * Octants are arranged as
		 * 
		 *   2--3    y
		 *  /  /|    |
		 * 6--7 1    |__x
		 * |  |/    /
		 * 4--5    z
		 *
		 */
        static SIZE_T GetOctant(Vector3<FLOAT>& Min, Vector3<FLOAT>& Max, Vector3<FLOAT>& P);
		

      protected:
        Vector3<FLOAT> m_ExtentMin;
        Vector3<FLOAT> m_ExtentMax;


      };

      template<SIZE_T N, class PointData>
      struct OctreeSpatialIndexNode
      {
        OctreeSpatialIndexNode()
        { Count = 0; }

        SIZE_T Count;
        Vector3<FLOAT> Point[N];
        PointData Data[N];

        static BOOL NeedToSplit(TreeNode<OctreeSpatialIndexNode>* pNode);
        static VOID Split(TreeNode<OctreeSpatialIndexNode>* pNode, CONST Vector3<FLOAT>& ExtentMin, CONST Vector3<FLOAT>& ExtentMax);
        static VOID Splat(TreeNode<OctreeSpatialIndexNode>* pNode,  CONST Vector3<FLOAT>& ExtentMin, CONST Vector3<FLOAT>& ExtentMax, CONST Vector3<FLOAT>& P);

      };

      template<SIZE_T MaximumPointsPerNode, class PointData, class Allocator=DefaultAllocator<TreeNode<OctreeSpatialIndexNode<MaximumPointsPerNode,PointData>>>, class ThreadingModel=SingleThreadedModel>
      class OctreeSpatialIndex : public Octree<OctreeSpatialIndexNode<MaximumPointsPerNode,PointData>, Allocator, ThreadingModel>
      {
      public:
        OctreeSpatialIndex(Vector3<FLOAT> ExtentMin, Vector3<FLOAT> ExtentMax);

        /* Adds a point to the index */
        VOID AddPoint(Vector3<FLOAT> P, PointData& D);

        /* \brief Finds the closest point to a given location, up to a maximum distance.
         * \returns FALSE if no point is found
		 *
		 * Written by Zach Pezzementi
		 * 25 June 2008
         */
        BOOL FindClosest(Vector3<FLOAT> P, Vector3<FLOAT>& Point, PointData& Data, FLOAT MaxDistance);
		
	  protected:
	    /**
		 * \brief Helper function for the public FindClosest
		 *
		 * Written by Zach Pezzementi
		 * 25 June 2008
         */
		BOOL FindClosest(TreeNode<OctreeSpatialIndexNode<MaximumPointsPerNode,PointData>>* pNode, Vector3<FLOAT> P, Vector3<FLOAT>& Point, PointData& Data, FLOAT& MaxDistance);
		
		/**
		 * \brief A distance function for 2 3-vectors, currently Euclidean without the sqrt
		 *
		 * Written by Zach Pezzementi
		 * 25 June 2008
		 */
		FLOAT distance(Vector3<FLOAT> P, Vector3<FLOAT> Q);
		
		/**
		 * \brief A distance function for a 3-vector compared to the current node.
		 * 
		 * Computes the distance between P and the closest point on the sphere circumscribing this node, currently 
		 * using Euclidean distance without the sqrt.
		 *
		 * Written by Zach Pezzementi
		 * 25 June 2008
		 */
		FLOAT distance(TreeNode<OctreeSpatialIndexNode<MaximumPointsPerNode,PointData>>* qNode, Vector3<FLOAT> P);


      };

      #include "OctreeImpl.hpp"
    }
  }
}
