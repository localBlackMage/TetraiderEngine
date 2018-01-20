#include "PhysicsManager.h"
#include "Body.h"
#include "GameObject.h"
#include "../Source/Math/Collisions.h"

PhysicsManager::PhysicsManager() {
	CollisionFunctions[ST_Circle][ST_Circle] = StaticCircleToStaticCircle;
	CollisionFunctions[ST_Circle][ST_AABB] = StaticCircleToStaticAABB;
	CollisionFunctions[ST_AABB][ST_Circle] = StaticAABBToStaticCircle;
	CollisionFunctions[ST_AABB][ST_AABB] = StaticAABBToStaticAABB;
}

PhysicsManager::~PhysicsManager() {
	m_gameObjects.clear();
}

void PhysicsManager::Integrate(float dt) {
	int size = m_gameObjects.size();
	for (int i = 0; i < size; ++i) {
		Body* c = static_cast<Body*>(m_gameObjects[i]->GetComponent(ComponentType::Body));
		c->Integrate(dt);
	}
}

void PhysicsManager::AddGameObject(GameObject* pGO) {
	m_gameObjects.push_back(pGO);
}

void PhysicsManager::RemoveGameObject(GameObject* pGO) {
	std::vector<GameObject*>::iterator it = std::find(m_gameObjects.begin(), m_gameObjects.end(), pGO);
	if (it != m_gameObjects.end())
		m_gameObjects.erase(it);
}

void PhysicsManager::ResolveCollisions() {
	CheckCollisionsAndGenerateContacts();
	FireEventsToContacts();
	ClearContacts();
}

void PhysicsManager::FireEventsToContacts() {
	//TODO fire events to game objects
}

void PhysicsManager::ClearContacts() {
	for (auto contact : m_pContacts)
		delete contact;

	m_pContacts.clear();
}

void PhysicsManager::CheckCollisionsAndGenerateContacts() {
	if (m_gameObjects.size() <= 1)
		return;

	for (unsigned int i = 0; i < m_gameObjects.size() - 1; ++i) {
		Body* pBodyA = static_cast<Body*>(m_gameObjects[i]->GetComponent(ComponentType::Body));

		if (pBodyA->pGO->m_isCollisionDisabled)
			continue;

		for (unsigned int j = i + 1; j < m_gameObjects.size(); ++j) {
			Body* pBodyB = static_cast<Body*>(m_gameObjects[j]->GetComponent(ComponentType::Body));

			if (pBodyB->pGO->m_isCollisionDisabled)
				continue;

			if (pBodyA->m_isStatic && pBodyB->m_isStatic)
				continue;

			if (CollisionFunctions[pBodyA->m_pShape->type][pBodyB->m_pShape->type](pBodyA, pBodyB)) {
				GenerateContact(pBodyA, pBodyB);
			}
		}
	}
}

// TODO add extra data like normals and penetration depth
void PhysicsManager::GenerateContact(Body* pBodyA, Body* pBodyB) {
	Contact* pContact = new Contact();
	pContact->pBody[0] = pBodyA;
	pContact->pBody[1] = pBodyB;
	m_pContacts.push_back(pContact);
}

bool StaticCircleToStaticCircle(Body* pBodyA, Body* pBodyB) {
	return false;
}

bool StaticCircleToStaticAABB(Body* pBodyA, Body* pBodyB) {
	return false;
}

bool StaticAABBToStaticCircle(Body* pBodyA, Body* pBodyB) {
	return false;
}

bool StaticAABBToStaticAABB(Body* pBodyA, Body* pBodyB) {
	return false;
}