/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_WalkTowardPlayerEngage::AI_WalkTowardPlayerEngage()
	: AI_State(NPC_State_WalkTowardPlayerEngage) {

}

AI_WalkTowardPlayerEngage::~AI_WalkTowardPlayerEngage() {
}

void AI_WalkTowardPlayerEngage::OnEnter() {
	idleDuration = 3.0f;
	idledSoFar = 0.0f;
	pAgent->SetSpeedMultiplier(2.0f);
}

void AI_WalkTowardPlayerEngage::OnUpdate(float dt) {
	pAgent->LookAtPlayer();
	pAgent->MoveToPlayer();
	if (idledSoFar > idleDuration) {
   		pAgent->ChangeState(NPC_ATTACK);
	}
	else {
		idledSoFar += dt;
	}
}

void AI_WalkTowardPlayerEngage::OnExit() {
}

void AI_WalkTowardPlayerEngage::HandleEvent(Event* pEvent) {

}

void AI_WalkTowardPlayerEngage::Serialize(const json& j) {
}