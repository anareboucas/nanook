/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Collections/ArrayImpl.hpp $
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





//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementTemplate4RuntimeType(Bolitho,Array,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Array()
{
	m_Length = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Array(CONST T* pData, SIZE_T N)
{
	m_Length = 0;
	Resize(N);
	if (N > 0)
		CopyObjects(&m_Buffer[0], pData, N);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Array(SIZE_T N)
{
  m_Buffer.Allocate(N);
  m_Length = N;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Array(SIZE_T N, CONST T& Item)
{
  m_Buffer.Allocate(N);
  m_Length = N;
	for (SIZE_T i=0; i<N; i++)
		m_Buffer[i] = Item;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Array(CONST Array& rkReference)
{
  m_Length = rkReference.m_Length;
  m_Buffer = rkReference.m_Buffer;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Resize(SIZE_T N)
{
	m_Buffer.Resize(N);
	m_Length = N;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::InternalResize(SIZE_T N)
{
	if (N > m_Buffer.Length())
    if (m_Length < 16)
		  m_Buffer.Resize(N);
    else
		  m_Buffer.Resize((INT)((FLOAT)N * 1.5));
	m_Length = N;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Reserve(SIZE_T N)
{
  m_Buffer.Resize(N);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Swap(Iterator From, Iterator To)
{
  ElementTraits::Swap(*From, *To);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Swap(SIZE_T From, SIZE_T To)
{
  ElementTraits::Swap(m_Buffer[From], m_Buffer[To]);
}
   
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>& Array<T,ElementTraits,Allocator,ThreadingModel>::operator += (CONST Array& rkReference)
{
  Add(rkReference);
  return *this;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>& Array<T,ElementTraits,Allocator,ThreadingModel>::operator += (CONST T& Item)
{
  Add(Item);
  return *this;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
T& Array<T,ElementTraits,Allocator,ThreadingModel>::Add(CONST Array<T,ElementTraits,Allocator,ThreadingModel>& rkReference)
{
  SIZE_T Length = m_Length;
  InternalResize(m_Length+rkReference.m_Length);
  m_Buffer.DataCopy(&m_Buffer[Length], &rkReference.m_Buffer[0], rkReference.m_Length);
  return m_Buffer[Length];
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
T& Array<T,ElementTraits,Allocator,ThreadingModel>::Add(CONST T& Item)
{
	SIZE_T Length = m_Length;
	InternalResize(m_Length+1);
  m_Buffer[Length] = Item;
  return m_Buffer[Length];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
template<class AnyIterator>
T& Array<T,ElementTraits,Allocator,ThreadingModel>::Add(AnyIterator Begin, AnyIterator End )
{
  Assert(Begin.IsReachable(End));

  SIZE_T Length = m_Length;
  while (Begin != End)
    Add(*Begin++);
  return m_Buffer[Length];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Insert(CONST T& Item, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator At)
{
  SIZE_T Position = At.m_pItem - &m_Buffer[0];
  InternalResize(m_Length + 1);
  m_Buffer.DataCopy(&m_Buffer[Position+1], &m_Buffer[Position] , m_Length - 1 - Position);
  m_Buffer[Position] = Item;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Insert(CONST Array& rkArray, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator At)
{
  SIZE_T Position = At.m_pItem - &m_Buffer[0];
  SIZE_T OldLength = m_Length;
  InternalResize(m_Length + rkArray.m_Length);
  m_Buffer.DataCopy(&m_Buffer[Position+rkArray.m_Length], &m_Buffer[Position] , OldLength - Position);
  m_Buffer.DataCopy(&m_Buffer[Position], &rkArray.m_Buffer[0], rkArray.m_Length);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
template<class AnyIterator>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Insert(AnyIterator Begin, AnyIterator End, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator At)
{
  // TODO: Fix this: It doesn't insert at the correct location

  Assert(Begin.IsReachable(End));
  SIZE_T OldLength = m_Length;
  SIZE_T NewLength = End-Begin;
  Resize(OldLength + NewLength);
  
  for (SIZE_T i=0; i<NewLength; i++)
    m_Buffer[i] = *Begin++;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::Remove(CONST T& Item)
{
  typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin = this->Begin();
  typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator End = this->End();

  while (Begin != End)
  {
    if (*Begin == Item)
    {
      T Temp = *Begin;
      CopyObjects(Begin.m_pItem, Begin.m_pItem+1, End-Begin);
      End--;
      *End = Temp;
    }
    else
      Begin++;
  }
  return End;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
template<class Predicate>
typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::Remove(CONST Predicate& P)
{
  typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin = this->Begin();
  typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator End = this->End();

  while (Begin != End)
  {
    if (P(*Begin))
    {
      T Temp = *Begin;
      m_Buffer.DataCopy(Begin.m_pItem, Begin.m_pItem+1, End-Begin);
      End--;
      *End = Temp;
    }
    else
      Begin++;
  }
  return End;

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
T Array<T,ElementTraits,Allocator,ThreadingModel>::EraseLast()
{
  Assert(m_Length > 0);
  T Item = m_Buffer[m_Length-1];
  Resize(m_Length - 1);
  return Item;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Erase(typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator At)
{
  typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin = this->Begin();
  SIZE_T EraseSize = m_Length - (At-Begin);
  Resize(At-Begin);
  return EraseSize;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Erase(typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator EraseBegin, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator EraseEnd)
{
  typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin = this->Begin();
  SIZE_T EraseSize = EraseEnd - EraseBegin;
  SIZE_T BeforeSize = EraseBegin - Begin;
  m_Buffer.DataCopy(EraseBegin.m_pItem, EraseEnd.m_pItem, m_Length - BeforeSize - EraseSize);
  Resize(m_Length - EraseSize);
  return EraseSize;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Erase(SIZE_T At)
{
  SIZE_T EraseSize = m_Length - At;
  Resize(At);
  return EraseSize;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
SIZE_T Array<T,ElementTraits,Allocator,ThreadingModel>::Erase(SIZE_T EraseBegin, SIZE_T EraseEnd)
{
  SIZE_T EraseSize = EraseEnd - EraseBegin;
  SIZE_T BeforeSize = EraseBegin;
  m_Buffer.DataCopy(&m_Buffer[EraseBegin], &m_Buffer[EraseEnd], m_Length - BeforeSize - EraseSize);
  Resize(m_Length - EraseSize);
  return EraseSize;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::Begin()
{
#ifdef DEBUG
  return Iterator(&m_Buffer[0], this);
#else
  return Iterator(&m_Buffer[0]);
#endif
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::Begin() CONST
{
#ifdef DEBUG
  return Iterator(&m_Buffer[0], this);
#else
  return Iterator(&m_Buffer[0]);
#endif
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::End()
{
#ifdef DEBUG
  return Iterator(&m_Buffer[0] + m_Length, this);
#else
  return Iterator(&m_Buffer[0] + m_Length);
#endif
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::End() CONST
{
#ifdef DEBUG
  return Iterator(&m_Buffer[0] + m_Length, this);
#else
  return Iterator(&m_Buffer[0] + m_Length);
#endif
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator Array<T,ElementTraits,Allocator,ThreadingModel>::Elements() CONST
{
  return Enumerator(this, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator Array<T,ElementTraits,Allocator,ThreadingModel>::Elements()
{
  return Enumerator(this, 0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::Find(CONST T& Item)
{
  return Find(Item, Begin(), End());
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::Find(CONST T& Item, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator End)
{
  Assert(Begin.IsReachable(End));

  while(Begin != End)
    if (ElementTraits::CompareEqual(*Begin,Item))
      return Begin;
    else
      Begin++;
  return End;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::BinarySearch(CONST T& Item)
{
  return BinarySearch(Item, Begin(), End());
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::BinarySearch(CONST T& Item, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator End)
{
  Assert(Begin.IsReachable(End));

  while (Begin != End)
  {
    if (ElementTraits::CompareLessThan(Item, *Begin))
      return End;
    if (LessThan(*(End-1), Item))
      return End;

    Iterator Middle = Begin + (End-Begin)/2;
    if (ElementTraits::CompareLessThan(Item, *Middle))
      End = Middle;
    else if (ElementTraits::CompareLessThan(*Middle, Item))
      Begin = Middle;
    else
      return Middle;
  }
  return End;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Sort()
{
  return Sort(Begin(), End());
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::Sort(typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator End)
{
  Assert(Begin.IsReachable(End));
  
	CONST int STKSIZE = (8*sizeof(VOID*) - 2);
	
	typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Pivot;
	typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator LowStack[STKSIZE];
	typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator HighStack[STKSIZE];
	unsigned StackP = 1;
    
	do
	{
		if (Begin < End)
		{
			
			if (End-Begin < 10)
			{
				InsertionSort(Begin,End);

				StackP--;
				Begin = LowStack[StackP];
				End = HighStack[StackP];
				continue;
			}
						
			Pivot = SortPartition(Begin,End);
      if (Pivot-1-Begin < End-Pivot+1)
			{
				//QSort(l, Pivot-1);
				LowStack[StackP] = Pivot+1;
				HighStack[StackP] = End;
				StackP++;
				End = Pivot;
			}
			else
			{
				//QSort(Pivot+1, h);
				LowStack[StackP] = Begin;
				HighStack[StackP] = Pivot;
				StackP++;
				Begin = Pivot+1;

			}
		}
		else
		{
			StackP--;
			Begin = LowStack[StackP];
			End = HighStack[StackP];
		}
	}
	while (StackP > 0);

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
VOID Array<T,ElementTraits,Allocator,ThreadingModel>::InsertionSort(typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator End)
{
	for (typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator j = Begin + 1; j < End; j++)
	{
		T key = *j;
    typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator i = j - 1;
    while (i >= Begin && ElementTraits::CompareLessThan(key,*i))
    {
      *(i+1) = *i;
      i--;
    }
		*(i+1) = key;
	}
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::SortPartition(typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator End)
{
	// Choosing Pivot Item as median of l-mid-h.
	typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Middle = Begin + ((End-Begin)>>1);
  End--;

  if (ElementTraits::CompareLessThan(*Middle, *Begin))
		Swap(Middle, Begin);
	if (ElementTraits::CompareLessThan(*End, *Begin))
		Swap(End, Begin);
	if (ElementTraits::CompareLessThan(*End, *Middle))
		Swap(End, Middle);
    
	Swap(Middle, Begin);
    
	typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Pivot = Begin;

	while (Begin < End) 
	{
		/* Move left while item <= pivot */
    while(Begin != End)
      if (ElementTraits::CompareLessThan(*Begin,*Pivot)) 
		    Begin++;   // less
      else if (ElementTraits::CompareLessThan(*Pivot,*Begin))
        break;
      else
        Begin++;   // equal

    /* Move right while item > pivot */
		while(ElementTraits::CompareLessThan(*Pivot,*End)) 
		  End--;
		
		if (Begin < End) 
			Swap(Begin,End);
	}
	/* right is final position for the pivot */
	Swap(Pivot, End);
	return End;

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
template<class UnaryFunctor>
UnaryFunctor Array<T,ElementTraits,Allocator,ThreadingModel>::Apply(UnaryFunctor& F)
{
  return Apply(F, Begin(), End());
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
template<class BinaryFunctor>
BinaryFunctor Array<T,ElementTraits,Allocator,ThreadingModel>::Apply(BinaryFunctor& F, CONST T& B)
{
  return Apply(F, B, Begin(), End());
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
template<class UnaryFunctor>
UnaryFunctor Array<T,ElementTraits,Allocator,ThreadingModel>::Apply(UnaryFunctor& F, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator End)
{
  Assert(Begin.IsReachable(End));
  while(Begin != End)
  {
    *Begin = F(*Begin);
    Begin++;
  }
  return F;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
template<class BinaryFunctor>
BinaryFunctor Array<T,ElementTraits,Allocator,ThreadingModel>::Apply(BinaryFunctor& F, CONST T& B, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator End)
{
  Assert(Begin.IsReachable(End));
  while(Begin != End)
  {
    *Begin = F(*Begin, B);
    Begin++;
  }
  return F;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
template<class BinaryFunctor, class AnyIterator>
BinaryFunctor Array<T,ElementTraits,Allocator,ThreadingModel>::Apply(BinaryFunctor& F, AnyIterator B_Begin, AnyIterator B_End , typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Begin, typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator End)
{
  while(Begin != End || B_Begin != B_End)
  {
    *Begin = F(*Begin, *B_Begin);
    Begin++;
    B_Begin++;
  }
  return F;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::Iterator(T* pItem)
{
  m_pItem = pItem;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::Iterator()
{
  m_pItem = NULL;
}

#ifdef DEBUG
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::Iterator(T* pItem, Array<T,ElementTraits,Allocator,ThreadingModel>* pArray)
{
  Assert(pArray != NULL);
  m_pArray = pArray;
  m_pItem = pItem;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::IsValid() CONST
{
  Assert(m_pArray != NULL);
  Assert(m_pItem >= &m_pArray->m_Buffer[0]);
  Assert(m_pItem <= &m_pArray->m_Buffer[m_pArray->m_Length-1]);
  return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::IsReachable(CONST Iterator& I) CONST
{
  Assert(m_pArray == I.m_pArray);
  Assert(m_pItem <= I.m_pItem);
  return true;
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline CONST T& Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator* () CONST
{
  Assert(IsValid());
  return *m_pItem;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline T& Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator* ()
{
  Assert(IsValid());
  return *m_pItem;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator++ ()
{
#ifdef DEBUG
  return Iterator(m_pItem+1, m_pArray);
#else
    return Iterator(m_pItem+1);
#endif

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator++ (int)
{
  m_pItem++;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator-- ()
{
#ifdef DEBUG
  return Iterator(m_pItem-1, m_pArray);
#else
  return Iterator(m_pItem-1);
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator-- (int)
{
  m_pItem--;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator  +  (PTRDIFF_T N) CONST
{

#ifdef DEBUG
  return Iterator(m_pItem+N, m_pArray);
#else
  return Iterator(m_pItem+N);
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator += (PTRDIFF_T N)
{
  m_pItem += N;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator  -  (PTRDIFF_T N) CONST
{
#ifdef DEBUG
  return Iterator(m_pItem-N, m_pArray);
#else
  return Iterator(m_pItem-N);
#endif
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator -= (PTRDIFF_T N)
{
  m_pItem-=N;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline PTRDIFF_T Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator- (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& rkReference) CONST
{
  return (m_pItem-rkReference.m_pItem);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator== (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& rkReference) CONST
{
  return rkReference.m_pItem == m_pItem;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator!= (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& rkReference) CONST
{
  return rkReference.m_pItem != m_pItem; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator> (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& rkReference) CONST
{
  return m_pItem > rkReference.m_pItem;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator< (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& rkReference) CONST
{
  return m_pItem < rkReference.m_pItem; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator>= (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& rkReference) CONST
{
  return m_pItem >= rkReference.m_pItem;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator::operator<= (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Iterator& rkReference) CONST
{
  return m_pItem <= rkReference.m_pItem; 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::Enumerator(Array* pArray, INT Index)
{
  m_pArray = pArray;
  m_Index = Index;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator BOOL()
{ 
  return HasMoreElements(); 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline T& Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator* ()
{
  return CurrentElement();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline CONST T& Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator*() CONST
{
  return CurrentElement();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator ++ ()
{
  return Enumerator(m_pArray, m_Index+1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator++ (int)
{
  m_Index++;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator -- ()
{
  return Enumerator(m_pArray, m_Index-1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator-- (int)
{
  m_Index--;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator  +  (int N) CONST
{
  return Enumerator(m_pArray, m_Index + N);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator += (int N)
{
  m_Index += N;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator  -  (int N) CONST
{
  return Enumerator(m_pArray, m_Index - N);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator -= (int N)
{
  m_Index -= N;
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline INT Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator - (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& E) CONST
{
  Assert(m_pArray == E.m_pArray);
  return m_Index - E.m_Index;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator== (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& E) CONST
{
  return m_pArray == E.m_pArray && m_Index == E.m_Index;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator!= (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& E) CONST
{
  return m_pArray != E.m_pArray || m_Index != E.m_Index;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator<  (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& E) CONST
{
  return m_pArray == E.m_pArray && m_Index < E.m_Index;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator>  (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& E) CONST
{
  return m_pArray == E.m_pArray && m_Index > E.m_Index;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator<= (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& E) CONST
{
  return m_pArray == E.m_pArray && m_Index <= E.m_Index;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::operator>= (CONST typename Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator& E) CONST
{
  return m_pArray == E.m_pArray && m_Index >= E.m_Index;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
BOOL Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::HasMoreElements()
{
  return m_Index >= 0 && m_Index < (INT)m_pArray->Length();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
CONST T& Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::CurrentElement() CONST
{
  return m_pArray->Get(m_Index);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
T& Array<T,ElementTraits,Allocator,ThreadingModel>::Enumerator::CurrentElement()
{
  return m_pArray->Get(m_Index);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline Bolitho::IO::BinaryStream& operator << (Bolitho::IO::BinaryStream& S, CONST Array<T,ElementTraits,Allocator,ThreadingModel>& A)
{
  S << (ULONGLONG)A.Length();
  for(SIZE_T i=0; i<A.Length(); i++)
    S << A[i];
  return S;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T, class ElementTraits, class Allocator, class ThreadingModel>
inline Bolitho::IO::BinaryStream& operator >> (Bolitho::IO::BinaryStream& S, Array<T,ElementTraits,Allocator,ThreadingModel>& A)
{
  ULONGLONG N = S.ReadUInt64();
#if defined(PLATFORM_32BIT)
  if (N > 0x7fffffff)
    throw InvalidOperationException(0, "Unable to load array -- there are more entries than can be addressed on PLATFORM_32BIT");
#endif
  A.Resize((SIZE_T)N);
  for(SIZE_T i=0; i<(SIZE_T)N; i++)
    S >> A[i];  
  return S;
}

