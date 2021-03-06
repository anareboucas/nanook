/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Collections/Map.hpp $
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

#include "Utility/Pair.hpp"
#include "Utility/Traits.hpp"

namespace Bolitho
{
  template<class K, class V, class KeyElementTraits=Traits<K>, class ValueElementTraits=Traits<V>, class ThreadingModel=SingleThreadedModel>
  class Map : public Object
  {
    DeclareRuntimeType();
    
    class Enumerator;
    class Iterator;

    class Node
    {
      friend class Map;
			friend class Enumerator;
			friend class Iterator;
		public:
			Node()
			{ m_pLeft = m_pRight = m_pParent = 0; }

			Node( CONST K& Key, CONST V& Value ) :
				m_Key(Key), m_Value(Value)
			{ m_pLeft = m_pRight = m_pParent = 0; }


    private:
      Node* m_pLeft;
      Node* m_pRight;
      Node* m_pParent;
      K m_Key;
      V m_Value;
      enum { Red, Black } m_Status;
    };

		class Enumerator
    {
      friend class Map;
    protected:
      Enumerator( CONST Map* pHashMap );
    public:
      inline operator BOOL() CONST;

      inline Enumerator operator ++ ();
      inline Enumerator& operator++ (int);

      BOOL IsValid() CONST;
			Pair<K,V> CurrentElement() CONST;

    protected:
      CONST Map* m_pMap;
			Node* m_pNode;
    };

		class Iterator
		{
			friend class Map;
		protected:
			Iterator( Map* pMap, Node* pNode );
		public:
      inline Pair<K,V> operator* () CONST;
      inline Pair<K,V> operator-> () CONST;

      CONST K& Key() CONST
      { return m_pNode->m_Key; }
      CONST V& Value() CONST
      { return m_pNode->m_Value; }
      V& Value()
      { return m_pNode->m_Value; }

      // pointer aritmatic
      inline Iterator operator ++ ();
      inline Iterator& operator++ (int);

      inline Iterator operator -- ();
      inline Iterator& operator-- (int);

      // comparison operators
      BOOL operator== (CONST Iterator& rkReference) CONST;
      BOOL operator!= (CONST Iterator& rkReference) CONST;

		protected:
			Map* m_pMap;
			Node* m_pNode;
		};

	public:
    Map();
    virtual ~Map();

    VOID Add( CONST K& Key, CONST V& Value );
    BOOL Remove( CONST K& Key );
		VOID RemoveAll();

    BOOL Contains( CONST K& Key ) CONST;

		V& operator[] (CONST K& Key);
		CONST V& operator[] (CONST K& Key) CONST;

    V& Get( CONST K& Key );
    CONST V& Get( CONST K& Key ) CONST;
    BOOL Get( CONST K& Key, V& Value ) CONST;

		Iterator Find( CONST K& Key ) CONST;

    SIZE_T Count() CONST;
		BOOL IsEmpty() CONST;

    Enumerator Elements() CONST;

		Iterator Begin();
		Iterator End();
  private:
    Node* m_pRoot;
		Node* m_pNil;

    SIZE_T m_NodeCount;

    Node* LeftRotate( Node* pNode );
		Node* RightRotate( Node* pNode );

		Node* Insert( CONST K& Key, CONST V& Value );
		Node* InsertImpl( CONST K& Key, CONST V& Value );

		BOOL IsNil(Node* pNode) CONST;
		VOID SetNil( Node** pNode );
		Node* Minimum(Node* pNode) CONST;
		Node* Maximum(Node* pNode) CONST;
		Node* Predecessor(Node* pNode ) CONST;
		Node* Successor(Node* pNode) CONST;
		VOID SwapNode( Node* pDest, Node* pSrc);

		BOOL Delete( Node* pZ );
		VOID DeleteFixup( Node* pNode );
		Node* FindNode(CONST K& Key) CONST;

  };

  #include "MapImpl.hpp"

}
