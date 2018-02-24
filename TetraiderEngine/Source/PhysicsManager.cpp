#include "PhysicsManager.h"
#include "GameObject.h"
#include "ComponentTypes.h"
#include "Body.h"
#include "TetraiderAPI.h"

PhysicsManager::PhysicsManager(): m_isSpatialParitioning(false) {
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

	//m_isSpatialParitioning = true;
}

PhysicsManager::~PhysicsManager() {
	m_gameObjects.clear();
}

void PhysicsManager::Integrate(float dt) {
	int size = m_gameObjects.size();
	for (int i = 0; i < size; ++i) {
		if (m_gameObjects[i]->m_isActive) {
			Body* c = m_gameObjects[i]->GetComponent<Body>(ComponentType::C_Body);
			c->Integrate(dt);
		}
	}
}

void PhysicsManager::AddGameObject(GameObject* pGO) {
	m_gameObjects.push_back(pGO);
}

void PhysicsManager::RemoveGameObject(GameObject* pGO) {
	if (m_gameObjects.size() > 0) {
		std::vector<GameObject*>::iterator it = std::find(m_gameObjects.begin(), m_gameObjects.end(), pGO);
		if (it != m_gameObjects.end())
			m_gameObjects.erase(it);
	}
}

void PhysicsManager::ResolveCollisions() {
	if (m_isSpatialParitioning) {
		UpdateGrid();
		m_physicsGrid.DebugGrid();
	}

	CheckCollisionsAndGenerateContacts();
	FireEventsToContacts();
	ClearContacts();
}

void PhysicsManager::FireEventsToContacts() {
	for (auto contact : m_pContacts) {
		OnCollideData dataFirst(contact->m_pBodies[0]->pGO, MTV(contact->m_MTV.normal, contact->m_MTV.penetration));

		Vector3D directionOfCenters = contact->m_pBodies[1]->GetPosition() - contact->m_pBodies[0]->GetPosition();
		if (Vector3D::Dot(directionOfCenters, contact->m_MTV.normal) < 0)
			dataFirst.mtv.normal = -1 * dataFirst.mtv.normal;

		contact->m_pBodies[1]->pGO->HandleEvent(&Event(EventType::EVENT_OnCollide, &dataFirst));

		dataFirst.mtv.normal = -1* dataFirst.mtv.normal;
		dataFirst.pGO = contact->m_pBodies[1]->pGO;

		contact->m_pBodies[0]->pGO->HandleEvent(&Event(EventType::EVENT_OnCollide, &dataFirst));
	}
}

void PhysicsManager::ClearContacts() {
	for (auto contact : m_pContacts)
		delete contact;

	m_pContacts.clear();
}

