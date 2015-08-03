/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Collections/Array.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1310 $
 * Last Updated: $Date: 2007-12-02 12:48:02 -0500 (Sun, 02 Dec 2007) $
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

#include "Utility/Traits.hpp"
#include "Memory/SharedBuffer.hpp"
#include "IO/Stream.hpp"
#include "IO/BinaryStream.hpp"

namespace Bolitho
{

  template<class T, class ElementTraits, class Allocator, class ThreadingModel>
  class Array : public Object
  {
	private:
		typename ThreadingModel::LockType m_Lock;
    DeclareRuntimeType();
  public:
    // iterator 
    class Iterator
    {
      friend class Array;

    protected:
      // CONSTruction
      Iterator();
      Iterator(T* pItem);
#ifdef DEBUG
      Iterator(T* pItem, Array* pArray);
#endif
    public:
      // implicit conversions
      inline CONST T& operator* () CONST;
      inline T& operator* ();

      // pointer aritmatic
      inline Iterator operator ++ ();
      inline Iterator& operator++ (int);

      inline Iterator operator -- ();
      inline Iterator& operator-- (int);

      inline Iterator operator  +  (PTRDIFF_T N) CONST;
      inline Iterator& operator += (PTRDIFF_T N);

      inline Iterator operator  -  (PTRDIFF_T N) CONST;
      inline Iterator& operator -= (PTRDIFF_T N);

      inline PTRDIFF_T operator - (CONST Iterator& i) CONST;

      // comparison operators
      BOOL operator== (CONST Iterator& rkReference) CONST;
      BOOL operator!= (CONST Iterator& rkReference) CONST;
      BOOL operator<  (CONST Iterator& rkReference) CONST;
      BOOL operator>  (CONST Iterator& rkReference) CONST;
      BOOL operator<= (CONST Iterator& rkReference) CONST;
      BOOL operator>= (CONST Iterator& rkReference) CONST;

    protected:
      T* m_pItem;

#ifdef DEBUG
      BOOL IsValid() CONST;
      BOOL IsReachable(CONST Iterator& I) CONST;

      Array* m_pArray;
#endif
    };

    class Enumerator
    {
      friend class Array;
    protected:
      Enumerator(Array* pArray, INT Index);
    public:
      inline operator BOOL();
      inline T& operator* ();
      inline CONST T& operator*() CONST;

      inline Enumerator operator ++ ();
      inline Enumerator& operator++ (int);

      inline Enumerator operator -- ();
      inline Enumerator& operator-- (int);

      inline Enumerator operator  +  (int N) CONST;
      inline Enumerator& operator += (int N);

      inline Enumerator operator  -  (int N) CONST;
      inline Enumerator& operator -= (int N);

      inline int operator - (CONST Enumerator& i) CONST;

      // comparison operators
      BOOL operator== (CONST Enumerator& rkReference) CONST;
      BOOL operator!= (CONST Enumerator& rkReference) CONST;
      BOOL operator<  (CONST Enumerator& rkReference) CONST;
      BOOL operator>  (CONST Enumerator& rkReference) CONST;
      BOOL operator<= (CONST Enumerator& rkReference) CONST;
      BOOL operator>= (CONST Enumerator& rkReference) CONST;

      BOOL HasMoreElements();
      CONST T& CurrentElement() CONST;
      T& CurrentElement();

    protected:
      Array* m_pArray;
      INT m_Index;

    };

    // Construction
    Array();
    Array(SIZE_T N, CONST T& Item);
    Array(CONST Array& A);
    Array(CONST T* pData, SIZE_T N);

		virtual ~Array() 
		{}

		explicit Array(SIZE_T N);

    // storage management
    VOID Resize(SIZE_T N);
    VOID Reserve(SIZE_T N);

    /* Returns the maximum length of the array that can be created without resizing the underlying memory block */
    INLINE SIZE_T Capacity() CONST
    { return m_Buffer.Length(); }
    /* Returns the number of elements in the array */
    INLINE SIZE_T Length() CONST
    { return m_Length; }
    /* Returns TRUE if the array is empty */
    BOOL IsEmpty() CONST
    { return m_Length == 0; }

    /* Returns a reference to the i-th element of the array */ 
    INLINE CONST T& operator() (SIZE_T i) CONST
    { return Get(i); }
    /* Returns a reference to the i-th element of the array */
    INLINE T& operator() (SIZE_T i)
    { return Get(i); }

    /* Returns a reference to the i-th element of the array */ 
    INLINE CONST T& operator[] (SIZE_T i) CONST
    { return Get(i); }
    /* Returns a reference to the i-th element of the array */
    INLINE T& operator[] (SIZE_T i)
    { return Get(i); }

    /* Returns a reference to the i-th element of the array */
    INLINE T& Get(SIZE_T i)
    { Assert(i < m_Length); return m_Buffer[i]; }
    /* Returns a reference to the i-th element of the array */
    INLINE CONST T& Get(SIZE_T i) CONST
    { Assert(i < m_Length); return m_Buffer[i]; }

