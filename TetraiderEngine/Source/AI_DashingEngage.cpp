/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_DashingEngage::AI_DashingEngage()
	: AI_State(NPC_State_DashingEngage) {
}

AI_DashingEngage::~AI_DashingEngage() {
}

void AI_DashingEngage::OnEnter() {
	sinceEngage = 0.0f;
	engageTimer = 2.0f;
	pAgent->StopMoving();
	//play pre-dashing animation!
}

void AI_DashingEngage::OnUpdate(float dt) {
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
}

void AI_DashingEngage::OnExit(){
	pAgent->ControlAnimationOnVelocity(true);
}

void AI_DashingEngage::HandleEvent(Event* pEvent) {

}

void AI_DashingEngage::Serialize(const json& j) {
}