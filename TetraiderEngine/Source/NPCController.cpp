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
	m_isAvoidingObstacle(false),
	m_isDeathAnim(false),
	m_isAttackAnim(false),
	m_isActive(true),
	m_isBossEntering(false),
	m_isReactionAnim(false),
	m_reactionTimer(10.0f),
	m_nextReactionIn(15.0f)
{
	TETRA_EVENTS.Subscribe(EVENT_OnPlayerHealthZero, this);
	TETRA_EVENTS.Subscribe(EVENT_OnBossLand, this);
	m_tagsToIgnore[0] = T_Projectile;
	m_tagsToIgnore[1] = T_Player;
	m_tagsToIgnore[2] = T_Enemy;
	m_tagsToIgnore[3] = T_BoundaryObstacle;

	m_tagsToIgnoreForObstacleAvoidance[0] = T_Projectile;
	m_tagsToIgnoreForObstacleAvoidance[1] = T_Player;
	m_tagsToIgnoreForObstacleAvoidance[2] = T_Hazard;
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
	if (m_isDead || !m_isActive) return;

	if (m_isBossEntering) {
		SetOrientation();
		if (!pGO->GetComponent<ScriptedAnimation>(C_ScriptedAnimation)->IsPlaying())
			ExitBoss();
		
		return;
	}

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
	if (m_healthBarUI && !m_isBoss && m_healthBarUI->m_isRender) {
		m_healthBarUI->GetComponent<Transform>(C_Transform)->SetPosition(m_pTransform->GetPosition() + m_healthBarPosOffset);
	}

	if (m_reactionTimer < m_nextReactionIn) {
		m_reactionTimer += dt;
	}
}

void NPCController::LateUpdate(float dt) {
	if (m_isDead || !m_isActive) return;

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
		if (ValueExists((j)["AIStates"][i], "data")) {
			newState->Serialize((j)["AIStates"][i]["data"]);
		}
		newState->pAgent = this;
		m_AIStates[ParseInt((j)["AIStates"][i], "behaviorIndex")] = newState;
	}

	m_detectionRadius = ParseFloat(j, "detectionRadius");
	m_outOfSightRadius = ParseFloat(j, "outOfSightRadius");
	m_zoneWidth = ParseFloat(j, "zoneWidth");
	m_zoneHeight = ParseFloat(j, "zoneHeight");
	m_attackRange = ParseFloat(j, "attackRange");

	m_healthBarPrefab = ParseString(j, "healthBarPrefab");
	m_healthBarPosOffset = ParseVector3D(j, "healthBarOffset");
	m_healthScale = ParseFloat(j, "healthScale");

	m_isDeathAnim = ParseBool(j, "isDeathAnimationAvailable");
	m_deathAnim = ParseInt(j, "deathAnimationIndex");

	m_isAttackAnim = ParseBool(j, "isAttackAnimationAvailable");
	m_attackAnim = ParseInt(j, "attackAnimationIndex");

	m_isReactionAnim = ParseBool(j, "isReactionAnimation");
	m_reactionAnim = ParseInt(j, "reactionAnimationIndex");

	m_reactionPrefab = ParseString(j, "reactionPrefab");

	m_isBoss = ParseBool(j, "isBoss");
	m_puffParticlePrefab = ParseString(j, "puffParticlePrefab");
	m_puffOffset = ParseFloat(j, "puffOffset");

	if (m_isBoss) {
		TETRA_EVENTS.Subscribe(EVENT_OnEnterBoss, this);
	}

	if(ValueExists(j, "isActive"))
		m_isActive = ParseBool(j, "isActive");
}

void NPCController::EnterBoss() {
	pGO->m_isRender = true;
	m_isActive = true;
	if (m_pWeapon)
		m_pWeapon->HideWeapon(false);

	if (pGO->HasComponent(C_ScriptedAnimation)) {
		m_isBossEntering = true;
	}
	pGO->GetComponent<ScriptedAnimation>(C_ScriptedAnimation)->PlayAnimation();
	LookAtPlayer();
	SetOrientation();
	Event* pEvent = new Event(EVENT_OnBossLand, 1.5f);
	TETRA_EVENTS.AddDelayedEvent(pEvent);
}

void NPCController::ExitBoss() {
	m_isBossEntering = false;
	pGO->m_isCollisionDisabled = false;
	m_healthBarUI = TETRA_GAME_OBJECTS.CreateGameObject(m_healthBarPrefab, true, m_healthBarPosOffset);
	TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_OnExitBoss));
}

