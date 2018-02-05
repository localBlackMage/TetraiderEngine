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

	class MemoryManagerInterface {
	public:
		virtual void* Alloc(std::size_t) = 0;
		virtual void Free(void*) = 0;
	};

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
	}MemoryBlock;



	class MemoryManager : public MemoryManagerInterface {
	private:
		static void* m_Buffer;
		const size_t m_TotalBufferSize;
		MemoryBlock* m_pHead;
		MemoryBlock* m_Cache[MAX_CACHE_SIZE_NUM];
		int m_NumCachedBlock;
	public:

		MemoryManager();
		virtual ~MemoryManager();
		MemoryManager(const MemoryManager &) = delete;
		void operator=(const MemoryManager &) = delete;

		virtual void* Alloc(std::size_t size);
		virtual void Free(void* ptr);
		MemoryBlock* NewMemoryBlock();
		void Recycle(MemoryBlock*);
	};

	extern MemoryManager* gMemoryManager;

#endif