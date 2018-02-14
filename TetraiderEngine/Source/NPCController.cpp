#include "GameObject.h"
#include "NPCController.h"
#include "Weapon.h"
#include "Health.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include "Camera.h"
#include <iostream>

NPCController::NPCController() :
	Agent(ComponentType::C_NPCCONTROLLER)
{
}

NPCController::~NPCController() {}

void NPCController::Deactivate() {
	m_pWeapon = nullptr; 
	Agent::Deactivate();
}

void NPCController::Update(float dt) {
	// THIS CODE IS GARABGE, JUST FOR RAYCAST TESTING
	GameObject* player = TETRA_GAME_OBJECTS.FindObjectWithTag(T_Player);
	Transform* playerTransfrom = player->GetComponent<Transform>(ComponentType::C_Transform);
	LineSegment2D ray(Vector2D(m_pTransform->GetPosition().x, m_pTransform->GetPosition().y), Vector2D(playerTransfrom->GetPosition().x, playerTransfrom->GetPosition().y));
	GameObjectTag tagsToIgnore[3];
	tagsToIgnore[0] = T_Enemy;
	tagsToIgnore[1] = T_Player;
	tagsToIgnore[2] = T_Projectile;
	if (!TETRA_PHYSICS.Raycast(ray, tagsToIgnore, 3)) {
		TETRA_DEBUG.DrawLine(Vector3D(ray.getP0()), Vector3D(ray.getP1()), DebugColor::WHITE);
	}
	else {
		TETRA_DEBUG.DrawLine(Vector3D(ray.getP0()), Vector3D(ray.getP1()), DebugColor::RED);
	}
	//---------------------------------

	Agent::Update(dt);
}

void NPCController::Serialize(const json& j) {
	Agent::Serialize(j["AgentData"]);
}

void NPCController::HandleEvent(Event* pEvent) {
	Agent::HandleEvent(pEvent);
}

void NPCController::LateInitialize() {
	Agent::LateInitialize();

	/*if(!m_pWeapon) {
		if (pGO)
			m_pWeapon = pGO->GetComponent<Weapon>(ComponentType::C_Weapon);
		else {
			printf("No Game Object found. NPC Controller component failed to operate.\n");
			return;
		}

		if (!m_pWeapon) {
			printf("No Weapon component found. NPC Controller component failed to operate.\n");
			return;
		}
	}*/
}