void PhysicsManager::CheckCollisionsAndGenerateContacts() {
	if (m_gameObjects.size() <= 1) return;

	if (!m_isSpatialParitioning) {
		for (unsigned int i = 0; i < m_gameObjects.size() - 1; ++i) {
			Body* pBodyA = m_gameObjects[i]->GetComponent<Body>(ComponentType::C_Body);

			if (pBodyA->pGO->m_isCollisionDisabled) continue;

			for (unsigned int j = i + 1; j < m_gameObjects.size(); ++j) {
				Body* pBodyB = m_gameObjects[j]->GetComponent<Body>(ComponentType::C_Body);

				if (pBodyB->pGO->m_isCollisionDisabled) continue;

				if (pBodyA->m_isStatic && pBodyB->m_isStatic) continue;
				MTV mtv;
				if (CollisionFunctions[pBodyA->m_pShape->type][pBodyB->m_pShape->type](*pBodyA, *pBodyB, &mtv))
					GenerateContact(*pBodyA, *pBodyB, &mtv);
			}
		}
	}
	else {
		std::vector<std::pair<int, int>> neighbours;
		for (int i = 0; i < m_physicsGrid.m_gridSize.second; ++i) {
			for (int j = 0; j < m_physicsGrid.m_gridSize.first; ++j) {
				if (m_physicsGrid.m_ppPhysicsNode[i][j].m_gameObjects.size() <= 1) continue;

				for (unsigned int k = 0; k < m_physicsGrid.m_ppPhysicsNode[i][j].m_gameObjects.size() - 1; ++k) {
					const Body* pBodyA = m_physicsGrid.m_ppPhysicsNode[i][j].m_gameObjects[k]->GetComponent<Body>(ComponentType::C_Body);

					if (pBodyA->pGO->m_isCollisionDisabled) continue;
					// Check for collisions between pairs in the same node:
					for (unsigned int w = k + 1; w < m_physicsGrid.m_ppPhysicsNode[i][j].m_gameObjects.size(); ++w) {
						const Body* pBodyB = m_physicsGrid.m_ppPhysicsNode[i][j].m_gameObjects[w]->GetComponent<Body>(ComponentType::C_Body);

						if (pBodyA->pGO->m_isCollisionDisabled) continue;
						if (pBodyA->m_isStatic && pBodyB->m_isStatic) continue;
						MTV mtv;

						if (CollisionFunctions[pBodyA->m_pShape->type][pBodyB->m_pShape->type](*pBodyA, *pBodyB, &mtv))
							GenerateContact(*pBodyA, *pBodyB, &mtv);
					}
				}
				// Check for collisions between neighbour nodes:
				m_physicsGrid.SetNeighbours(neighbours, i, j);
				for (unsigned int k = 0; k < m_physicsGrid.m_ppPhysicsNode[i][j].m_gameObjects.size(); ++k) {
					const Body* pBodyA = m_physicsGrid.m_ppPhysicsNode[i][j].m_gameObjects[k]->GetComponent<Body>(ComponentType::C_Body);

					if (pBodyA->pGO->m_isCollisionDisabled) continue;
					for (unsigned int w = 0; w < neighbours.size(); w++) {
						for (unsigned int x = 0; x < m_physicsGrid.m_ppPhysicsNode[neighbours[w].first][neighbours[w].second].m_gameObjects.size(); ++x) {
							const Body* pBodyB = m_physicsGrid.m_ppPhysicsNode[neighbours[w].first][neighbours[w].second].m_gameObjects[x]->GetComponent<Body>(ComponentType::C_Body);

							if (pBodyA->pGO->m_isCollisionDisabled) continue;
							if (pBodyA->m_isStatic && pBodyB->m_isStatic) continue;
							MTV mtv;

							if (CollisionFunctions[pBodyA->m_pShape->type][pBodyB->m_pShape->type](*pBodyA, *pBodyB, &mtv))
								GenerateContact(*pBodyA, *pBodyB, &mtv);
						}
					}
				}
			}
		}
	}
}

void PhysicsManager::GenerateContact(const Body& pBodyA, const Body& pBodyB, MTV* pMTV) {
	Contact* pContact = new Contact();
	pContact->m_pBodies[0] = &pBodyA;
	pContact->m_pBodies[1] = &pBodyB;
	pContact->m_MTV.normal = pMTV->normal;
	pContact->m_MTV.penetration = pMTV->penetration;
	m_pContacts.push_back(pContact);
}

bool PhysicsManager::Raycast(const LineSegment2D& ray, const GameObjectTag* pIgnoreLayer, int layerSize) {
	for (unsigned int i = 0; i < m_gameObjects.size(); ++i) {
		Body* pBody = m_gameObjects[i]->GetComponent<Body>(ComponentType::C_Body);
		
		bool isIgnore = false;
		for (int i = 0; i < layerSize; ++i) {
			if (pBody->pGO->m_tag == pIgnoreLayer[i]) {
				isIgnore = true;
				break;
			}
		}

		if (isIgnore) continue;
		if (pBody->pGO->m_isCollisionDisabled) continue;

		if (RayCastFunctions[pBody->m_pShape->type](ray, *pBody))
			return true;
	}

	return false;
}

void PhysicsGrid::SetNeighbours(std::vector<std::pair<int, int>>& neighbours, int r, int c) {
	neighbours.clear();
	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			if (i == 0 && j == 0)
				continue;

			int checkA = r + i;
			int checkB = c + j;
			if (checkA < 0 || checkA >= m_gridSize.first || checkB < 0 || checkB >= m_gridSize.second)
				continue;

			if (m_ppPhysicsNode[checkA][checkB].m_isChecked)
				continue;

			std::pair<int, int> neighbour;
			neighbour.first = checkA;
			neighbour.second = checkB;
			neighbours.push_back(neighbour);
		}
	}
}

void PhysicsManager::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EVENT_OnLevelInitialized: {
			m_physicsGrid.SetUpGrid(5, 5, 0, 0, 500);
			break;
		}
	}
}

PhysicsGrid::PhysicsGrid() {
	m_gridSize.first = 0;
	m_gridSize.second = 0;
	m_startPosition.Set(-1000, -1000, 0);
}

