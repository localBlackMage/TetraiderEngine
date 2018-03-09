//#include "GameObject.h"
//#include "NPCController.h"
//#include "Weapon.h"
//#include "Health.h"
//#include "Transform.h"
//#include "Animation.h"
//#include "TetraiderAPI.h"
//#include "Camera.h"
//#include "Math\MathFunctions.h"
//#include <iostream>

#include <Stdafx.h>

NPCController::NPCController() :
	Agent(ComponentType::C_NPCCONTROLLER),
	m_currentState(NPC_IDLE),
	m_previousState(NPC_IDLE),
	m_detectionRadius(0.0f),
	m_outOfSightRadius(0.0f),
	m_zoneWidth(0.0f),
	m_zoneHeight(0.0f),
	m_arrivedAtDestination(true),
	m_speedMultiplier(1.0f),
	m_isAvoidObstacles(true),
	m_isAvoidingObstacle(false)
{
	TETRA_EVENTS.Subscribe(EVENT_OnPlayerHealthZero, this);
	m_tagsToIgnore[0] = T_Projectile;
	m_tagsToIgnore[1] = T_Player;
	m_tagsToIgnore[2] = T_Enemy;
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
		if (m_AIStates[m_currentState]) {
			m_AIStates[m_previousState]->OnExit();
			m_AIStates[m_currentState]->OnEnter();
			m_previousState = m_currentState;
		}
		else {
			m_currentState = m_previousState;
		}
	}
	
	// Update with currentState
	if(m_AIStates[m_currentState])
		m_AIStates[m_currentState]->OnUpdate(dt);

	// Move to destination
	if (!m_arrivedAtDestination && !IsArrivedAtDestination()) {
		// Avoid Obstacles
		AvoidObstacles(dt);
		m_prevPos = m_pTransform->GetPosition();
		Vector3D dirToTarget;
		if (m_isAvoidingObstacle) {
			dirToTarget = m_secondaryDestination - m_pTransform->GetPosition();
		}
		else {
			dirToTarget = m_targetDestination - m_pTransform->GetPosition();
		}

		dirToTarget.Normalize();
		m_targetVelocity = dirToTarget*m_speed*m_speedMultiplier;
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
			if(m_isAvoidingObstacle)
				TETRA_DEBUG.DrawLine(m_pTransform->GetPosition(), m_secondaryDestination, DebugColor::YELLOW);

		}
	}

	CheckForObstacleAvoidance();
}

