/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Collections/HashMapImpl.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1303 $
 * Last Updated: $Date: 2007-11-28 09:47:18 -0500 (Wed, 28 Nov 2007) $
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




//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementTemplate4RuntimeType(Bolitho,DynamicHashMap,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::Bin()
{
  m_AllocBitmap = 0;
  m_pNextBin = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::~Bin()
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
typename DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin* DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::NextBin()
{
  return m_pNextBin;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
CONST typename DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin* DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::NextBin() CONST
{
  return m_pNextBin;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
CONST K& DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::Key(SIZE_T i) CONST
{
  Assert( i < N );
  return m_Keys[i];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
K& DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::Key(SIZE_T i)
{
  Assert( i < N );
  return m_Keys[i];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
CONST V& DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::Value(SIZE_T i) CONST
{
  Assert( i < N );
  return m_Values[i];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
V& DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::Value(SIZE_T i)
{
  Assert( i < N );
  return m_Values[i];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
SIZE_T DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::Find(CONST K& Key) CONST
{
  SIZE_T Bits = m_AllocBitmap;
  SIZE_T i = 0;
  while( Bits != 0 && i < N)
  {
    if ( (Bits & 0x1) && (m_Keys[i] == Key) )
      return i;
    Bits >>= 1;
    i++;
  }
  if (m_pNextBin)
    return m_pNextBin->Find(Key);
  return (SIZE_T)-1; 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
SIZE_T DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::Find(CONST K& Key, Bin*& pBin)
{
  SIZE_T Bits = m_AllocBitmap;
  SIZE_T i = 0;
  while( Bits != 0 && i < N)
  {
    if ( (Bits & 0x1) && (m_Keys[i] == Key) )
    {
      pBin = this;
      return i;
    }
    Bits >>= 1;
    i++;
  }
  if (m_pNextBin)
    return m_pNextBin->Find(Key, pBin);
  return (SIZE_T)-1; 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
SIZE_T DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::Find(CONST K& Key, CONST Bin*& pBin) CONST
{
  SIZE_T Bits = m_AllocBitmap;
  SIZE_T i = 0;
  while( Bits != 0 && i < N)
  {
    if ( (Bits & 0x1) && (m_Keys[i] == Key) )
    {
      pBin = this;
      return i;
    }
    Bits >>= 1;
    i++;
  }
  if (m_pNextBin)
    return m_pNextBin->Find(Key, pBin);
  return (SIZE_T)-1; 
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
SIZE_T DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::Allocate(CONST K& Key, CONST V& Value)
{
  SIZE_T Bits = m_AllocBitmap;
  SIZE_T i = 0;
  
  while( (Bits & 0x1) != 0 && i < N)
  {
    Bits >>= 1;
    i++;
  }

  if ( i < N )
  {
    m_AllocBitmap |= ((SIZE_T)1 << i);
    m_Keys[i] = Key;
    m_Values[i] = Value;
    return i;
  }

  if (!m_pNextBin)
    m_pNextBin = new Bin();
  return N + m_pNextBin->Allocate(Key,Value);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
BOOL DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::IsEmpty() CONST
{
  return m_AllocBitmap == 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
VOID DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Bin::Print() CONST
{
  for(SIZE_T i=0; i<N; i++)
    if (m_AllocBitmap & (1<<i))
      Trace("%d %d %g", i, m_Keys[i], m_Values[i]);
    else
      Trace("%d Unused", i);
  if (m_pNextBin)
    m_pNextBin->Print();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::DynamicHashMap()
{
  m_BinCount = AllocationSize(1);
  m_BaseBinCount = BaseAllocationSize(m_BinCount);
  m_pBins.Allocate(m_BinCount);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::DynamicHashMap(SIZE_T N)
{
  m_BinCount = AllocationSize(N);
  m_BaseBinCount = BaseAllocationSize(m_BinCount);
  m_pBins.Allocate(m_BinCount);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::~DynamicHashMap()
{
  for (SIZE_T i=0; i<m_BinCount; i++)
  {
    Bin* pBin = m_pBins[i].NextBin();
    while (pBin != 0)
    {
      Bin* pNext = pBin->NextBin();
      delete pBin;
      pBin = pNext;
    }
  }
  m_pBins.Free();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
VOID DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Add(CONST K& Key, CONST V& Value)
{
  SIZE_T H = KeyElementTraits::Hash(Key);
  SIZE_T H1 = H % (m_BaseBinCount*2);
  SIZE_T H2 = H % (m_BaseBinCount);

  if (H1 >= m_BinCount)
    H = H2;
  else
    H = H1;

  Bin& B = m_pBins[H];
  SIZE_T j = B.Allocate(Key,Value);

  /*
  if (j >= Bin::N)
    Resize(m_BinCount+1);
  */
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
VOID DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Remove(CONST K& Key)
{
  SIZE_T H = KeyElementTraits::Hash(Key);
  SIZE_T H1 = H % (m_BaseBinCount*2);
  SIZE_T H2 = H % (m_BaseBinCount);

  if (H1 >= m_BinCount)
    H = H2;
  else
    H = H1;

  Bin* pBin = 0;
  SIZE_T j = m_pBins[H].Find(Key, pBin);
  Assert (j != (SIZE_T)-1);

  pBin->Key(j % Bin::N) = K();
  pBin->Value(j % Bin::N) = V();
  pBin->m_AllocBitmap &= ~(1<<(j%Bin::N));

  if (pBin->m_AllocBitmap == 0 && pBin->m_pNextBin != 0)
    Split(pBin);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
BOOL DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Contains(CONST K& Key) CONST
{
  SIZE_T H = KeyElementTraits::Hash(Key);
  SIZE_T H1 = H % (m_BaseBinCount*2);
  SIZE_T H2 = H % (m_BaseBinCount);

  if (H1 >= m_BinCount)
    H = H2;
  else
    H = H1;

  SIZE_T j = m_pBins[H].Find(Key);
  if (j == (SIZE_T)-1)
    return false;
  return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
V& DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Get(CONST K& Key)
{
  SIZE_T H = KeyElementTraits::Hash(Key);
  SIZE_T H1 = H % (m_BaseBinCount*2);
  SIZE_T H2 = H % (m_BaseBinCount);

  if (H1 >= m_BinCount)
    H = H2;
  else
    H = H1;

  Bin* pBin = 0;
  SIZE_T j = m_pBins[H].Find(Key, pBin);
  Assert(j != (SIZE_T)-1);
  return pBin->Value(j % Bin::N);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
CONST V& DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Get(CONST K& Key) CONST
{
  SIZE_T H = KeyElementTraits::Hash(Key);
  SIZE_T H1 = H % (m_BaseBinCount*2);
  SIZE_T H2 = H % (m_BaseBinCount);

  if (H1 >= m_BinCount)
    H = H2;
  else
    H = H1;

  CONST Bin* pBin = 0;
  SIZE_T j = m_pBins[H].Find(Key, pBin);
  Assert(j != (SIZE_T)-1);
  return pBin->Value(j % Bin::N);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
BOOL DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Get(CONST K& Key, V& Value) CONST
{
  SIZE_T H = KeyElementTraits::Hash(Key);
  SIZE_T H1 = H % (m_BaseBinCount*2);
  SIZE_T H2 = H % (m_BaseBinCount);

  if (H1 >= m_BinCount)
    H = H2;
  else
    H = H1;

  CONST Bin* pBin = 0;
  SIZE_T j = m_pBins[H].Find(Key, pBin);
  if(j != (SIZE_T)-1)
  {
    Value = pBin->Value(j % Bin::N);
    return true;
  }
  return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
VOID DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Print() CONST
{
  for(SIZE_T i=0; i<m_BinCount; i++)
  {
    Trace("BIN %d", i);
    m_pBins[i].Print();
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
VOID DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Split(Bin* pBin)
{
  Bin B = *pBin;
  pBin->m_AllocBitmap = 0;
  pBin->m_pNextBin = 0;

  SIZE_T j = 0;
  SIZE_T Bits = B.m_AllocBitmap;
  while ( Bits != 0 && j < Bin::N )
  {
    if ((Bits & 0x1) != 0)
      Add( B.m_Keys[j], B.m_Values[j] );
    Bits >>= 1;
    j++;
  }
  if (B.m_pNextBin)
    Split(B.m_pNextBin);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
VOID DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Resize( SIZE_T N )
{
  SIZE_T OldCount = m_BinCount;
  N = m_BinCount = AllocationSize(N);
  m_pBins.Resize(m_BinCount);

  if (m_BinCount < 2*m_BaseBinCount)
  {
    SIZE_T Begin = OldCount-m_BaseBinCount;
    SIZE_T End = Begin + m_BinCount-OldCount;
    for (SIZE_T i=Begin; i<End; i++)
      Split(&m_pBins[i]);
  }
  else
  {
    SIZE_T Begin = OldCount-m_BaseBinCount;
    SIZE_T End = Begin + 2*m_BaseBinCount - OldCount;
    for (SIZE_T i=Begin; i<End; i++)
      Split(&m_pBins[i]);

    Begin = 0;
    End = m_BinCount - 2*m_BaseBinCount;
    m_BaseBinCount *= 2;
    for (SIZE_T i=Begin; i<End; i++)
      Split(&m_pBins[i]);
  }

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
SIZE_T DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::AllocationSize( SIZE_T N ) CONST
{
  CONST SIZE_T Align = 2;
  return Align * ((N+Align-1) / Align);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
SIZE_T DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::BaseAllocationSize( SIZE_T N ) CONST
{
  SIZE_T i;
  SIZE_T x = N;
  for( i = 0; i < 8*sizeof(SIZE_T); i++)
    if ( x == 1 )
      break;
    else
      x >>= 1;
  return (SIZE_T)1 << i;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
typename DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Enumerator DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Items()
{
  return Enumerator(this);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Enumerator::Enumerator( DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>* pHashMap )
{
  m_pHashMap = pHashMap;
  m_BinIndex = 0;
  
  m_CurrentOffset = 0;
  if (m_pHashMap->m_BinCount == 0)
    m_pCurrentBin = 0;
  else
    m_pCurrentBin = &m_pHashMap->m_pBins[0];

  while (m_pCurrentBin != 0)
  {
    SIZE_T AllocBitmap = m_pCurrentBin->m_AllocBitmap;
    while( (AllocBitmap & 0x01) == 0 && AllocBitmap != 0)
    {
      m_CurrentOffset++;
      AllocBitmap >>= 1;
    }
    if (AllocBitmap == 0)
    {
      if (m_pCurrentBin->m_pNextBin != 0)
        m_pCurrentBin = m_pCurrentBin->m_pNextBin;
      else
      {
        m_BinIndex++;
        if (m_BinIndex < m_pHashMap->m_BinCount)
          m_pCurrentBin = &m_pHashMap->m_pBins[m_BinIndex];
        else
          m_pCurrentBin = 0;
      }
    }
    else
      break;
  }  

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Enumerator::operator BOOL()
{
  return IsValid();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
typename DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Enumerator DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Enumerator::operator ++ ()
{
  Enumerator E = Enumerator(*this);
  (*this)++;
  return E;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
typename DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Enumerator& DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Enumerator::operator++ (int)
{
  m_CurrentOffset++;
  while (m_pCurrentBin != 0)
  {
    SIZE_T AllocBitmap = m_pCurrentBin->m_AllocBitmap;
    
    if (m_CurrentOffset > 0)
    {
      SIZE_T Offset = m_CurrentOffset;
      while(Offset > 0)
      {
        Offset--;
        AllocBitmap >>= 1;
      }
    }

    while( (AllocBitmap & 0x01) == 0 && AllocBitmap != 0)
    {
      m_CurrentOffset++;
      AllocBitmap >>= 1;
    }
    if (AllocBitmap == 0)
    {
      if (m_pCurrentBin->m_pNextBin != 0)
        m_pCurrentBin = m_pCurrentBin->m_pNextBin;
      else
      {
        m_BinIndex++;
        if (m_BinIndex < m_pHashMap->m_BinCount)
          m_pCurrentBin = &m_pHashMap->m_pBins[m_BinIndex];
        else
          m_pCurrentBin = 0;
      }
      m_CurrentOffset = 0;
    }
    else
      break;
  }  

  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
BOOL DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Enumerator::IsValid()
{
  return m_pCurrentBin != 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits >
Pair<K,V> DynamicHashMap<K,V,KeyElementTraits,ValueElementTraits>::Enumerator::CurrentElement()
{
  Assert( m_pCurrentBin != 0 );
  return Pair<K,V>( m_pCurrentBin->Key(m_CurrentOffset), m_pCurrentBin->Value(m_CurrentOffset) );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementTemplate4RuntimeType(Bolitho,HashMap,Object);
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
CONST SIZE_T HashMap<K,V,KeyElementTraits,ValueElementTraits>::PRIMES[] = { 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741 };
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
HashMap<K,V,KeyElementTraits,ValueElementTraits>::HashMap()
{
  m_Bins.Resize(PRIMES[0]);
  m_BinCountIndex = 0;
  m_Count = 0;
  m_MaximumBinCollisions = 0;
  m_NumberOfZeroBins = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
HashMap<K,V,KeyElementTraits,ValueElementTraits>::HashMap(SIZE_T InitialSize)
{
  m_Bins.Resize(InitialSize);
  m_Count = 0;
  m_MaximumBinCollisions = 0;
  m_NumberOfZeroBins = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
VOID HashMap<K,V,KeyElementTraits,ValueElementTraits>::Resize(SIZE_T NewSize)
{
  for (SIZE_T i=0; i<26; i++)
    if (PRIMES[i] >= NewSize)
    {
      m_BinCountIndex = i;
      break;
    }

  //printf("Resize(%d)\n", PRIMES[m_BinCountIndex]);
  Buffer<Bin> NewBins(PRIMES[m_BinCountIndex]);

  for (SIZE_T i=0; i<m_Bins.Length(); i++)
  {
    if (m_Bins[i].Count > 0)
      Add(m_Bins[i].Key, m_Bins[i].Value, NewBins);
  }
  m_Bins = NewBins;
  //printf("End Resize\n");
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
BOOL HashMap<K,V,KeyElementTraits,ValueElementTraits>::Add(CONST K& Key, CONST V& Value, Buffer<Bin>& Bins)
{
  //printf("Key: %p\n", Key);
  SIZE_T H0 = KeyElementTraits::Hash(Key);
  H0 = H0 % Bins.Length();

  Bin& B = Bins[H0];
  if (B.Count == 0)
  {
    B.Count = 1;
    B.Key = Key;
    B.Value = Value;
  }
  else
  {
    //printf("Collide %d\n", H0);
    SIZE_T H = H0;
    USHORT Distance = 1;
    do
    {
      H = (H+1) % Bins.Length();
      Bin& Bi = Bins[H];
      if (Bi.Count == 0)
      {
        Bi.Count = 1;
        if (B.Count < Distance)
          B.Count = Distance;
        Bi.Key = Key;
        Bi.Value = Value;
        break;
      }
      Distance++;
    }
    while(TRUE);
  }

  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
VOID HashMap<K,V,KeyElementTraits,ValueElementTraits>::Add(CONST K& Key, CONST V& Value)
{
  if (Add(Key, Value, m_Bins))
    m_Count++;

  if (GetLoadFactor() > 75)
    Resize(PRIMES[m_BinCountIndex+1]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
VOID HashMap<K,V,KeyElementTraits,ValueElementTraits>::Remove(CONST K& Key)
{
  SIZE_T H0 = KeyElementTraits::Hash(Key);
  H0 = H0 % Bins.Length();
  SIZE_T H = Find(Key);

  if (H == INVALIDBIN)
    return;

  /* Simple Case */
  if (H0 == H && m_Bins[H].Count == 1)
  {
    m_Bins[H].Count = 0;
    return;
  }

  /* Hard cases */
  Assert(FALSE);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
SIZE_T HashMap<K,V,KeyElementTraits,ValueElementTraits>::Find(CONST K& Key) CONST
{
  SIZE_T H = KeyElementTraits::Hash(Key);
  H = H % m_Bins.Length();

  CONST Bin& B = m_Bins[H];

  if (B.Key == Key)
    return H;
  
  INT i = B.Count - 1;
  while (i > 0)
  {
    H = (H+1) % m_Bins.Length();
    CONST Bin& B = m_Bins[H];

    if (B.Count == 0)
      return INVALIDBIN;
 
    if (B.Key == Key)
      return H;
    
    i--;
  }
  
  return INVALIDBIN;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
BOOL HashMap<K,V,KeyElementTraits,ValueElementTraits>::Contains(CONST K& Key) CONST
{
  return Find(Key) != INVALIDBIN;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
V& HashMap<K,V,KeyElementTraits,ValueElementTraits>::Get(CONST K& Key)
{
  SIZE_T Index = Find(Key);
  if (Index != INVALIDBIN)
    return m_Bins[Index].Value;
  throw KeyNotFoundException(this, Bolitho::ToString<K>(Key));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
CONST V& HashMap<K,V,KeyElementTraits,ValueElementTraits>::Get(CONST K& Key) CONST
{
  SIZE_T Index = Find(Key);
  if (Index != INVALIDBIN)
    return m_Bins[Index].Value;
  throw KeyNotFoundException(this, Bolitho::ToString<K>(Key));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
template<class K, class V, class KeyElementTraits, class ValueElementTraits>
BOOL HashMap<K,V,KeyElementTraits,ValueElementTraits>::Get(CONST K& Key, V& Value) CONST
{
  SIZE_T Index = Find(Key);
  if (Index != INVALIDBIN)
  {
    Value = m_Bins[Index].Value;
    return TRUE;
  }
  return FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
