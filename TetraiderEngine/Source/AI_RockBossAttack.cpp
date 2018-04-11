#include <Stdafx.h>

AI_RockBossAttack::AI_RockBossAttack()
: AI_State(NPC_State_RockBossAttack) {
	m_phase2Attacks = 0;
}

AI_RockBossAttack::~AI_RockBossAttack(){
}

void AI_RockBossAttack::OnEnter(){	
	//printf("ATTACK ENTERED\n");
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		m_phase2Attacks++;
		if (m_phase2Attacks % 3 == 0) {
			pAgent->SetPositionBehindPlayer(175.0f);
		}
		else {
			pAgent->SetPositionBehindPlayer(-12.0f);
		}
		pAgent->SetSpeedMultiplier(5.0f);
		triedSofar = 0.0f;
		tryTime = 1.5f;
		pAgent->SetAvoidObstacles(false);
		pAgent->PlayAnimation(1);
		break;
	case PHASE2:
		m_phase2Attacks++;
		if (m_phase2Attacks % 3 == 0) {
			pAgent->SetPositionBehindPlayer(175.0f);
		}
		else {
			pAgent->SetPositionBehindPlayer(-12.0f);
		}
		pAgent->SetSpeedMultiplier(5.25f);
		triedSofar = 0.0f;
		tryTime = 1.5f;
		pAgent->SetAvoidObstacles(false);
		pAgent->PlayAnimation(1);
		break;
	case PHASE3:		
		m_phase2Attacks++;
		if (m_phase2Attacks % 3 == 0) {
			pAgent->SetPositionBehindPlayer(175.0f);
		}
		else {
			pAgent->SetPositionBehindPlayer(-12.0f);
		}
		pAgent->SetSpeedMultiplier(5.75f);
		triedSofar = 0.0f;
		tryTime = 1.5f;
		pAgent->SetAvoidObstacles(false);
		pAgent->PlayAnimation(1);
		break;
	}
}

void AI_RockBossAttack::OnUpdate(float dt){
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR	
		if (triedSofar > tryTime) {
			if (m_phase2Attacks % 3 == 0) {
				pAgent->ChangeState(NPC_ENGAGE);
			}
			else {
				pAgent->ChangeState(NPC_REACTION);
			}
		}
		else {
			triedSofar += dt;
		}
		break;
	case PHASE2:
		if (triedSofar > tryTime) {
			if (m_phase2Attacks % 3 == 0) {
				pAgent->ChangeState(NPC_ENGAGE);
			}
			else {
				pAgent->ChangeState(NPC_REACTION);
			}
		}
		else {
			triedSofar += dt;
		}
		break;
	case PHASE3:
		if (triedSofar > tryTime) {
			if (m_phase2Attacks % 3 == 0) {
				pAgent->ChangeState(NPC_ENGAGE);
			}
			else {
				pAgent->ChangeState(NPC_REACTION);
			}
		}
		else {
			triedSofar += dt;
		}
		break;
	}

}

void AI_RockBossAttack::OnExit(){
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		pAgent->SetSpeedMultiplier(1.0f);
		pAgent->SetAvoidObstacles(true);
		pAgent->ControlAnimationOnVelocity(true);
		break;
	case PHASE2:
		pAgent->SetSpeedMultiplier(1.0f);
		pAgent->SetAvoidObstacles(true);
		pAgent->ControlAnimationOnVelocity(true);
		break;
	case PHASE3:
		pAgent->SetSpeedMultiplier(1.0f);
		pAgent->SetAvoidObstacles(true);
		pAgent->ControlAnimationOnVelocity(true);
		break;
		break;
	}
}

void AI_RockBossAttack::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EventType::EVENT_ForceKnockBack:
			pAgent->SetVelocityToZero();
			m_phase2Attacks = 0;
			pAgent->ChangeState(NPC_STUNNED);
			break;
	}
}

void AI_RockBossAttack::Serialize(const json& j) {
}