void NPCController::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_EnemySpawned));
		m_pPlayerTransform = TETRA_GAME_OBJECTS.GetPlayer()->GetComponent<Transform>(C_Transform);
		m_startingPoint = m_pTransform->GetPosition();
		m_targetDestination = m_startingPoint;
	}
	else if (pEvent->Type() == EVENT_OnTakeDamage) {
		if (m_healthBarUI) {
			m_healthBarUI->m_isRender = true;
			m_healthBarUI->HandleEvent(pEvent);
		}
	}
	else if (pEvent->Type() == EVENT_OnPlayerHealthZero) {
		m_currentState = NPC_IDLE;
		m_isPlayerDead = true;
	}
	else if (pEvent->Type() == EVENT_OnEnemyHealthZero) {
		if (m_isDeathAnim) {
			m_pAnimation->Play(m_deathAnim);
			if(m_pWeapon) m_pWeapon->HideWeapon();
			pGO->m_isCollisionDisabled = true;
			TETRA_GAME_OBJECTS.SwitchGameObjectLayer(pGO, RENDER_LAYER::L_ONE);
			//pGO->SwitchTag(T_DeadEnemy);
			m_pBody->SetVelocity(Vector3D(0, 0, 0));
			m_isDead = true;
			Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
			if (pAudio)
				pAudio->Play();
		}
		else {
			pGO->Destroy();
		}

		if (m_isBoss) {
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_OnBossDefeated));
		}
	}
	else if (pEvent->Type() == EVENT_OnEnterBoss) {
		EnterBoss();
	}
	else if (pEvent->Type() == EVENT_OnBossLand) {
		TETRA_GAME_OBJECTS.CreateGameObject(m_puffParticlePrefab, true, m_pTransform->GetPosition() + Vector3D(0, m_puffOffset,0));
	}

	if (m_isDead) return;

	Agent::HandleEvent(pEvent);

	if(m_AIStates[m_currentState])
		m_AIStates[m_currentState]->HandleEvent(pEvent);
}

void NPCController::LateInitialize() {
	Agent::LateInitialize();

	if(!m_pWeapon) {
		if (pGO)
			m_pWeapon = pGO->GetComponent<Weapon>(ComponentType::C_Weapon);
	}

	if(!m_isBoss)
		m_healthBarUI = TETRA_GAME_OBJECTS.CreateGameObject(m_healthBarPrefab);

	if (m_healthBarUI) {
		ScaleByHPStamina* pScaleByHpStamina = m_healthBarUI->GetComponent<ScaleByHPStamina>(C_ScaleByHPStamina);
		pScaleByHpStamina->SetOriginalScale(m_healthScale, true);
		m_healthBarUI->m_isRender = false;
	}

	if (!m_isActive) {
		pGO->m_isRender = false;
		pGO->m_isCollisionDisabled = true;
		if (m_pWeapon)
			m_pWeapon->HideWeapon(true);
	}

	if (!pGO->HasComponent(C_ScriptedAnimation)) {
		pGO->m_isRender = true;
		pGO->m_isCollisionDisabled = false;
		if (m_pWeapon)
			m_pWeapon->HideWeapon(false);
		m_isActive = true;
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
		return !TETRA_PHYSICS.Raycast(ray, m_tagsToIgnore, 4);
	}
	else return false;
}

bool NPCController::IsPlayerOutOfSight() {
	if (m_isPlayerDead) 
		return true;

	LineSegment2D ray(m_pTransform->GetPosition().x, m_pTransform->GetPosition().y, m_pPlayerTransform->GetPosition().x, m_pPlayerTransform->GetPosition().y);
	if (!TETRA_PHYSICS.Raycast(ray, m_tagsToIgnore, 4)) {
		return GetSquareDistanceToPlayer() > m_outOfSightRadius*m_outOfSightRadius;
	}
	else return true;
}

