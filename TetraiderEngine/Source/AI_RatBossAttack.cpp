#include <Stdafx.h>

AI_RatBossAttack::AI_RatBossAttack()
: AI_State(NPC_State_RatBossAttack) {
	m_attackMinLimit = 2;
	m_attackMaxLimit = 3;

	m_steerFor = 0.2f;
	idleDuration = 0.55;
}

AI_RatBossAttack::~AI_RatBossAttack(){
}

void AI_RatBossAttack::OnEnter(){
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL MELEE ENEMY BEHAVIOR
		m_attackCounter = RandomInt(m_attackMinLimit, m_attackMaxLimit + 1);
		pAgent->StopMoving();
		break;
	case PHASE2:
		//pAgent->LookAtPlayer();
		pAgent->SetSpeedMultiplier(2.75f);
		m_steerTimer = 0.0f;
		break;
	case PHASE3:
		pAgent->SetSpeedMultiplier(3.25f);
		m_steerTimer = 0.0f;
		break;
	}
	
}

void AI_RatBossAttack::OnUpdate(float dt){
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL MELEE ENEMY BEHAVIOR
		pAgent->LookAtPlayer();
		if (m_attackCounter > m_attackMaxLimit && pAgent->IsAttackAnimComplete()) {
			pAgent->ChangeState(NPC_ENGAGE);
		}

		if (idleTime < idleDuration) {
			idleTime += dt;
			return;
		}

		if (pAgent->UseAttack(0)) {
			pAgent->PlayAttackAnim();
			m_attackCounter++;
		}
		if (pAgent->IsAttackAnimComplete())
			pAgent->ControlAnimationOnVelocity(true);
		break;
	case PHASE2:
		pAgent->LookInDirectionOfMovement();
		if (m_steerTimer < m_steerFor) {
			pAgent->SteerTowardPlayer(4.5f);
		}
		if (pAgent->IsArrivedAtDestination() && pAgent->IsAttackAnimComplete() || (m_steerTimer > m_steerFor + 1.2f)) {
			pAgent->StopMoving();
		}

		if (pAgent->IsArrivedAtDestination() && pAgent->IsAttackAnimComplete() || (m_steerTimer > m_steerFor + 1.6f)) {
			pAgent->ChangeState(NPC_RETREAT);
		}
		if (pAgent->UseAttack(0)) {
			pAgent->PlayAttackAnim();
		}
		if (pAgent->IsAttackAnimComplete())
			pAgent->ControlAnimationOnVelocity(true);

		m_steerTimer += dt;
		break;
	case PHASE3:
		pAgent->LookInDirectionOfMovement();
		if (m_steerTimer < m_steerFor) {
			pAgent->SteerTowardPlayer(7.5f);
		}
		if (pAgent->IsArrivedAtDestination() && pAgent->IsAttackAnimComplete() || (m_steerTimer > m_steerFor + 1.2f)) {
			pAgent->StopMoving();
		}
		if (pAgent->IsArrivedAtDestination() && pAgent->IsAttackAnimComplete() || (m_steerTimer > m_steerFor + 1.6f)) {
			pAgent->ChangeState(NPC_RETREAT);
		}
		if (pAgent->UseAttack(0)) {
			pAgent->PlayAttackAnim();
		}
		if (pAgent->IsAttackAnimComplete())
			pAgent->ControlAnimationOnVelocity(true);

		m_steerTimer += dt;
		break;
	}
}

void AI_RatBossAttack::OnExit(){
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL MELEE ENEMY BEHAVIOR
		pAgent->SetSpeedMultiplier(1.0f);
		pAgent->ControlAnimationOnVelocity(true);
	case PHASE2:
		pAgent->MoveInLookDirection(15.0f);
		break;
	case PHASE3:
		pAgent->MoveInLookDirection(15.0f);
		break;
	}
}

void AI_RatBossAttack::HandleEvent(Event* pEvent) {

}

void AI_RatBossAttack::Serialize(const json& j) {
	m_attackMinLimit = ParseInt(j, "attackMinLimit");
	m_attackMaxLimit = ParseInt(j, "attackMaxLimit");
}