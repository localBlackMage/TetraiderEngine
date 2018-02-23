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
	m_currentState(NPC_IDLE),
	m_previousState(NPC_IDLE)
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
		m_AIStates[m_previousState]->OnExit();
		m_AIStates[m_currentState]->OnEnter();
		m_AIStates[m_currentState]->OnUpdate();
		m_previousState = m_currentState;
	}
	
	// Update with currentState
	m_AIStates[m_currentState]->OnUpdate();
	
	if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_Y)) {
		m_currentState = static_cast<NPC_CONTROLLER_AI>((int)m_currentState + 1);
		if (m_currentState == NUM_AI_STATES)
			m_currentState = NPC_IDLE;
		std::cout << "State changed from " << m_previousState << " to " << m_currentState << std::endl;
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
	int definedSize = j["AIStates"].size();
	if (definedSize != NPC_NUM_BEHAVIOR) {
		std::cout << "INVALID DEFINITION OF AI BEHAVIOR: PROVIDE CORRECT AIStates in json!\n";
	}

	for (unsigned int i = 0; i < NPC_NUM_BEHAVIOR; ++i) {
		//AI_State* newState = AIStateFactory.CreateState(ParseString((j)["AIStates"][i], "AIStateType"));
		m_AIStates[i] = AIStateFactory.CreateState(ParseString((j)["AIStates"][i], "AIStateType"));
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