bool NPCController::IsPlayerTooClose(float distance) {
	if (m_isPlayerDead)
		return false;
	if (GetSquareDistanceToPlayer() < distance*distance) {
		return true;
	}
	else return false;
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

void NPCController::MoveAwayFromPlayer(float dist) {
	if (m_isPlayerDead)
		SetTargetDestination(m_pTransform->GetPosition());

	float angleOffset = RandomFloat(-45, 45);
	Vector3D dir = m_pPlayerTransform->GetPosition() - m_pTransform->GetPosition();
	dir.Normalize();
	dir = -1.0f * dir;
	SetTargetDestination(dir*dist + m_pTransform->GetPosition());
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

void NPCController::PlayReactionEffect() {
	// TODO: Add reaction effect for enemies
	if (m_isReactionAnim) {
		if(m_pAnimation->GetCurrentAnimation() == 1)
			PlayAnimation(m_reactionAnim);
	}
	else if(m_reactionTimer >= m_nextReactionIn && m_reactionPrefab != "") {
		m_reactionTimer = 0;
		GameObject* o = TETRA_GAME_OBJECTS.CreateGameObject(m_reactionPrefab, true, m_pTransform->GetPosition() + Vector3D(0, m_healthBarPosOffset.y, 0));
		o->GetComponent<ScriptedAnimation>(C_ScriptedAnimation)->SetInitialPos(m_pTransform->GetPosition() + Vector3D(0, m_healthBarPosOffset.y, 0));
		o->SetParent(pGO);
		o->DestroyIn(2.0f);
	}
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

	int tagNumber = 3;
	if (!m_isIgnoreHazards)
		tagNumber = 2;

	// Is there an obstacle right infront of me
	Vector3D pos = m_pTransform->GetPosition();
	Vector3D dirToTargetDestination = (m_targetDestination - m_pTransform->GetPosition());
	dirToTargetDestination.Normalize();
	LineSegment2D ray(pos.x, pos.y, dirToTargetDestination.x*ObstacleAvoidanceFrontCheack + pos.x, dirToTargetDestination.y*ObstacleAvoidanceFrontCheack + pos.y);
	if (!TETRA_PHYSICS.Raycast(ray, m_tagsToIgnoreForObstacleAvoidance, tagNumber, pGO)) {
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
		if (!TETRA_PHYSICS.Raycast(ray, m_tagsToIgnoreForObstacleAvoidance, tagNumber, pGO)) {
			TETRA_DEBUG.DrawLine(Vector3D(ray.getP0()), Vector3D(ray.getP1()), DebugColor::WHITE);
			angle = dirToTargetDestination.AngleDegrees() - ObstacleAvoidanceSideCheckAngle;
			if (angle > 180.0f)	angle -= 360;
			else if (angle < -180) angle += 360;

			Vector3D dir = Vector3D::VectorFromAngleDegrees(angle);
			ray.SetLine(pos.x, pos.y, dir.x*ObstacleAvoidanceSideCheck + pos.x, dir.y*ObstacleAvoidanceSideCheck + pos.y);
			if (TETRA_PHYSICS.Raycast(ray, m_tagsToIgnoreForObstacleAvoidance, tagNumber, pGO)) {
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

bool NPCController::IsAttackAnimComplete() {
	if (m_isAttackAnim) {
		if (m_pAnimation->IsPlaying())
			return false;
		else
			return true;
	}
	else
		return true;
}

void NPCController::PlayAttackAnim() {
	if (m_isAttackAnim) {
		PlayAnimation(m_attackAnim);
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

	int tagNumber = 3;
	if (!m_isIgnoreHazards)
		tagNumber = 2;

	Vector3D dir = Vector3D::VectorFromAngleDegrees(angle);
	LineSegment2D ray(pos.x, pos.y, dir.x*ObstacleAvoidanceSideCheck + pos.x, dir.y*ObstacleAvoidanceSideCheck + pos.y);
	LineSegment2D rayForward(pos.x, pos.y, dirToTargetDestination.x*ObstacleAvoidanceFrontCheack + pos.x, dirToTargetDestination.y*ObstacleAvoidanceFrontCheack + pos.y);
	if (!TETRA_PHYSICS.Raycast(ray, m_tagsToIgnoreForObstacleAvoidance, tagNumber, pGO) && !TETRA_PHYSICS.Raycast(rayForward, m_tagsToIgnoreForObstacleAvoidance, tagNumber, pGO)) {
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

BossPhase NPCController::GetCurrentPhase() {
	Health *pHealth = pGO->GetComponent<Health>(ComponentType::C_Health);
	float maxHP = (float)pHealth->GetMaxHealth();
	float curHP = (float)pHealth->GetHealth();
	float index = maxHP / (float)TOTAL_PHASE_NUM;

	return static_cast<BossPhase>( (int)std::ceil(curHP / index) - 1);
}


void NPCController::SteerTowardPlayer(float distance) {
	Vector3D dir = m_pPlayerTransform->GetPosition() - m_pTransform->GetPosition();
	dir.Normalize();
	m_lookDirection = dir;
	m_targetDestination += distance * dir;
}


void NPCController::MoveInLookDirection(float distance) {
	m_targetDestination += distance * m_lookDirection;
}