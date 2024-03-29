/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_WalkTowardPlayerAttack::AI_WalkTowardPlayerAttack()
: AI_State(NPC_State_WalkTowardPlayerAttack) {

}

AI_WalkTowardPlayerAttack::~AI_WalkTowardPlayerAttack(){
}

void AI_WalkTowardPlayerAttack::OnEnter(){
	idleDuration = RandomFloat(0.5f, 0.9f); 
	idledSoFar = 0.0f;
	pAgent->LookAtPlayer();
	outOfSightTime = 0.0f;
}

void AI_WalkTowardPlayerAttack::OnUpdate(float dt){
	if (idledSoFar > idleDuration) {
		pAgent->LookAtPlayer();
		pAgent->MoveToPlayer();
	}
	else {
		idledSoFar += dt;
	}

	if (pAgent->IsPlayerOutOfSight()) {
		if (outOfSightTime > OUT_OF_SIGHTTIME) {
			pAgent->ChangeState(NPC_STUNNED);
			return;
		}
		else {
			outOfSightTime += dt;
			return;
		}
	}
	else {
		outOfSightTime = 0;
	}
}

void AI_WalkTowardPlayerAttack::OnExit(){
	pAgent->SetSpeedMultiplier(1.0f);
}

void AI_WalkTowardPlayerAttack::HandleEvent(Event* pEvent) {

}

void AI_WalkTowardPlayerAttack::Serialize(const json& j) {
}