#include <Stdafx.h>

AI_RockBossEngage::AI_RockBossEngage()
	: AI_State(NPC_State_RockBossEngage) {
	engageTimer = 2.0f;
}

AI_RockBossEngage::~AI_RockBossEngage() {
}

void AI_RockBossEngage::OnEnter() {
	//printf("ENGAGE ENTERED\n");
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL MELEE ENEMY BEHAVIOR
		sinceEngage = 0.0f;
		engageTimer = 2.0f;
		pAgent->StopMoving();
		break;
	case PHASE2:
		std::cout << "ROCKBOSS PHASE 2" << std::endl;
		sinceEngage = 0.0f;
		engageTimer = 2.0f;
		pAgent->StopMoving();
		break;
	case PHASE3:
		std::cout << "ROCKBOSS PHASE 3" << std::endl;
		engageTimer = 4.0f;
		pAgent->LookAtPlayer();
		bombAngleOffset = 0.0f;
		sinceEngage = 0.0f;
		pAgent->StopMoving();
		break;
	}
}

void AI_RockBossEngage::OnUpdate(float dt) {
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		// always face player on engage
		pAgent->LookAtPlayer();

		if (sinceEngage > engageTimer) {
			pAgent->ChangeState(NPC_REACTION);
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
		// always face player on engage
		pAgent->LookAtPlayer();

		if (sinceEngage > engageTimer) {
			pAgent->ChangeState(NPC_REACTION);
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
	case PHASE3:
		// always face player on engage
		if (sinceEngage > engageTimer) {
			pAgent->ChangeState(NPC_REACTION);
			return;
		}
		else if (sinceEngage < 0.4f) {
			pAgent->MoveToPlayer();
		}
		else {
			pAgent->PlayAnimation(2); // shaking
			pAgent->LookAtPlayer(bombAngleOffset);
			if (pAgent->UseAttack(0)) {
				bombAngleOffset += 36;
			}
			pAgent->StopMoving();
		}

		sinceEngage += dt;
		break;
	}

	
}

void AI_RockBossEngage::OnExit(){
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		pAgent->ControlAnimationOnVelocity(true);
		break;
	case PHASE2:
		pAgent->ControlAnimationOnVelocity(true);
		break;
	case PHASE3:
		pAgent->ControlAnimationOnVelocity(true);
		engageTimer = 2.0f;
		break;
	}
}

void AI_RockBossEngage::HandleEvent(Event* pEvent) {

}

void AI_RockBossEngage::Serialize(const json& j) {
}