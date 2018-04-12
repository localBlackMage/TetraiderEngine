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
		std::cout << "ROCKBOSS PHASE 1" << std::endl;
		sinceEngage = 0.0f;
		engageTimer = 2.0f;
		pAgent->StopMoving();
		break;
	case PHASE2:
		std::cout << "ROCKBOSS PHASE 2" << std::endl;
		engageTimer = 5.0f;
		pAgent->LookAtPlayer();
		bombAngleOffset = 90.0f;
		sinceEngage = 0.0f;
		pAgent->StopMoving();
		break;
	case PHASE3:
		std::cout << "ROCKBOSS PHASE 3" << std::endl;
		pAgent->SetProjectileSpeed(1300.0f);
		engageTimer = 5.0f;
		pAgent->LookAtPlayer();
		bombAngleOffset = 90.0f;
		sinceEngage = 0.0f;
		pAgent->StopMoving();
		break;
	}
}

void AI_RockBossEngage::OnUpdate(float dt) {
	switch (pAgent->GetCurrentPhase()) {
	case PHASE1: // NORMAL ROCK ENEMY BEHAVIOR
		pAgent->LookAtPlayer();

		if (sinceEngage > engageTimer) {
			pAgent->ChangeState(NPC_REACTION);
			return;
		}
		else if (sinceEngage < 0.75f) {
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
		if (sinceEngage > engageTimer) {
			pAgent->ChangeState(NPC_REACTION);
			return;
		}
		else if (sinceEngage < 1.2f) {
			pAgent->MoveToPlayer();
		}
		else if (sinceEngage < 1.75f) {
			pAgent->StopMoving();
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
	case PHASE3:
		// always face player on engage
		if (sinceEngage > engageTimer) {
			pAgent->ChangeState(NPC_REACTION);
			return;
		}
		else if (sinceEngage < 2.0f) {
			pAgent->MoveToPlayer();
		}
		else if (sinceEngage < 2.55f) {
			pAgent->StopMoving();
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
		break;
	}
}

void AI_RockBossEngage::HandleEvent(Event* pEvent) {

}

void AI_RockBossEngage::Serialize(const json& j) {
}