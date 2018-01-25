#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <vector>
#include "Shape.h"
#include "../Source/Math/Collisions.h"
#include "../Source/Math/Vector3D.h"

// Forward declaration
class GameObject;
enum class GameObjectTag;
class Body;

struct Contact {
	Body* m_pBody[2];
	MTV m_MTV;
};

class PhysicsManager {
private:
	PhysicsManager();
	~PhysicsManager();

public:
	PhysicsManager(const PhysicsManager &) = delete;
	void operator=(const PhysicsManager &) = delete;

	static PhysicsManager& GetInstance() {
		static PhysicsManager instance;
		return instance;
	}
	void Integrate(float dt);
	void ResolveCollisions();
	void AddGameObject(GameObject* pGO);
	void RemoveGameObject(GameObject* pGO);
	void CheckCollisionsAndGenerateContacts();
	void FireEventsToContacts();
	void ClearContacts();
	bool Raycast(const LineSegment2D& ray, const GameObjectTag* pIgnoreLayer, int layerSize);
	
	std::vector<GameObject*> m_gameObjects;
private:
	bool(*CollisionFunctions[ST_Count][ST_Count])(Body*, Body*, MTV*);
	bool(*RayCastFunctions[ST_Count])(const LineSegment2D&, Body*);
	void GenerateContact(Body*, Body*, MTV*);
	std::vector<Contact*> m_pContacts;
};

bool StaticCircleToStaticCircle(Body*, Body*, MTV*); 
bool StaticAABBToStaticAABB(Body*, Body*, MTV*); 
bool StaticCircleToStaticAABB(Body*, Body*, MTV*); 
bool StaticAABBToStaticCircle(Body*, Body*, MTV*);
bool StaticPolygonToStaticPolygon(Body*, Body*, MTV*);
bool StaticPolygonToStaticAABB(Body*, Body*, MTV*);
bool StaticAABBToStaticPolygon(Body*, Body*, MTV*);
bool StaticPolygonToStaticCircle(Body*, Body*, MTV*);
bool StaticCircleToStaticPolygon(Body*, Body*, MTV*);

bool RayCastToAABB(const LineSegment2D&, Body*);
bool RayCastToCircle(const LineSegment2D&, Body*);
bool RayCastToPolygon(const LineSegment2D&, Body*);

#endif