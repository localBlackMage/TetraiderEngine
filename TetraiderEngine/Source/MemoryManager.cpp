/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MemoryManager.cpp
Purpose: Allocation/Deallocation of Memory
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/1/18
- End Header --------------------------------------------------------*/

#include "MemoryManager.h"
#include <iostream>

// temp
#include <conio.h>
#include <chrono>
#include <vector>

MemoryManager* gMemoryManager;

MemoryManager::MemoryManager():
	m_TotalBufferSize(DEFAULT_BUFFER_SIZE_BYTE), m_pHead(nullptr) {

	m_Buffer = malloc(m_TotalBufferSize);
	m_pHead = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	m_pHead->Initialize(m_Buffer, 0, m_TotalBufferSize);
	m_NumCachedBlock = 0;
	//cout << m_pHead<<endl;
}

MemoryManager::~MemoryManager() {
	if (m_pHead){
		m_pHead->CleanUp();
		free(m_pHead);
		m_pHead = nullptr;
	}
	free(m_Buffer);
}

inline void* MemoryManager::Alloc(std::size_t size) {
	std::cout << "myalloc\n";
	MemoryBlock *current = m_pHead;
	// find next available slot 
	while(current && (current->freesize < size)) {
		current = current->next;
	}
	// request failed
	if (!current) {
		std::cout << "MemoryManager::Alloc() FAILED - No Available Slot" << endl;
		return nullptr;
	}
	// create a new memoryblock and insert into the list
	current->next = NewMemoryBlock();
	current->next->Initialize(	static_cast<unsigned char*>(current->pData) + current->size, 
								size, 
								current->freesize - size );
	current->freesize = 0;
	current->next->prev = current;

	return current->next->pData;
}

inline void MemoryManager::Free(void* ptr){
	MemoryBlock *current = m_pHead->next;
	if (!current || (current->pData==nullptr)){
		return;
	}
	// TODO: remove while-loop to O(1) of block node find
	//		 use unordered_map<?,MemoryBlock*>
	//						  <?,std::shared_ptr<MemoryBlock> >
	// ? = reinterpret_case<unsigned long>
	while(current->pData != ptr){
		current = current->next;
	}
	// remove links
	if (current->next){
		current->next->prev = current->prev;
	}
	if (current->prev){
		current->prev->next = current->next;
		current->prev->freesize = current->size + current->freesize;
	}

	current->next = nullptr;
	// try caching current block. If not, free.
	Recycle(current);
}

inline MemoryBlock* MemoryManager::NewMemoryBlock(){

	if (m_NumCachedBlock == 0){ // no cached memoryblock to be reused
		return (MemoryBlock*)malloc(sizeof(MemoryBlock));
	}else{									// use previously recycled memory block
		return m_Cache[--m_NumCachedBlock]; // NOTE: cacheIndex = m_NumCachedBlock-1
	}
}

inline void MemoryManager::Recycle(MemoryBlock* pNode){
	if (m_NumCachedBlock == MAX_CACHE_SIZE_NUM-1){
		std::cout << "CACHE IS FULL -- MAX_CACHE_SIZE_NUM: "<< MAX_CACHE_SIZE_NUM << endl;
		free(pNode);
		return;
	}
	m_Cache[m_NumCachedBlock++] = pNode;
}

void *MemoryManager::m_Buffer = nullptr;
//MemoryManager gMemoryManager;

//void *operator new(std::size_t size)
//{
//	return gMemoryManager.Alloc(size);
//}
//
//void operator delete(void *ptr)
//{
//	gMemoryManager.Free(ptr);
//}
//void* operator new[](std::size_t size)
//{
//	return gMemoryManager.Alloc(size);
//}
//
//void operator delete[](void* arrayPtr)
//{
//	gMemoryManager.Free(arrayPtr);
//}
/*
int main()
{
	///
	auto start = std::chrono::system_clock::now();
	for (int i = 0; i < 5000000; ++i)
	{
		int *k = (int *)malloc(sizeof(int));
		free(k);
	}
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	cout << "Benchmark OS heap allocation/deallocation:	" << elapsed_seconds.count() << endl;

	start = std::chrono::system_clock::now();
	for (int i = 0; i < 5000000; ++i)
	{
		int *k = new int;
		delete k;
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	cout << "Benchmark custom allocation/deallocation:	" << elapsed_seconds.count() << endl;

	//_getch();

	return 0;
}
*/
