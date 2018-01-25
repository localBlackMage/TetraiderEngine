#include "PhysicsManager.h"
#include "Body.h"
#include "GameObject.h"

PhysicsManager::PhysicsManager() {
	CollisionFunctions[ST_Circle][ST_Circle] = StaticCircleToStaticCircle;
	CollisionFunctions[ST_Circle][ST_AABB] = StaticCircleToStaticAABB;
	CollisionFunctions[ST_AABB][ST_Circle] = StaticAABBToStaticCircle;
	CollisionFunctions[ST_AABB][ST_AABB] = StaticAABBToStaticAABB;
	CollisionFunctions[ST_POLYGON][ST_POLYGON] = StaticPolygonToStaticPolygon;
	CollisionFunctions[ST_POLYGON][ST_Circle] = StaticPolygonToStaticCircle;
	CollisionFunctions[ST_POLYGON][ST_AABB] = StaticPolygonToStaticAABB;
	CollisionFunctions[ST_AABB][ST_POLYGON] = StaticAABBToStaticPolygon;
	CollisionFunctions[ST_Circle][ST_POLYGON] = StaticCircleToStaticPolygon;
	RayCastFunctions[ST_AABB] = RayCastToAABB;
	RayCastFunctions[ST_Circle] = RayCastToCircle;
	RayCastFunctions[ST_POLYGON] = RayCastToPolygon;
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
	for (auto contact : m_pContacts) {
		OnCollide onCollide;
		onCollide.mtv.normal = contact->m_MTV.normal;
		onCollide.mtv.penetration = contact->m_MTV.penetration;
		onCollide.pGO = contact->m_pBodies[0]->pGO;

		Vector3D directionOfCenters = contact->m_pBodies[1]->GetPosition() - contact->m_pBodies[0]->GetPosition();
		if (Vector3D::Dot(directionOfCenters, contact->m_MTV.normal) < 0)
			onCollide.mtv.normal = -1 * onCollide.mtv.normal;

		contact->m_pBodies[1]->pGO->HandleEvent(&onCollide);

		onCollide.pGO = contact->m_pBodies[1]->pGO;
		onCollide.mtv.normal = -1 * onCollide.mtv.normal;
		contact->m_pBodies[0]->pGO->HandleEvent(&onCollide);
	}
}

void PhysicsManager::ClearContacts() {
	for (auto contact : m_pContacts)
		delete contact;

	m_pContacts.clear();
}

void PhysicsManager::CheckCollisionsAndGenerateContacts() {
	if (m_gameObjects.size() <= 1) return;

	for (unsigned int i = 0; i < m_gameObjects.size() - 1; ++i) {
		Body* pBodyA = static_cast<Body*>(m_gameObjects[i]->GetComponent(ComponentType::Body));

		if (pBodyA->pGO->m_isCollisionDisabled) continue;

		for (unsigned int j = i + 1; j < m_gameObjects.size(); ++j) {
			Body* pBodyB = static_cast<Body*>(m_gameObjects[j]->GetComponent(ComponentType::Body));

			if (pBodyB->pGO->m_isCollisionDisabled) continue;

			if (pBodyA->m_isStatic && pBodyB->m_isStatic) continue;

			MTV mtv;
			if (CollisionFunctions[pBodyA->m_pShape->type][pBodyB->m_pShape->type](pBodyA, pBodyB, &mtv))
				GenerateContact(pBodyA, pBodyB, &mtv);
		}
	}
}

void PhysicsManager::GenerateContact(Body* pBodyA, Body* pBodyB, MTV* pMTV) {
	Contact* pContact = new Contact();
	pContact->m_pBodies[0] = pBodyA;
	pContact->m_pBodies[1] = pBodyB;
	pContact->m_MTV.normal = pMTV->normal;
	pContact->m_MTV.penetration = pMTV->penetration;
	m_pContacts.push_back(pContact);
}

bool PhysicsManager::Raycast(const LineSegment2D& ray, const GameObjectTag* pIgnoreLayer, int layerSize) {
	for (unsigned int i = 0; i < m_gameObjects.size(); ++i) {
		Body* pBody = static_cast<Body*>(m_gameObjects[i]->GetComponent(ComponentType::Body));
		
		bool isIgnore = false;
		for (int i = 0; i < layerSize; ++i) {
			if (pBody->pGO->m_tag == pIgnoreLayer[i]) {
				isIgnore = true;
				break;
			}
		}

		if (isIgnore) continue;
		if (pBody->pGO->m_isCollisionDisabled) continue;

		if (RayCastFunctions[pBody->m_pShape->type](ray, pBody))
			return true;
	}

	return false;
}

bool StaticCircleToStaticCircle(Body* pBodyA, Body* pBodyB, MTV* p_mtv) {
	Circle *pCircle1 = static_cast<Circle*>(pBodyA->m_pShape);
	Circle *pCircle2 = static_cast<Circle*>(pBodyB->m_pShape);
	return StaticCircleToStaticCircle(pBodyA->GetPosition(), pCircle1->radius, pBodyB->GetPosition(), pCircle2->radius, *p_mtv);
}

