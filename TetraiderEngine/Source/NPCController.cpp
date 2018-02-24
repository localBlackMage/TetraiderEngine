#include "GameObject.h"
#include "NPCController.h"
#include "Weapon.h"
#include "Health.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include "Camera.h"
#include "Math\MathFunctions.h"
#include <iostream>

NPCController::NPCController() :
	Agent(ComponentType::C_NPCCONTROLLER),
	m_currentState(NPC_IDLE),
	m_previousState(NPC_IDLE),
	m_detectionRadius(0.0f),
	m_outOfSightRadius(0.0f),
	m_zoneWidth(0.0f),
	m_zoneHeight(0.0f),
	m_arrivedAtDestination(true)
{
}

NPCController::~NPCController() {
	for (auto comp : m_AIStates)
		delete comp;
}

void NPCController::Deactivate() {
	m_pWeapon = nullptr; 
	Agent::Deactivate();
}

void NPCController::Update(float dt) {
	// Change of state
	if (m_currentState != m_previousState) {
		m_AIStates[m_previousState]->OnExit();
		m_AIStates[m_currentState]->OnEnter();
		m_previousState = m_currentState;
		//std::cout << "statechanged\n";
	}
	
	// Update with currentState
	m_AIStates[m_currentState]->OnUpdate(dt);

	// Move to destination
	if (!m_arrivedAtDestination && !IsArrivedAtDestination()) {
		Vector3D dirToTarget = m_targetDestination - m_pTransform->GetPosition();
		dirToTarget.Normalize();
		m_targetVelocity = dirToTarget*m_speed;
	}
	else {
		m_arrivedAtDestination = true;
		m_targetVelocity = Vector3D(0,0,0);
	}

	
	Agent::Update(dt);
}

void NPCController::LateUpdate(float dt) {
	if (TETRA_DEBUG.IsDebugModeOn()) {
		TETRA_DEBUG.DrawWireCircle(m_pTransform->GetPosition(), m_detectionRadius*2.0f, DebugColor::RED);
		TETRA_DEBUG.DrawWireCircle(m_pTransform->GetPosition(), m_outOfSightRadius*2.0f, DebugColor::YELLOW);
		TETRA_DEBUG.DrawWireRectangle(m_startingPoint, Vector3D(), Vector3D(m_zoneWidth, m_zoneHeight, 0), DebugColor::WHITE);
		if (!m_arrivedAtDestination) {
			TETRA_DEBUG.DrawLine(m_pTransform->GetPosition(), m_targetDestination, DebugColor::CYAN);
		}
	}

	// THIS CODE IS GARABGE, JUST FOR RAYCAST TESTING
	/*GameObject* player = TETRA_GAME_OBJECTS.FindObjectWithTag(T_Player);
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
	}*/
	//---------------------------------
}

void NPCController::Serialize(const json& j) {
	Agent::Serialize(j["AgentData"]);
	int definedSize = j["AIStates"].size();

	for (int i = 0; i < definedSize; ++i) {
		AI_State* newState = AIStateFactory.CreateState(ParseString((j)["AIStates"][i], "AIStateType"));
		newState->pAgent = this;
		m_AIStates[ParseInt((j)["AIStates"][i], "behaviorIndex")] = newState;
	}

	m_detectionRadius = ParseFloat(j, "detectionRadius");
	m_outOfSightRadius = ParseFloat(j, "outOfSightRadius");
	m_zoneWidth = ParseFloat(j, "zoneWidth");
	m_zoneHeight = ParseFloat(j, "zoneHeight");
	m_attackRange = ParseFloat(j, "attackRange");
}

void NPCController::HandleEvent(Event* pEvent) {
	Agent::HandleEvent(pEvent);

	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_EnemySpawned));
		m_pPlayerTransform = TETRA_GAME_OBJECTS.GetPlayer()->GetComponent<Transform>(C_Transform);
		m_startingPoint = m_pTransform->GetPosition();
		m_targetDestination = m_startingPoint;
	}
}

