#include "GameObject.h"
#include "NPCController.h"
#include "Weapon.h"
#include "Health.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include "Camera.h"
#include <iostream>

NPCController::NPCController() :
	Agent(ComponentType::C_NPCCONTROLLER),
	m_currentState(NPC_State_Idle),
	m_previousState(NPC_State_Idle)
{
}

NPCController::~NPCController() {}

void NPCController::Deactivate() {
	m_pWeapon = nullptr; 
	Agent::Deactivate();
}

void NPCController::Update(float dt) {
	// Test code for state machine -------------------------------------------
	// Change of state
	if (m_currentState != m_previousState) {
		// RunLuaScript(m_luaScripts[m_previousState] + "OnExit");
		std::cout << m_luaScripts[m_previousState] + "OnExit() called" << std::endl;
		// RunLuaScript(m_luaScripts[m_currentState] + "OnEnter");
		std::cout << m_luaScripts[m_currentState] + "OnEnter() called" << std::endl;
		std::cout << m_luaScripts[m_currentState] + "Update() being called every frame" << std::endl;
		m_previousState = m_currentState;
	}
	
	// RunLuaScript(m_luaScripts[m_currentState] + "Update");
	//std::cout << m_luaScripts[m_currentState] + "Update() called" << std::endl;

	if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_Y)) {
		m_currentState = static_cast<NPCState>((int)m_currentState + 1);
		if (m_currentState == NUM_NPC_STATES)
			m_currentState = NPC_State_Idle;
	}
	//-------------------------------------------------------------------------

	Agent::Update(dt);
}

void NPCController::LateUpdate(float dt) {
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
}

void NPCController::Serialize(const json& j) {
	Agent::Serialize(j["AgentData"]);
	int size = j["luaScripts"].size();

	for (int i = 0; i < size; ++i) {
		m_luaScripts[static_cast<NPCState>(ParseInt(j["luaScripts"][i], "state"))] = ParseString(j["luaScripts"][i], "luaScript");
	}
}

void NPCController::HandleEvent(Event* pEvent) {
	Agent::HandleEvent(pEvent);

	if(pEvent->Type() == EVENT_OnLevelInitialized)
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_EnemySpawned));
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

float NPCController::GetSquareDistanceToPlayer() {
	const GameObject* pPlayer = TETRA_GAME_OBJECTS.GetPlayer();
	const Transform* pPlayerTransform = pPlayer->GetComponent<Transform>(C_Transform);
	return Vector3D::SquareDistance(pPlayerTransform->GetPosition(), m_pTransform->GetPosition());
}