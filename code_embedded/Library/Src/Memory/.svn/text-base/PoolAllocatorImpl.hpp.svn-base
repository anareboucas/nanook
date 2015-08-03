//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
PoolAllocator<T>::PoolAllocator() : m_BlockSize(4096)
{
  m_pBlockChain = CreateBlock(m_BlockSize, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
PoolAllocator<T>::PoolAllocator(SIZE_T BlockSize)
{
  m_BlockSize = BlockSize;
  m_pBlockChain = CreateBlock(BlockSize, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
PoolAllocator<T>::~PoolAllocator()
{
  DeleteAll();  
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* PoolAllocator<T>::New(SIZE_T N)
{
  if (N > m_pBlockChain->FreeCount)
    if (N <= m_BlockSize)
      m_pBlockChain = CreateBlock(m_BlockSize, m_pBlockChain);
    else
      m_pBlockChain = CreateBlock(N, m_pBlockChain);

  m_pBlockChain->FreeCount -= N;
  T* pObjects = m_pBlockChain->pFreePointer;
  m_pBlockChain->pFreePointer += N;

  if (Traits<T>::IsPrimitive)
    ZeroMemory(pObjects, sizeof(T)*N);
  else
    for (SIZE_T i=0; i<N; i++)
      pObjects[i] = T();

  return pObjects;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
T* PoolAllocator<T>::New()
{
  if (m_pBlockChain->FreeCount < 1)
    m_pBlockChain = CreateBlock(m_BlockSize, m_pBlockChain);

  m_pBlockChain->FreeCount--;
  T* pObject = m_pBlockChain->pFreePointer;
  m_pBlockChain->pFreePointer++;

  if (Traits<T>::IsPrimitive)
    ZeroMemory(pObject, sizeof(T));
  else
    *pObject = T();

  return pObject;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID PoolAllocator<T>::Delete(T*& pData)
{
  /* Freeing an individual object does nothing.  DeleteAll will cause all memory to be freed */
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID PoolAllocator<T>::Delete(T*& pData, SIZE_T N)
{
  /* Freeing an individual object does nothing.  DeleteAll will cause all memory to be freed */
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
VOID PoolAllocator<T>::DeleteAll()
{
  MEMORYBLOCK* pBlock = m_pBlockChain;
  while (pBlock)
  {
    MEMORYBLOCK* pNext = pBlock->pNextBlock;
    for (SIZE_T i=0; i<pBlock->ObjectCount; i++)
      pBlock->pData[i].~T();
    FreeMemory(pBlock);
    pBlock = pNext;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
typename PoolAllocator<T>::MEMORYBLOCK* PoolAllocator<T>::CreateBlock(SIZE_T N, typename PoolAllocator<T>::MEMORYBLOCK* pNext)
{
  MEMORYBLOCK* pBlock = (MEMORYBLOCK*)AllocateMemory(sizeof(MEMORYBLOCK) + sizeof(T)*N);
  pBlock->ObjectCount = N;
  pBlock->FreeCount = N;
  pBlock->pData = (T*)(pBlock + 1);
  pBlock->pFreePointer = pBlock->pData;
  pBlock->pNextBlock = pNext;
  return pBlock;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