    /* Returns a reference to the i-th element of the array */
    INLINE T& Get(PTRDIFF_T i)
    { if (i >= 0) { Assert(i < m_Length); return m_Buffer[i]; } else { Assert(i > -m_Length); return m_Buffer[m_Length-i]; } }
    /* Returns a reference to the i-th element of the array */
    INLINE CONST T& Get(PTRDIFF_T i) CONST
    { if (i >= 0) { Assert(i < m_Length); return m_Buffer[i]; } else { Assert(i > -m_Length); return m_Buffer[m_Length-i]; } }

    /* Sets the i-th element of the array */
    VOID Set(SIZE_T i, CONST T& Item)
    { Assert(i < m_Length); m_Buffer[i] = Item; }

    /* Returns a reference to the first element in the array */
    CONST T& GetFirst() CONST
    { Assert(m_Length > 0); return m_Buffer[0]; }
    /* Returns a reference to the first element in the array */
    T& GetFirst()
    { Assert(m_Length > 0); return m_Buffer[0]; }
    /* Returns a reference to the last element in the array */
    CONST T& GetLast() CONST
    { Assert(m_Length > 0); return m_Buffer[m_Length-1]; }
    /* Returns a reference to the last element in the array */
    T& GetLast()
    { Assert(m_Length > 0); return m_Buffer[m_Length-1]; }

    // item swapping
    VOID Swap(Iterator From, Iterator To);
    VOID Swap(SIZE_T From, SIZE_T To);

    // append / concatenation
    Array& operator += (CONST Array& rkReference);
    Array& operator += (CONST T& Item);

    T& Add(CONST T& Item);
    T& Add(CONST Array& A);

    template<class AnyIterator>
    T& Add(AnyIterator Begin, AnyIterator End );

    // specialisation of Add for efficient Array to Array adds
    T& Add(typename Array<T>::Iterator Begin, typename Array<T>::Iterator End )
    {
      SIZE_T OldLength = m_Length;
      SIZE_T NewLength = End - Begin;
      Resize(OldLength-NewLength);
      m_Buffer.DataCopy(&m_Buffer[OldLength], Begin, NewLength);
      return m_Buffer[OldLength];
    }

    // insertion
    VOID Insert(CONST T& Item, Iterator At);
    VOID Insert(CONST Array& rkArray, Iterator At);
    template<class AnyIterator>
    VOID Insert(AnyIterator Begin, AnyIterator End, Iterator At);

    VOID Insert(Iterator Begin, Iterator End, Iterator At)
    {  
      Assert(Begin.IsReachable(End));
      Assert(At.IsValid());

      SIZE_T Position = At.m_pItem - &m_Buffer[0];
      SIZE_T CopyLength = End - Begin;
      SIZE_T OldLength = m_Length;
      Resize(m_Length + CopyLength);
      m_Buffer.DataCopy(&m_Buffer[Position+CopyLength], &m_Buffer[Position] , OldLength - Position);
      m_Buffer.DataCopy(&m_Buffer[Position], &*Begin, CopyLength);
    }

    // O(m) remove
    //  will remove the item(s), and place them at the end of 
    //  the collection, and return an iterator pointing to the first one.
    Iterator Remove(CONST T& Item);
    template<class Predicate>
    Iterator Remove(CONST Predicate& P);

    // O(m) erase
    //  erase deletes a range of items from the list.
    SIZE_T Erase(Iterator Begin);
    SIZE_T Erase(Iterator Begin, Iterator End);
    
    SIZE_T Erase(SIZE_T Begin);
    SIZE_T Erase(SIZE_T Begin, SIZE_T End);

    T EraseLast();

    // O(n) linear searching
    Iterator Find(CONST T& Item);
    Iterator Find(CONST T& Item, Iterator Begin, Iterator End);

    // O(log n) searching
    Iterator BinarySearch(CONST T& Item);
    Iterator BinarySearch(CONST T& Item, Iterator Begin, Iterator End);

    // O(n log n) sorting
    VOID Sort();
    VOID Sort(Iterator Begin, Iterator End);

    // O(n) functor application
    template<class UnaryFunctor>
    UnaryFunctor Apply(UnaryFunctor& F);
    template<class BinaryFunctor>
    BinaryFunctor Apply(BinaryFunctor& F, CONST T& B);

    template<class UnaryFunctor>
    UnaryFunctor Apply(UnaryFunctor& F, Iterator Begin, Iterator End);
    template<class BinaryFunctor>
    BinaryFunctor Apply(BinaryFunctor& F, CONST T& B, Iterator Begin, Iterator End);

    template<class BinaryFunctor, class AnyIterator>
    BinaryFunctor Apply(BinaryFunctor& F, AnyIterator B_Begin, AnyIterator B_End, Iterator Begin, Iterator End);

    // iterator access
    Iterator Begin();
    CONST Iterator Begin() CONST;
    Iterator End();
    CONST Iterator End() CONST;

    CONST Enumerator Elements() CONST;
    Enumerator Elements();

  protected:
    SIZE_T m_Length;
    SharedBuffer<T,Allocator> m_Buffer;

    // a Reserve preserving resize operation used internally
    VOID InternalResize(SIZE_T N);

    // helpers for Sort functions
    Iterator SortPartition(Iterator Begin, Iterator End);
    VOID InsertionSort(Iterator Begin, Iterator End);
  };

  #include "ArrayImpl.hpp"
}

