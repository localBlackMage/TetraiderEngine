/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_EnemyBatReaction::AI_EnemyBatReaction()
: AI_State(NPC_State_EnemyBatReaction) {

}

AI_EnemyBatReaction::~AI_EnemyBatReaction(){
}

void AI_EnemyBatReaction::OnEnter(){
	// play reaction animation here//
	pAgent->StopMoving();
	pAgent->LookAtPlayer();
	pAgent->PlayReactionEffect();
	pAgent->ControlAnimationOnVelocity(false);
	////////////////////////////////

	idleDuration = 0.75f;
	idledSoFar = 0.0f;
}

void AI_EnemyBatReaction::OnUpdate(float dt){
	if (idledSoFar > idleDuration) {
		pAgent->ChangeState(NPC_ENGAGE);
	}
	else {
		idledSoFar += dt;
	}
}

void AI_EnemyBatReaction::OnExit(){
	pAgent->ControlAnimationOnVelocity(true);
}


void AI_EnemyBatReaction::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
	case EventType::EVENT_OnTakeDamage:
		pAgent->ChangeState(NPC_ENGAGE);
	}
}

void AI_EnemyBatReaction::Serialize(const json& j) {
}