PhysicsGrid::~PhysicsGrid() {
	DeleteGrid();
}

void PhysicsGrid::SetUpGrid(int row, int column, float xOffset, float yOffset, int nodeSize) {
	if (!m_ppPhysicsNode)
		DeleteGrid();

	m_gridSize.first = row;
	m_gridSize.second = column;
	m_nodeSize = nodeSize;
	m_gridHeight = (float)(nodeSize * row);
	m_gridWidth = (float)(nodeSize * column);
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	m_startPosition.x += xOffset;
	m_startPosition.y += yOffset;

	m_ppPhysicsNode = new PhysicsNode* [column];
	for (int i = 0; i < column; ++i) {
		m_ppPhysicsNode[i] = new PhysicsNode[row];
	}
}

void PhysicsGrid::DeleteGrid() {
	for (int i = 0; i < m_gridSize.second; ++i) {
		delete []m_ppPhysicsNode[i];
	}

	delete[] m_ppPhysicsNode;
	m_ppPhysicsNode = nullptr;
}

void PhysicsGrid::ClearGrid() {
	for (int i = 0; i < m_gridSize.second; ++i) {
		for (int j = 0; j < m_gridSize.first; ++j) {
			m_ppPhysicsNode[i][j].m_gameObjects.clear();
			m_ppPhysicsNode[i][j].m_isChecked = false;
		}
	}
}

void PhysicsGrid::DebugGrid() {
	for (int i = 0; i < m_gridSize.second; ++i) {
		for (int j = 0; j < m_gridSize.first; ++j) {
			if(m_ppPhysicsNode[i][j].m_gameObjects.size() == 0)
				TETRA_DEBUG.DrawWireRectangle(Vector3D(m_startPosition.x + (float)(i*m_nodeSize),m_startPosition.y + (float)(j*m_nodeSize), 0) , Vector3D(0,0,0), Vector3D((float)m_nodeSize - 3, (float)m_nodeSize - 3, 0), DebugColor::CYAN);
			else
				TETRA_DEBUG.DrawWireRectangle(Vector3D(m_startPosition.x + (float)(i*m_nodeSize), m_startPosition.y + (float)(j*m_nodeSize), 0), Vector3D(0, 0, 0), Vector3D((float)m_nodeSize - 3, (float)m_nodeSize - 3, 0), DebugColor::RED);
		}
	}
}

void PhysicsManager::UpdateGrid() {
	m_physicsGrid.ClearGrid();
	for (unsigned int i = 0; i < m_gameObjects.size(); ++i) {
		m_physicsGrid.PlaceInNode(*m_gameObjects[i]);
	}
}

void PhysicsGrid::PlaceInNode(const GameObject& go) {
	// Place the game object in node
	const Body* pBody = go.GetComponent<Body>(C_Body);
	Vector3D pos = pBody->GetPosition();

	float percentX = (pos.x - m_xOffset + m_gridWidth / 2.0f) / m_gridWidth;
	float percentY = (pos.y - m_yOffset + m_gridHeight / 2.0f) / m_gridHeight;

	if (percentX > 1 || percentY > 1 || percentX < 0 || percentY < 0) {
		std::cout << "Body component outside grid. Increase grid size" << std::endl;
		return;
	}

	m_ppPhysicsNode[(int)(floor(percentX*m_gridSize.second))][(int)(floor(percentY*m_gridSize.first))].m_gameObjects.push_back(&go);
}

bool StaticCircleToStaticCircle(const Body& pBodyA, const Body& pBodyB, MTV* p_mtv) {
	Circle *pCircle1 = static_cast<Circle*>(pBodyA.m_pShape);
	Circle *pCircle2 = static_cast<Circle*>(pBodyB.m_pShape);
	return StaticCircleToStaticCircle(pBodyA.GetPosition(), pCircle1->radius, pBodyB.GetPosition(), pCircle2->radius, *p_mtv);
}

bool StaticCircleToStaticAABB(const Body& pBodyA, const Body& pBodyB, MTV* p_mtv) {
	Circle *pCircle = static_cast<Circle*>(pBodyA.m_pShape);
	AABB *pRect = static_cast<AABB*>(pBodyB.m_pShape);
	return StaticCircleToStaticRect(pBodyA.GetPosition(), pCircle->radius, pBodyB.GetPosition(), pRect->halfWidth, pRect->halfHeight, *p_mtv);
}

