/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

class PhysicsManager;

struct Contact {
	const Body* m_pBodies[2];
	MTV m_MTV;
};

struct PhysicsNode {
	PhysicsNode() : m_isChecked(false) {}
	std::vector<const GameObject*> m_gameObjects;
	bool m_isChecked;
};

class PhysicsGrid {
public:
	PhysicsGrid();
	~PhysicsGrid();
	void SetUpGrid(int row, int column, float xOffset, float yOffset, int nodeSize);
	void ClearGrid();
	void DebugGrid();
	void DeleteGrid();
	void PlaceInNode(const GameObject&);
	void SetNeighbours(std::vector<std::pair<int, int>>& neighbours, int r, int c);
	friend PhysicsManager;
private:
	std::pair<int, int> m_gridSize;  // row by column
	float m_gridWidth; // In pixels
	float m_gridHeight; // In pixels
	float m_xOffset;
	float m_yOffset;
	int m_nodeSize;
	PhysicsNode** m_ppPhysicsNode;
	Vector3D m_startPosition;
};

class PhysicsManager : public Subscriber
{
public:
	PhysicsManager();
	~PhysicsManager();
	PhysicsManager(const PhysicsManager &) = delete;
	void operator=(const PhysicsManager &) = delete;

	void Integrate(float dt);
	void ResolveCollisions();
	void AddGameObject(GameObject* pGO);
	void RemoveGameObject(GameObject* pGO);
	void CheckCollisionsAndGenerateContacts();
	void HandleEvent(Event* pEvent);
	void FireEventsToContacts();
	void ClearContacts();
	bool Raycast(const LineSegment2D& ray, const GameObjectTag* pIgnoreLayer, int layerSize, GameObject* rayCastSource = nullptr);
	
	std::vector<GameObject*> m_gameObjects;
private:
	bool(*CollisionFunctions[ST_Count][ST_Count])(const Body&,const Body&, MTV*);
	bool(*RayCastFunctions[ST_Count])(const LineSegment2D&,const Body&);
	void GenerateContact(const Body&,const Body&, MTV*);
	void UpdateGrid();
	std::vector<Contact*> m_pContacts;
	PhysicsGrid m_physicsGrid;
	bool m_isSpatialParitioning;
};

bool StaticCircleToStaticCircle(const Body&, const Body&, MTV*);
bool StaticAABBToStaticAABB(const Body&, const Body&, MTV*);
bool StaticCircleToStaticAABB(const Body&, const Body&, MTV*);
bool StaticAABBToStaticCircle(const Body&, const Body&, MTV*);
bool StaticPolygonToStaticPolygon(const Body&, const Body&, MTV*);
bool StaticPolygonToStaticAABB(const Body&, const Body&, MTV*);
bool StaticAABBToStaticPolygon(const Body&, const Body&, MTV*);
bool StaticPolygonToStaticCircle(const Body&, const Body&, MTV*);
bool StaticCircleToStaticPolygon(const Body&, const Body&, MTV*);

bool RayCastToAABB(const LineSegment2D&, const Body&);
bool RayCastToCircle(const LineSegment2D&, const Body&);
bool RayCastToPolygon(const LineSegment2D&, const Body&);

#endif