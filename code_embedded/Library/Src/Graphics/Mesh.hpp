/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Graphics/Mesh.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1313 $
 * Last Updated: $Date: 2007-12-02 17:54:01 -0500 (Sun, 02 Dec 2007) $
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

#include "IO/File.hpp"
#include "IO/MemoryStream.hpp"
#include "Collections/Map.hpp"

#include "Utility/FuncPtr.hpp"
#include "Utility/Progress.hpp"
#include "Utility/ElementDecl.hpp"

#include "Math/AxisAlignedBox.hpp"
#include "Math/Sphere.hpp"
#include "Math/Line.hpp"
#include "Math/Matrix3.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Histogram.hpp"
#include "Math/Geometry/Types.hpp"
#include "Lua/Binding.hpp"
#include "Graphics/Image.hpp"

namespace Bolitho
{
  namespace Graphics
  {
    class LIBRARY_API MeshException : public UserException
    {
      DeclareRuntimeType();
    public:
      MeshException( CONST Object* Sender, LPCTSTR Description ) : UserException(Sender)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(Description);
      }

      MeshException( CONST MeshException& E ) : UserException(E)
      {
        m_Description = CharTraits<TCHAR>::Duplicate(E.m_Description);
      }

      virtual ~MeshException()
      {
        if (m_Description)
          FreeMemory(m_Description);
      }

      virtual String Description() CONST
      {
        return m_Description;
      }

    private:
      TCHAR* m_Description;
    };

    
    struct VertexAttribute
    {
      /* Vertex Attributes */
      static const ElementDecl POSITION3;
      static const ElementDecl POSITION2;
      static const ElementDecl NORMAL2;
      static const ElementDecl NORMAL3;
      static const ElementDecl DIFFUSE;
    };

    struct TriangleAttribute
    {
      /* Triangle Attributes */
      static const ElementDecl VERTICES;
    };

    /* A simple geometric mesh */
    class LIBRARY_API Mesh : public Object
    {
      DeclareRuntimeType();
    protected:
      static CONST UINT32 MESHVERSION = 0x0211;
      static CONST UINT64 MESHSIGNATURE = 0x202020204853454d;
      
      struct ELEMENTDECL
      {
        CHAR Name[32];
        DWORD Type;
        UINT Length;
      };

      struct TABLEHEADER
      {
        UINT Extent;
        UINT Count;
        UINT AllocatedCount;
        UINT ElementSize;
        UINT AttributeCount;
        ELEMENTDECL ElementDecl[32];
      };

      struct EXTENT
      {
        ULONGLONG Offset;
        ULONGLONG Length;
        DWORD Flags;

        static CONST DWORD FREE = 0;
        static CONST DWORD RESERVED = 0x80000000;
        static CONST DWORD END = 0xffffffff;
        static CONST DWORD USED = 1;
      };

      struct MESHHEADER
      {
        UINT64 Signature;        
        UINT32 Version;

        TABLEHEADER VertexDataDecl;
        TABLEHEADER TriangleDataDecl;

        EXTENT Extents[64];
        
        UINT MetaDataExtent;
        UINT SpatialIndexExtent;
      };

      struct SPATIALINDEXNODE
      {
        UINT VertexDataOffset;
        UINT VertexDataCount;
      };

      class LIBRARY_API DataTable
      {
        friend class Mesh;
      public:
        /* Maps the table data into memory */
        VOID MapView(DWORD PageAccess);

        /* Unmaps the table data from memory */
        VOID Unmap(BOOL Force = FALSE);

        /* Locks a segment of the table data into memory */
        VOID Lock(UINT i, UINT Length);

        /* Unlocks a segment of the table data */
        VOID Unlock(UINT i, UINT Length);

        /* Determines if the table is currently mapped */
        BOOL IsMapped() CONST
        { return m_MappingFlags != 0; }

        /* Bulk loads the table */
        VOID Load(Ptr<IO::Stream> pSource, UINT TableOffset, UINT Length);

        /* Bulk dumps the table */
        VOID Dump(Ptr<IO::Stream> pDestination, UINT TableOffset, UINT Length);


        /* Returns TRUE if the table contains a specific attribute */
        BOOL HasAttribute(CONST ElementDecl& A) CONST;
        
        /* Returns the offset of an attribute within the table, or -1 if the attribute does not exist */
        UINT GetAttributeOffset(CONST ElementDecl& A) CONST;
        
        /* Returns the index of an attribute in the table, or -1 if the attribute does not exist */
        UINT GetAttributeIndex(CONST ElementDecl& A) CONST;
        
        /* Gets the number of attributes */
        UINT GetAttributeCount() CONST
        { return m_pHeader->AttributeCount; }

        /* Adds an attribute to the table.  
         * If there is free space within the element, the space will be allocated to the new attribute
         * If there is no free space within the element, the table is resized and free space is added to each element */
        VOID AddAttribute(CONST ElementDecl& A, IProgress* pProgress = 0);
        
        /* Removes an attribute from the table */
        VOID RemoveAttribute(CONST ElementDecl& A);
        
