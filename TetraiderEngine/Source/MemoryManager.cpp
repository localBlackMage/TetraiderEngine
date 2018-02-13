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
#include "TetraiderAPI.h"
#include "Component.h"
#include "Transform.h"
#include "Sprite.h"
#include "Animation.h"
#include "Controller.h"
#include "Body.h"
#include "Camera.h"
#include "CamFollow.h"
#include "Health.h"
#include "FlashOnTakeDamage.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Audio.h"
#include "NPCController.h"
#include "DestroyOnHealthZero.h"
#include "DealDamageOnCollision.h"
#include <iostream>

// temp
#include <conio.h>
#include <chrono>
#include <vector>

//MemoryManager* gMemoryManager;

static void PrintLinkedList(MemoryBlock* node) {
	if (!node) {
		printf("\n\nFINISHED\n\n");
		return;
	}

	printf("MEMORY BLOCK :: node: %p :: pData: %p :: Size: %d :: FreeSize: %d :: Next: %p :: Prev: %p\n", node, node->pData, node->size, node->freesize, node->next, node->prev);
	PrintLinkedList(node->next);
}

static void PrintPointer(const char* msg, void* ptr) {
	printf("%s: %p\n", msg, ptr);
}

static void PrintMemoryBlock(MemoryBlock* block) {
	PrintPointer("Next MB", block->next);
	if (block->next)
		PrintPointer("Next->Prev MB", block->next->prev);
	PrintPointer("Prev MB", block->prev);
	printf("Block Size: %d : Block Freesize: %d\n", block->size, block->freesize);
}

MemoryManager::MemoryManager():
	m_TotalBufferSize(DEFAULT_BUFFER_SIZE_BYTE), m_pHead(nullptr) {

	m_Buffer = malloc(m_TotalBufferSize);
	m_pHead = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	m_pHead->Initialize(m_Buffer, 0, m_TotalBufferSize);
	m_NumCachedBlock = 0;

}

MemoryManager::~MemoryManager() {

	if (m_pHead){
		m_pHead->CleanUp();
		free(m_pHead);
		m_pHead = nullptr;
	}
	free(m_Buffer);
}

void* MemoryManager::Alloc(std::size_t size) {
	MemoryBlock *current = m_pHead;
	// find next available slot 
	while (current && (current->freesize < size)) {
		current = current->next;
	}

	// request failed
	if (!current) {
		printf("MemoryManager::Alloc() FAILED - No Available Slot\n");
		return nullptr;
	}
	// create a new memoryblock and insert into the list
	MemoryBlock* temp = current->next;
	current->next = NewMemoryBlock();
	current->next->Initialize(	static_cast<unsigned char*>(current->pData) + current->size, 
								size, 
								current->freesize - size );

	if (temp) 
		temp->prev = current->next;
	current->next->next = temp;
	current->freesize = 0;
	current->next->prev = current;

	return current->next->pData;
}

void MemoryManager::Free(void* ptr){
	MemoryBlock *current = m_pHead->next;
	if (!current || (current->pData == nullptr)) {
		printf("MEMMNGR::FREE() NEVER REACH HERE!\n");
		return;
	}

	// TODO: remove while-loop to O(1) of block node find
	//		 use unordered_map<?,MemoryBlock*>
	//						  <?,std::shared_ptr<MemoryBlock> >
	// ? = reinterpret_case<unsigned long>
	while(current && (current->pData != ptr)){
		current = current->next;
	}

	if (!current) {
		printf("MEMMNGR::FREE() NEVER REACH HERE --2 !!!\n");
		return;
	}
	
	// remove links
	if (current->next)
		current->next->prev = current->prev;
	if (current->prev) {
		current->prev->next = current->next;
		current->prev->freesize += current->size + current->freesize;
	}
	current->freesize = 0;

	current->next = nullptr;
	current->prev = nullptr;

	// try caching current block. If not, free.
	Recycle(current);
}

 MemoryBlock* MemoryManager::NewMemoryBlock(){
	if (m_NumCachedBlock == 0) { // no cached memoryblock to be reused
		return (MemoryBlock*)malloc(sizeof(MemoryBlock));
	}
	else {									// use previously recycled memory block
		return m_Cache[--m_NumCachedBlock]; // NOTE: cacheIndex = m_NumCachedBlock-1
	}
}

 void MemoryManager::Recycle(MemoryBlock* pNode){
	if (m_NumCachedBlock == MAX_CACHE_SIZE_NUM-1){
		//printf("CACHE IS FULL -- MAX_CACHE_SIZE_NUM: %d\n", MAX_CACHE_SIZE_NUM);
		free(pNode);
		return;
	}
	m_Cache[m_NumCachedBlock++] = pNode;
}

void* MemoryManager::m_Buffer = nullptr;

GameObject* MemoryManager::GetNewGameObject(unsigned int id) {
	if (m_GameObjectCache.empty()) {
		return new GameObject(id);
	}
	else {
		GameObject* emptyGO = m_GameObjectCache.back();
		m_GameObjectCache.pop_back();
		return emptyGO;
	}
}
void MemoryManager::DeleteGameObject(GameObject* ptr) {
	if (m_GameObjectCache.size() < MAX_GAMEOBJECT_CACHE) {
		delete ptr;
	}
	else {
		m_GameObjectCache.push_back(ptr);
	}
}

Component* MemoryManager::GetNewComponent(ComponentType type) {
	if (m_ComponentCache[type].empty()) {
		switch (type) {
		case C_Transform:
			return new Transform();
		case C_Sprite:
			return new Sprite();
		case C_Animation:
			return new Animation();
		case C_Controller:
			return new Controller();
		case C_Body:
			return new Body();
		case C_Camera:
			return new Camera();
		case C_CamFollow:
			return new CamFollow();
		case C_Health:
			return new Health();
		case C_FlashOnTakeDamage:
			return new FlashOnTakeDamage();
		case C_Projectile:
			return new Projectile();
		case C_Weapon:
			return new Weapon();
		case C_Audio:
			return new Audio();
		case C_NPCCONTROLLER:
			return new NPCController();
		case C_ParticleEmitter:
			//return new ParticleEmitter();
			break;
		case C_DestroyOnHealthZero:
			return new DestroyOnHealthZero();
		case C_DealDamageOnCollision:
			return new DealDamageOnCollision();
		}
		return nullptr;
	}
	else { // currently no caching for components
		Component* emptyComp = m_ComponentCache[type].front();
		m_ComponentCache[type].erase(m_ComponentCache[type].begin());
		return emptyComp;
	}
}
void MemoryManager::DeleteComponent(Component* ptr) {
	
	delete ptr;

}
/* REFERENCE OF HOW NEW DELETE IS DEFINED IN SUBSCRIBER CLASS */
// 
//void* MemoryManager::operator new(std::size_t size)
//{
//	return TETRA_MEMORY.Alloc(size);
//}
//void MemoryManager::operator delete(void *ptr)
//{
//	TETRA_MEMORY.Free(ptr);
//}
//void* MemoryManager::operator new[](std::size_t size)
//{
//	return TETRA_MEMORY.Alloc(size);
//}
//void MemoryManager::operator delete[](void* arrayPtr)
//{
//	TETRA_MEMORY.Free(arrayPtr);
//}