bool StaticCircleToStaticAABB(Body* pBodyA, Body* pBodyB, MTV* p_mtv) {
	Circle *pCircle = static_cast<Circle*>(pBodyA->m_pShape);
	AABB *pRect = static_cast<AABB*>(pBodyB->m_pShape);
	return StaticCircleToStaticRect(pBodyA->GetPosition(), pCircle->radius, pBodyB->GetPosition(), pRect->halfWidth, pRect->halfHeight, *p_mtv);
}

bool StaticAABBToStaticCircle(Body* pBodyA, Body* pBodyB, MTV* p_mtv) {
	AABB *pRect = static_cast<AABB*>(pBodyA->m_pShape);
	Circle *pCircle = static_cast<Circle*>(pBodyB->m_pShape);
	return StaticCircleToStaticRect(pBodyB->GetPosition(), pCircle->radius, pBodyA->GetPosition(), pRect->halfWidth, pRect->halfHeight, *p_mtv);
}

bool StaticAABBToStaticAABB(Body* pBodyA, Body* pBodyB, MTV* p_mtv) {
	AABB *pRect1 = static_cast<AABB*>(pBodyA->m_pShape);
	AABB *pRect2 = static_cast<AABB*>(pBodyB->m_pShape);
	return StaticRectToStaticRect(pBodyA->GetPosition(), pRect1->halfWidth, pRect1->halfHeight, pBodyB->GetPosition(), pRect2->halfWidth, pRect2->halfHeight, *p_mtv);
}

bool StaticPolygonToStaticPolygon(Body* pBodyA, Body* pBodyB, MTV* p_mtv) {
	Polygon* pPoly1 = static_cast<Polygon*>(pBodyA->m_pShape);
	Polygon* pPoly2 = static_cast<Polygon*>(pBodyB->m_pShape);
	return SeperatingAxisTheorom::SAT(pBodyA->GetPosition(), pPoly1->m_vertices, pBodyB->GetPosition(), pPoly2->m_vertices, *p_mtv);
}

bool StaticPolygonToStaticAABB(Body* pBodyA, Body* pBodyB, MTV* p_mtv) {
	Polygon* pPoly = static_cast<Polygon*>(pBodyA->m_pShape);
	AABB *pRect = static_cast<AABB*>(pBodyB->m_pShape);
	return StaticPolygonToStaticAABB(pBodyA->GetPosition(), pPoly->m_vertices, pBodyB->GetPosition(), pRect->halfWidth, pRect->halfHeight, *p_mtv);
}

bool StaticAABBToStaticPolygon(Body* pBodyA, Body* pBodyB, MTV* p_mtv) {
	Polygon* pPoly = static_cast<Polygon*>(pBodyB->m_pShape);
	AABB *pRect = static_cast<AABB*>(pBodyA->m_pShape);
	return StaticPolygonToStaticAABB(pBodyB->GetPosition(), pPoly->m_vertices, pBodyA->GetPosition(), pRect->halfWidth, pRect->halfHeight, *p_mtv);;
}

bool StaticPolygonToStaticCircle(Body* pBodyA, Body* pBodyB, MTV* p_mtv) {
	Polygon* pPoly = static_cast<Polygon*>(pBodyA->m_pShape);
	Circle *pCircle = static_cast<Circle*>(pBodyB->m_pShape);
	return StaticPolygonToStaticCircle(pBodyA->GetPosition(), pPoly->m_vertices, pBodyB->GetPosition(), pCircle->radius, *p_mtv);
}

bool StaticCircleToStaticPolygon(Body* pBodyA, Body* pBodyB, MTV* p_mtv) {
	Polygon* pPoly = static_cast<Polygon*>(pBodyB->m_pShape);
	Circle *pCircle = static_cast<Circle*>(pBodyA->m_pShape);
	return StaticPolygonToStaticCircle(pBodyB->GetPosition(), pPoly->m_vertices, pBodyA->GetPosition(), pCircle->radius, *p_mtv);
}

bool RayCastToAABB(const LineSegment2D& ray, Body* pBody) {
	AABB *pRect = static_cast<AABB*>(pBody->m_pShape);
	return StaticAABBToRay(pBody->GetPosition(), pRect->halfWidth, pRect->halfHeight, ray);
}

bool RayCastToCircle(const LineSegment2D& ray, Body* pBody) {
	Circle *pCircle = static_cast<Circle*>(pBody->m_pShape);
	return StaticCircleToRay(pBody->GetPosition(), pCircle->radius, ray);
}

bool RayCastToPolygon(const LineSegment2D& ray, Body* pBody) {
	Polygon* pPoly = static_cast<Polygon*>(pBody->m_pShape);
	return StaticPolygonToRay(pBody->GetPosition(), pPoly->m_vertices, ray);
}