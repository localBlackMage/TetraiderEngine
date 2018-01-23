#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <vector>
#include "Shape.h"
#include "../Source/Math/Vector3D.h"

// Forward declaration
class GameObject;
class Body;

struct Contact {
	Body* pBody[2];
	// Vector3D normal;
	// float penetrationDepth;
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
	
	std::vector<GameObject*> m_gameObjects;
private:
	bool(*CollisionFunctions[ST_Count][ST_Count])(Body*, Body*);
	void GenerateContact(Body*, Body*);
	std::vector<Contact*> m_pContacts;
};

bool StaticCircleToStaticCircle(Body*, Body*); // Vector3D &normal, float& penetrationDepth);
bool StaticAABBToStaticAABB(Body*, Body*); // Vector3D &normal, float &penetrationDepth);
bool StaticCircleToStaticAABB(Body*, Body*); // Vector3D &normal, float &penetrationDepth);
bool StaticAABBToStaticCircle(Body*, Body*); // Vector3D &normal, float &penetrationDepth);

bool StaticPolygonToStaticPolygon(Body*, Body*);

#endif