void NPCController::LateInitialize() {
	Agent::LateInitialize();

	if(!m_pWeapon) {
		if (pGO)
			m_pWeapon = pGO->GetComponent<Weapon>(ComponentType::C_Weapon);
		else {
		//	printf("No Game Object found. NPC Controller component failed to operate.\n");
			return;
		}

		if (!m_pWeapon) {
		//	printf("No Weapon component found. NPC Controller component failed to operate.\n");
			return;
		}
	}
}

float NPCController::GetSquareDistanceToPlayer() {
	return Vector3D::SquareDistance(m_pPlayerTransform->GetPosition(), m_pTransform->GetPosition());
}

bool NPCController::IsArrivedAtDestination() {
	return Vector3D::SquareDistance(m_pTransform->GetPosition(), m_targetDestination) < 150.0f;
}

bool NPCController::IsPlayerInSight() {
	return GetSquareDistanceToPlayer() < m_detectionRadius*m_detectionRadius;
}

bool NPCController::IsPlayerOutOfSight() {
	return GetSquareDistanceToPlayer() > m_outOfSightRadius*m_outOfSightRadius;
}

void NPCController::StopMoving() {
	m_targetDestination = m_pTransform->GetPosition();
}

void NPCController::SetTargetDestination(const Vector3D& pos) {
	m_arrivedAtDestination = false;
	m_targetDestination = pos;
}

bool NPCController::RollDie(float probability) {
	return RandomFloat(0,1) < probability;
}

void NPCController::SetDestinationToRandomPointInZone() {
	float x = RandomFloat(m_startingPoint.x - m_zoneWidth/2.0f, m_startingPoint.x + m_zoneWidth / 2.0f);
	float y = RandomFloat(m_startingPoint.y - m_zoneHeight / 2.0f, m_startingPoint.y + m_zoneHeight / 2.0f);
	SetTargetDestination(Vector3D(x, y, 0));
}

void NPCController::ChangeState(NPC_CONTROLLER_AI newState) {
	m_previousState = m_currentState;
	m_currentState = newState;
}

void NPCController::LookInDirectionOfMovement() {
	m_lookDirection = GetCurrentVelocity();
	m_lookDirection.Normalize();
}

void NPCController::LookAtPlayer() {
	m_lookDirection = m_pPlayerTransform->GetPosition() - m_pTransform->GetPosition();
	m_lookDirection.Normalize();
}

void NPCController::LookAtPlayer(float offsetAngleDegrees) {
	m_lookDirection = m_pPlayerTransform->GetPosition() - m_pTransform->GetPosition();
	m_lookDirection.Normalize();
	float angle = m_lookDirection.AngleDegrees() + offsetAngleDegrees;
	m_lookDirection = Vector3D::VectorFromAngleDegrees(angle);
}

void NPCController::MoveToPlayer() {
	SetTargetDestination(m_pPlayerTransform->GetPosition());
}

void NPCController::MoveAroundPlayer() {
	float angleOffset;

	if (RollDie(0.85f)) angleOffset = RandomFloat(-90, 90);
	else {
		angleOffset = RandomFloat(90, 180);
		if (RollDie(0.5)) angleOffset *= -1;
	}
	
	float angle = m_lookDirection.AngleDegrees() + angleOffset;
	if (angle > 180.0f)	angle -= 360;
	else if (angle < -180) angle += 360;

	Vector3D dir = Vector3D::VectorFromAngleDegrees(angle);
	float magnitude = RandomFloat(m_detectionRadius / 2.0f, m_detectionRadius);
	SetTargetDestination(dir*magnitude + m_pTransform->GetPosition());
}

bool NPCController::UseAttack(int attack) {
	return m_pWeapon->UseAttack(attack, m_lookDirection);
}

bool NPCController::IsInAttackRange() {
	return GetSquareDistanceToPlayer() < m_attackRange*m_attackRange;
}