
#include <Stdafx.h>

AI_RockBossAttack::AI_RockBossAttack()
: AI_State(NPC_State_RockBossAttack) {

}

AI_RockBossAttack::~AI_RockBossAttack(){
}

void AI_RockBossAttack::OnEnter(){	
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		pAgent->SetPositionBehindPlayer(175.0f);
		pAgent->SetSpeedMultiplier(4.5f);
		triedSofar = 0.0f;
		tryTime = 1.5f;
		pAgent->SetAvoidObstacles(false);
		pAgent->PlayAnimation(1);
		break;
	case PHASE2:
		break;
	case PHASE3:
		break;
	}
}

void AI_RockBossAttack::OnUpdate(float dt){
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR	
		if (triedSofar > tryTime) {
			pAgent->ChangeState(NPC_ENGAGE);
		}
		else {
			triedSofar += dt;
		}
		break;
	case PHASE2:
		break;
	case PHASE3:
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
		break;
	case PHASE3:
		break;
	}
}

void AI_RockBossAttack::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EventType::EVENT_ForceKnockBack:
			pAgent->SetVelocityToZero();
			pAgent->ChangeState(NPC_STUNNED);
			break;
		case EventType::EVENT_OnCollide:
			OnCollideData * pData = pEvent->Data<OnCollideData>();
			if (pData->pGO->m_tag==T_Obstacle) {
				pAgent->ChangeState(NPC_ENGAGE);
			}
			break;
	}
}

void AI_RockBossAttack::Serialize(const json& j) {
}