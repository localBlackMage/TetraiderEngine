#include <Stdafx.h>

AI_RockBossReaction::AI_RockBossReaction()
: AI_State(NPC_State_RockBossReaction) {
	idleDuration = 0.12f;
}

AI_RockBossReaction::~AI_RockBossReaction(){
}

void AI_RockBossReaction::OnEnter(){
	//printf("REACTION ENTERED\n");
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		break;
	case PHASE2:
		idledSoFar = 0.0f;
		break;
	case PHASE3:
		break;
	}

}

void AI_RockBossReaction::OnUpdate(float dt){

	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		pAgent->LookAtPlayer();
		if (idledSoFar > idleDuration) {
			pAgent->ChangeState(NPC_ATTACK);
			return;
		}
		else {
			pAgent->PlayAnimation(2); // shaking
			pAgent->StopMoving();
		}
		idledSoFar += dt;
		break;
	case PHASE2:
		pAgent->LookAtPlayer();
		if (idledSoFar > idleDuration) {
			pAgent->ChangeState(NPC_ATTACK);
			return;
		}
		else {
			pAgent->PlayAnimation(2); // shaking
			pAgent->StopMoving();
		}
		idledSoFar += dt;
		break;
	case PHASE3:		
		pAgent->LookAtPlayer();
		if (idledSoFar > idleDuration) {
			pAgent->ChangeState(NPC_ATTACK);
			return;
		}
		else {
			pAgent->PlayAnimation(2); // shaking
			pAgent->StopMoving();
		}
		idledSoFar += dt;
		break;
	}
}

void AI_RockBossReaction::OnExit(){
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		pAgent->ControlAnimationOnVelocity(true);
		break;
	case PHASE2:
		pAgent->ControlAnimationOnVelocity(true);
		break;
	case PHASE3:
		pAgent->ControlAnimationOnVelocity(true);
		break;
	}
}


void AI_RockBossReaction::HandleEvent(Event* pEvent) {

}

void AI_RockBossReaction::Serialize(const json& j) {
}