void NPCController::Serialize(const json& j) {
	Agent::Serialize(j["AgentData"]);
	int definedSize = j["AIStates"].size();

	for (int i = 0; i < definedSize; ++i) {
		if (ParseString((j)["AIStates"][i], "AIStateType") == "AI_DashingEngage")
			m_isControlAnimationOnVelocity = false;

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
	else if (pEvent->Type() == EVENT_OnPlayerHealthZero) {
		m_currentState = NPC_IDLE;
		m_isPlayerDead = true;
	}

	if(m_AIStates[m_currentState])
		m_AIStates[m_currentState]->HandleEvent(pEvent);
}

void NPCController::LateInitialize() {
	Agent::LateInitialize();

	if(!m_pWeapon) {
		if (pGO)
			m_pWeapon = pGO->GetComponent<Weapon>(ComponentType::C_Weapon);
	}
}

float NPCController::GetSquareDistanceToPlayer() {
	return Vector3D::SquareDistance(m_pPlayerTransform->GetPosition(), m_pTransform->GetPosition());
}

bool NPCController::IsArrivedAtDestination() {
	return Vector3D::SquareDistance(m_pTransform->GetPosition(), m_targetDestination) < 200.0f;
}

bool NPCController::IsPlayerInSight() {
	if (m_isPlayerDead)
		return false;

	if (GetSquareDistanceToPlayer() < m_detectionRadius*m_detectionRadius) {
		LineSegment2D ray(m_pTransform->GetPosition().x, m_pTransform->GetPosition().y, m_pPlayerTransform->GetPosition().x, m_pPlayerTransform->GetPosition().y);
		return !TETRA_PHYSICS.Raycast(ray, m_tagsToIgnore, 3);
	}
	else return false;
}

bool NPCController::IsPlayerOutOfSight() {
	if (m_isPlayerDead) 
		return true;

	LineSegment2D ray(m_pTransform->GetPosition().x, m_pTransform->GetPosition().y, m_pPlayerTransform->GetPosition().x, m_pPlayerTransform->GetPosition().y);
	if (!TETRA_PHYSICS.Raycast(ray, m_tagsToIgnore, 3)) {
		return GetSquareDistanceToPlayer() > m_outOfSightRadius*m_outOfSightRadius;
	}
	else return true;
}

void NPCController::StopMoving() {
	m_targetDestination = m_pTransform->GetPosition();
	m_isAvoidingObstacle = false;
	m_arrivedAtDestination = true;
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

void NPCController::SetVelocityToZero() {
	m_currentVelocity = Vector3D(0, 0, 0);
	m_isAvoidingObstacle = false;
	m_arrivedAtDestination = true;
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

bool NPCController::IsPlayerOutOfScreen() {
	return GetSquareDistanceToPlayer() > TETRA_RENDERER.WindowWidth()*TETRA_RENDERER.WindowWidth();
}

void NPCController::LookAtPlayer(float offsetAngleDegrees) {
	m_lookDirection = m_pPlayerTransform->GetPosition() - m_pTransform->GetPosition();
	m_lookDirection.Normalize();
	float angle = m_lookDirection.AngleDegrees() + offsetAngleDegrees;
	if (angle > 180.0f)	angle -= 360;
	else if (angle < -180) angle += 360;

	m_lookDirection = Vector3D::VectorFromAngleDegrees(angle);
}

void NPCController::MoveToPlayer() {
	SetTargetDestination(m_pPlayerTransform->GetPosition());
}

void NPCController::GoToPositionAroundPlayer() {
	if(m_isPlayerDead)
		SetTargetDestination(m_pTransform->GetPosition());

	float angleOffset;

	if (GetSquareDistanceToPlayer() > 250.0f) {
		angleOffset = RandomFloat(-90, 90);
	}
	else {
		angleOffset = RandomFloat(90, 180);
		if (RollDie(0.5)) angleOffset *= -1;
	}
	
	float angle = m_lookDirection.AngleDegrees() + angleOffset;
	if (angle > 180.0f)	angle -= 360;
	else if (angle < -180) angle += 360;

	Vector3D dir = Vector3D::VectorFromAngleDegrees(angle);
	float magnitude = RandomFloat(100, sqrtf(GetSquareDistanceToPlayer()));
	SetTargetDestination(dir*magnitude + m_pTransform->GetPosition());
}

bool NPCController::UseAttack(int attack) {
	return m_pWeapon->UseAttack(attack, m_lookDirection);
}

bool NPCController::IsInAttackRange() {
	if (m_isPlayerDead)
		return false;

	return GetSquareDistanceToPlayer() < m_attackRange*m_attackRange;
}

void NPCController::SetPositionBehindPlayer(float distance) {
	if (m_isPlayerDead)
		SetTargetDestination(m_pTransform->GetPosition());

	Vector3D dirToPlayer = m_pPlayerTransform->GetPosition() - m_pTransform->GetPosition();
	dirToPlayer.Normalize();
	SetTargetDestination(dirToPlayer*distance + m_pPlayerTransform->GetPosition());
}

void NPCController::PlayAnimation(int animation) {
	m_isControlAnimationOnVelocity = false;
	m_pAnimation->Play(animation);
}

bool NPCController::IsTooFarFromStartingPoint() {
	return Vector3D::SquareDistance(m_pTransform->GetPosition(), m_startingPoint) > 1000000;
}

void NPCController::GoToStartingPoint() {
	SetTargetDestination(m_startingPoint);
}

void NPCController::CheckForObstacleAvoidance() {
	// Player can see enemy, check for obstacle avoidance
	if (IsPlayerOutOfScreen() || !m_isAvoidObstacles) {
		m_isAvoidingObstacle = false;
		return;
	}

	if (m_isAvoidingObstacle) return;

	// Is there an obstacle right infront of me
	Vector3D pos = m_pTransform->GetPosition();
	Vector3D dirToTargetDestination = (m_targetDestination - m_pTransform->GetPosition());
	dirToTargetDestination.Normalize();
	LineSegment2D ray(pos.x, pos.y, dirToTargetDestination.x*ObstacleAvoidanceFrontCheack + pos.x, dirToTargetDestination.y*ObstacleAvoidanceFrontCheack + pos.y);
	if (!TETRA_PHYSICS.Raycast(ray, m_tagsToIgnore, 2, pGO)) {
		// No need to avoid obstacles
		TETRA_DEBUG.DrawLine(Vector3D(ray.getP0()), Vector3D(ray.getP1()), DebugColor::WHITE);
		m_isAvoidingObstacle = false;
		return;
	}
	else {
		TETRA_DEBUG.DrawLine(Vector3D(ray.getP0()), Vector3D(ray.getP1()), DebugColor::RED);
		float angle = dirToTargetDestination.AngleDegrees() + ObstacleAvoidanceSideCheckAngle;
		if (angle > 180.0f)	angle -= 360;
		else if (angle < -180) angle += 360;

		Vector3D dir = Vector3D::VectorFromAngleDegrees(angle);
		ray.SetLine(pos.x, pos.y, dir.x*ObstacleAvoidanceSideCheck + pos.x, dir.y*ObstacleAvoidanceSideCheck + pos.y);
		if (!TETRA_PHYSICS.Raycast(ray, m_tagsToIgnore, 2, pGO)) {
			TETRA_DEBUG.DrawLine(Vector3D(ray.getP0()), Vector3D(ray.getP1()), DebugColor::WHITE);
			angle = dirToTargetDestination.AngleDegrees() - ObstacleAvoidanceSideCheckAngle;
			if (angle > 180.0f)	angle -= 360;
			else if (angle < -180) angle += 360;

			Vector3D dir = Vector3D::VectorFromAngleDegrees(angle);
			ray.SetLine(pos.x, pos.y, dir.x*ObstacleAvoidanceSideCheck + pos.x, dir.y*ObstacleAvoidanceSideCheck + pos.y);
			if (TETRA_PHYSICS.Raycast(ray, m_tagsToIgnore, 2, pGO)) {
				TETRA_DEBUG.DrawLine(Vector3D(ray.getP0()), Vector3D(ray.getP1()), DebugColor::RED);
				m_avoidDirection = ObstacleAvoidanceDirection::LEFT;
			}
		}
		else {
			TETRA_DEBUG.DrawLine(Vector3D(ray.getP0()), Vector3D(ray.getP1()), DebugColor::RED);
			m_avoidDirection = ObstacleAvoidanceDirection::RIGHT;
		}

		m_isAvoidingObstacle = true;
		m_stuckTimer = 0.0f;
	}
}

void NPCController::AvoidObstacles(float dt) {
	if (!m_isAvoidingObstacle || !m_isAvoidObstacles) return;

	Vector3D pos = m_pTransform->GetPosition();
	Vector3D dirToTargetDestination = (m_targetDestination - m_pTransform->GetPosition());
	dirToTargetDestination.Normalize();
	float angle = 0;
	
	int sign = 1;

	if (m_avoidDirection == ObstacleAvoidanceDirection::LEFT)
		sign = -1;

	angle = dirToTargetDestination.AngleDegrees() + ObstacleAvoidanceSideCheckAngle*sign;

	if (angle >= 180.0f) angle -= 360;
	else if (angle < -180.0f) angle += 360;

	Vector3D dir = Vector3D::VectorFromAngleDegrees(angle);
	LineSegment2D ray(pos.x, pos.y, dir.x*ObstacleAvoidanceSideCheck + pos.x, dir.y*ObstacleAvoidanceSideCheck + pos.y);
	LineSegment2D rayForward(pos.x, pos.y, dirToTargetDestination.x*ObstacleAvoidanceFrontCheack + pos.x, dirToTargetDestination.y*ObstacleAvoidanceFrontCheack + pos.y);
	if (!TETRA_PHYSICS.Raycast(ray, m_tagsToIgnore, 2, pGO) && !TETRA_PHYSICS.Raycast(rayForward, m_tagsToIgnore, 2, pGO)) {
		m_isAvoidingObstacle = false;
		m_stuckTimer = 0.0f;
		return;
	}
	else {
		TETRA_DEBUG.DrawLine(Vector3D(ray.getP0()), Vector3D(ray.getP1()), DebugColor::RED);
		TETRA_DEBUG.DrawLine(Vector3D(rayForward.getP0()), Vector3D(rayForward.getP1()), DebugColor::RED);
		angle = dirToTargetDestination.AngleDegrees() - 70*sign;

		if (angle >= 180.0f) angle -= 360;
		else if (angle < -180.0f) angle += 360;

		dir = Vector3D::VectorFromAngleDegrees(angle);
		SetSecondaryTargetDestination(Vector3D(pos.x + dir.x*ObstacleAvoidanceSideCheck*4.0f, pos.y + dir.y*ObstacleAvoidanceSideCheck*4.0f, 0));
	}

	// Check if AI is stuck
	/*if (Vector3D::SquareDistance(m_pTransform->GetPosition(), m_prevPos) < 1.0f)
		m_stuckTimer += dt;
	else
		m_stuckTimer = 0;

	if (m_stuckTimer > 0.2f) {
		if (m_avoidDirection == ObstacleAvoidanceDirection::LEFT)
			m_avoidDirection = ObstacleAvoidanceDirection::RIGHT;
		else
			m_avoidDirection = ObstacleAvoidanceDirection::LEFT;

		m_stuckTimer = 0.0f;
	}*/
}