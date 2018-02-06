/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MemoryManager.h
Purpose: Allocation/Deallocation of Memory
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/1/18
- End Header --------------------------------------------------------*/
#pragma once
#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <cstdlib>

	using namespace std;

	const int	 MAX_CACHE_SIZE_NUM = 100;			// # of Cacheable MemoryBlocks
	const size_t DEFAULT_BUFFER_SIZE_BYTE = 524288000;	// 5 MB = 5*1024*1024 byte

	/*class MemoryManagerInterface {
	public:
		virtual void* Alloc(std::size_t) = 0;
		virtual void Free(void*) = 0;
	};*/

	typedef struct ListNode {
		void *pData; 				// pointer to data block's position in m_Buffer
		size_t size;				// size of this data block
		size_t freesize;			// available memory space b/w current and next block
		ListNode *next;				// next memoryblock
		ListNode *prev;				// previous memoryblock

		void Initialize(void *_ptr, size_t _size, size_t _freesize) {
			pData = _ptr;
			size = _size;
			freesize = _freesize;
			next = prev = nullptr;
		}

		void CleanUp() {
			// recursively delete blocks
			MemoryBlock *current = this;
			if (current->next)
			{
				current->next->CleanUp();
				free(current->next);
			}
		}
	} MemoryBlock;



	class MemoryManager  {
	private:
		static void* m_Buffer;						// Block of memory where all data exists
		const size_t m_TotalBufferSize;				// How big the buffer is in bytes
		MemoryBlock* m_pHead;						// First MemoryBlock in the linked list
		MemoryBlock* m_Cache[MAX_CACHE_SIZE_NUM];	// Stores MemoryBlocks to be deleted, when full all will be deleted at once
		int m_NumCachedBlock;						// Number of MemoryBlocks stored in the m_Cache
	public:

		MemoryManager();
		 ~MemoryManager();
		MemoryManager(const MemoryManager &) = delete;
		void operator=(const MemoryManager &) = delete;

		void* Alloc(std::size_t size);
		void Free(void* ptr);
		MemoryBlock* NewMemoryBlock();
		void Recycle(MemoryBlock*);
	};

#endif