        /* Removes all attributes from the table */
        VOID RemoveAllAttributes();
        
        /* Sets the complete element declaration */
        VOID SetElementDecl(CONST Array<ElementDecl>& Decl, IProgress* pProgress = 0);

        /* Gets the i-th attribute declaration */
        CONST ElementDecl& GetAttribute(UINT i);

        /* Gets all attribute declarations */
        CONST Array<ElementDecl>& GetAttributes() CONST;

        /* Returns the number of rows in the table */
        UINT GetCount() CONST;
        /* Returns the size of the table, in bytes */
        ULONGLONG GetSize() CONST;
        /* Returns the size of an element, in bytes */
        UINT GetElementSize() CONST;

        /* Resizes the table */
        VOID Resize(UINT Count);

        /* Returns a pointer to the table data */
        LPVOID GetTableData()
        { return m_pData; }
        /* Returns a pointer to the table data */
        LPCVOID GetTableData() CONST
        { return m_pData; }

        /* Returns a pointer to the i-th row in the table */
        template<class T>
        T* Row(UINT i);
        /* Returns a pointer to the i-th row in the table */
        template<class T>
        CONST T* Row(UINT i) CONST;

        /* Returns a reference to an attribute in a given row */
        template<class T>
        T& Attribute(UINT Index, UINT Offset);
        /* Returns a reference to an attribute in a given row */
        template<class T>
        CONST T& Attribute(UINT Index, UINT Offset) CONST;

        /* Iterate table data */
        template<class T, class Func>
        VOID Iterate(Func F);
        template<class T, class Func, class U>
        VOID Iterate(Func F, U& P0);
        template<class T, class Func, class U, class V>
        VOID Iterate(Func F, U& P0, V& P1);
        
        /* Iterate a table attribute */
        template<class T, class Func>
        VOID Iterate(Func F, CONST ElementDecl& Attribute);
        template<class T, class Func, class U>
        VOID Iterate(Func F, CONST ElementDecl& Attribute, U& P0);
        template<class T, class Func, class U, class V>
        VOID Iterate(Func F, CONST ElementDecl& Attribute, U& P0, V& P1);
        template<class T, class Func>
        VOID Iterate(Func F, UINT Offset);
        template<class T, class Func, class U>
        VOID Iterate(Func F, UINT Offset, U& P0);
        template<class T, class Func, class U, class V>
        VOID Iterate(Func F, UINT Offset, U& P0, V& P1);

      protected:
        DataTable(Mesh& M);
        VOID Initialize(TABLEHEADER* pHeader, EXTENT* pExtent);

        Mesh& m_Mesh;
        
        Ptr<IO::FileView> m_pMappedView;
        DWORD m_MappingFlags;

        LPVOID m_pData;
        TABLEHEADER* m_pHeader;
        EXTENT* m_pExtent;
        Array<ElementDecl> m_Declaration;
      };
    public:
      virtual ~Mesh();

      /* Create an in-memory mesh */
      Mesh();
      /* Create/Open a persistant mesh */
      Mesh(LPCTSTR Filename, DWORD FileMode);

      /* Import a mesh from another format.  
       * The format is determined by filename extension 
       */
      VOID Import(LPCTSTR Filename, IProgress* pProgress = 0);

      /* Export the mesh to another format.  
       * The format is determined by filename extension 
       */
      VOID Export(LPCTSTR Filename, IProgress* pProgress = 0);

      /* Flushes all dirty data to disk */
      VOID Flush();

      /* Frees all memory used to store the mesh */
      VOID Free();

      /* The data tables */
      DataTable VertexData;
      DataTable TriangleData;

      /* Adds a metadata value to the mesh */
      VOID SetMetaData(LPCTSTR Key, LPCTSTR Value);

      /* Gets a metadata value */
      String GetMetaData(LPCTSTR Key);

      /* Removes a metadata value */
      VOID RemoveMetaData(LPCTSTR Key);

      /* Removes all metadata */
      VOID RemoveAllMetaData();

      /* Computes the bounding sphere of the mesh 
       * 'ForceRecompute' will not use an estimated value from the cache
       */
      Math::Sphere3<FLOAT> ComputeBoundingSphere( BOOL ForceRecompute = FALSE, IProgress* pProgress = 0  );

      /* Computes the bounding box of the mesh 
       * 'ForceRecompute' will not use an estimated value from the cache
       */
      Math::AxisAlignedBox3<FLOAT> ComputeBoundingBox( BOOL ForceRecompute = FALSE, IProgress* pProgress = 0  );

      /* Computes the center of mass of the mesh 
       * 'ForceRecompute' will not use an estimated value from the cache
       */
      Math::Vector3<FLOAT> ComputeCenterOfMass( BOOL ForceRecompute = FALSE, IProgress* pProgress = 0  );

      /* Computes the covariance matrix of the mesh vertices
       * 'ForceRecompute' will not use an estimated value from the cache
       */
      Math::Matrix3<FLOAT> ComputeCovariance( BOOL ForceRecompute = FALSE, IProgress* pProgress = 0  );

