#include <Stdafx.h>

AI_RockBossEngage::AI_RockBossEngage()
	: AI_State(NPC_State_RockBossEngage) {
}

AI_RockBossEngage::~AI_RockBossEngage() {
}

void AI_RockBossEngage::OnEnter() {
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL MELEE ENEMY BEHAVIOR
		sinceEngage = 0.0f;
		engageTimer = 2.0f;
		pAgent->StopMoving();
		break;
	case PHASE2:
		break;
	case PHASE3:
		break;
	}
}

void AI_RockBossEngage::OnUpdate(float dt) {
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		// always face player on engage
		pAgent->LookAtPlayer();
		if (pAgent->IsPlayerOutOfSight()) {
			pAgent->StopMoving();
			pAgent->ChangeState(NPC_IDLE);
			return;
		}

		if (sinceEngage > engageTimer) {
			pAgent->ChangeState(NPC_ATTACK);
			return;
		}
		else if (sinceEngage < 0.4f) {
			pAgent->MoveToPlayer();
		}
		else {
			pAgent->PlayAnimation(2); // shaking
			pAgent->StopMoving();
		}

		sinceEngage += dt;
		break;
	case PHASE2:
		break;
	case PHASE3:
		break;
	}

	
}

void AI_RockBossEngage::OnExit(){
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		pAgent->ControlAnimationOnVelocity(true);
		break;
	case PHASE2:
		break;
	case PHASE3:
		break;
	}
}

void AI_RockBossEngage::HandleEvent(Event* pEvent) {

}

void AI_RockBossEngage::Serialize(const json& j) {
}