bool StaticAABBToStaticCircle(const Body& pBodyA, const Body& pBodyB, MTV* p_mtv) {
	AABB *pRect = static_cast<AABB*>(pBodyA.m_pShape);
	Circle *pCircle = static_cast<Circle*>(pBodyB.m_pShape);
	return StaticCircleToStaticRect(pBodyB.GetPosition(), pCircle->radius, pBodyA.GetPosition(), pRect->halfWidth, pRect->halfHeight, *p_mtv);
}

bool StaticAABBToStaticAABB(const Body& pBodyA, const Body& pBodyB, MTV* p_mtv) {
	AABB *pRect1 = static_cast<AABB*>(pBodyA.m_pShape);
	AABB *pRect2 = static_cast<AABB*>(pBodyB.m_pShape);
	return StaticRectToStaticRect(pBodyA.GetPosition(), pRect1->halfWidth, pRect1->halfHeight, pBodyB.GetPosition(), pRect2->halfWidth, pRect2->halfHeight, *p_mtv);
}

bool StaticPolygonToStaticPolygon(const Body& pBodyA, const Body& pBodyB, MTV* p_mtv) {
	Polygon* pPoly1 = static_cast<Polygon*>(pBodyA.m_pShape);
	Polygon* pPoly2 = static_cast<Polygon*>(pBodyB.m_pShape);
	return SeperatingAxisTheorom::SAT(pBodyA.GetPosition(), pPoly1->m_vertices, pBodyB.GetPosition(), pPoly2->m_vertices, *p_mtv);
}

bool StaticPolygonToStaticAABB(const Body& pBodyA, const Body& pBodyB, MTV* p_mtv) {
	Polygon* pPoly = static_cast<Polygon*>(pBodyA.m_pShape);
	AABB *pRect = static_cast<AABB*>(pBodyB.m_pShape);
	return StaticPolygonToStaticAABB(pBodyA.GetPosition(), pPoly->m_vertices, pBodyB.GetPosition(), pRect->halfWidth, pRect->halfHeight, *p_mtv);
}

bool StaticAABBToStaticPolygon(const Body& pBodyA, const Body& pBodyB, MTV* p_mtv) {
	Polygon* pPoly = static_cast<Polygon*>(pBodyB.m_pShape);
	AABB *pRect = static_cast<AABB*>(pBodyA.m_pShape);
	return StaticPolygonToStaticAABB(pBodyB.GetPosition(), pPoly->m_vertices, pBodyA.GetPosition(), pRect->halfWidth, pRect->halfHeight, *p_mtv);;
}

bool StaticPolygonToStaticCircle(const Body& pBodyA, const Body& pBodyB, MTV* p_mtv) {
	Polygon* pPoly = static_cast<Polygon*>(pBodyA.m_pShape);
	Circle *pCircle = static_cast<Circle*>(pBodyB.m_pShape);
	return StaticPolygonToStaticCircle(pBodyA.GetPosition(), pPoly->m_vertices, pBodyB.GetPosition(), pCircle->radius, *p_mtv);
}

bool StaticCircleToStaticPolygon(const Body& pBodyA, const Body& pBodyB, MTV* p_mtv) {
	Polygon* pPoly = static_cast<Polygon*>(pBodyB.m_pShape);
	Circle *pCircle = static_cast<Circle*>(pBodyA.m_pShape);
	return StaticPolygonToStaticCircle(pBodyB.GetPosition(), pPoly->m_vertices, pBodyA.GetPosition(), pCircle->radius, *p_mtv);
}

bool RayCastToAABB(const LineSegment2D& ray, const Body& pBody) {
	AABB *pRect = static_cast<AABB*>(pBody.m_pShape);
	return StaticAABBToRay(pBody.GetPosition(), pRect->halfWidth, pRect->halfHeight, ray);
}

bool RayCastToCircle(const LineSegment2D& ray, const Body& pBody) {
	Circle *pCircle = static_cast<Circle*>(pBody.m_pShape);
	return StaticCircleToRay(pBody.GetPosition(), pCircle->radius, ray);
}

bool RayCastToPolygon(const LineSegment2D& ray, const Body& pBody) {
	Polygon* pPoly = static_cast<Polygon*>(pBody.m_pShape);
	return StaticPolygonToRay(pBody.GetPosition(), pPoly->m_vertices, ray);
}