      /* Computes normals for a mesh */
      VOID ComputeNormals(IProgress* pProgress = 0 );

      /* Constructs the spatial index */
      VOID ConstructSpatialIndex(UINT TargetIndexPageSize);
      
      /* Flips the winding order of all triangles */
      VOID FlipTriangles(IProgress* pProgress = 0 );

      /* Computes the distribution of triangle area */
      Math::Histogram1<FLOAT> ComputeTriangleAreaDistribution(UINT BinCount, IProgress* pProgress = 0);
      Math::Histogram1<FLOAT> ComputeTriangleAreaDistribution(UINT BinCount, FLOAT Min, FLOAT Max, IProgress* pProgress = 0);

      /* Removes triangles larger than a certain area */
      UINT RemoveLargeTriangles( FLOAT Area, IProgress* pProgress = 0 );

      /* Finds the closest point on the mesh to another point in space */
      Math::Vector3<FLOAT> FindClosestPoint( CONST Math::Vector3<FLOAT> P );

      /* Intersects a ray with the mesh */
      FLOAT FindFirstIntersection( CONST Math::Ray3<FLOAT> R );

      /* Renders an image of the Mesh */
      Image Render( UINT Width, UINT Height, CONST Math::Matrix4<FLOAT> Viewpoint );

      /* Generate Lua bindings for the Mesh class */
      #if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings( LPCTSTR Name );
      #endif

    protected:
      /* The Mesh file */
      Ptr<IO::File> m_pFile;
      Ptr<IO::FileView> m_pMappedHeader;

      /* The mesh header */
      MESHHEADER* m_pHeader;

      /* In-memory data tables */
      LPVOID m_pVertexData;
      LPVOID m_pTriangleData;

      /* Initialize Storage */
      VOID InitializeInMemory();
      VOID InitializeMapping();
      VOID InitializeOpen(LPCTSTR Filename, DWORD Flags);
      VOID InitializeCreate(LPCTSTR Filename);

      /* Resizes an extent */
      VOID ResizeExtent(UINT i, ULONGLONG NewSize);
      /* Allocates an unused extent, and resizes it */
      UINT AllocateExtent(ULONGLONG Size);
      /* Marks an extent as free */
      VOID FreeExtent(UINT i);

      /* Maps an extent into memory */
      Ptr<IO::FileView> MapExtent(UINT i, DWORD PageAccess);

      /* Validates the extents are correct */
      VOID ValidateExtents();

      /* A cached copy of the bounding sphere */
      Math::Sphere3<FLOAT> m_BoundingSphere;
      DWORD m_BoundingSphereStatus; /* 0 = unknown, 1 = estimated, 2 = computed */

      /* A cached copy of the bounding box */
      Math::AxisAlignedBox3<FLOAT> m_BoundingBox;
      DWORD m_BoundingBoxStatus; /* 0 = unknown, 1 = estimated, 2 = computed */

      /* A cached copy of the centre of mass */
      Math::Vector3<FLOAT> m_CenterOfMass;
      DWORD m_CenterOfMassStatus; /* 0 = unknown, 1 = estimated, 2 = computed */

      /* A cached copy of the covariance matrix */
      Math::Matrix3<FLOAT> m_Covariance;
      DWORD m_CovarianceStatus; /* 0 = unknown, 1 = estimated, 2 = computed */

      /* A collection of key/value pairs to store custom metadata */
      Map<String,String> m_MetaData;

      /* Loads a PLY File */
      VOID ImportPLY(LPCTSTR Filename, IProgress* pProgress);
      VOID ReadPlyVerticesBinary(Ptr<IO::Stream> pStream, IProgress* pProgress);
      VOID ReadPlyTrianglesBinary(Ptr<IO::Stream> pStream, IProgress* pProgress);
      VOID ReadPlyFacesBinary(Ptr<IO::Stream> pStream, IProgress* pProgress);

      VOID ImportBNPTS( LPCTSTR Filename, IProgress* pProgress );

      /* Spatial Index construction */
      struct SpatialIndexConstructionNode
      {
        SpatialIndexConstructionNode()
        { 
          Count = 0; 
          pParent = 0; 
          pChildren = 0;
        }

        VOID CreateChildren()
        {
          pChildren = new SpatialIndexConstructionNode[8];
          for (SIZE_T i=0; i<8; i++)
          {
            pChildren[i].pParent = this;

            if (Count > 1)
              pChildren[i].pStorage = IO::File::OpenTemporary();
            else
              pChildren[i].pStorage = new IO::MemoryStream();
          }
        }

        UINT Count;
        UINT Offset;
        Ptr<IO::SeekableStream> pStorage;
        SpatialIndexConstructionNode* pChildren;
        SpatialIndexConstructionNode* pParent;
      };

      VOID Split(Ptr<IO::SeekableStream> pTableData, UINT PositionOffset, UINT Stride, Math::Vector3F MinimumBound, Math::Vector3F MaximumBound, SpatialIndexConstructionNode* pNode, UINT& Offset, UINT MaximumNodeSize, UINT MaximumDepth);


    };

    #include "MeshImpl.hpp"

  }
}
