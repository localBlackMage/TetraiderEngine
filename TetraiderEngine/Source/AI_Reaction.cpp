/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_Reaction::AI_Reaction()
: AI_State(NPC_State_Reaction) {

}

AI_Reaction::~AI_Reaction(){
}

void AI_Reaction::OnEnter(){
	// play reaction animation here//
	pAgent->StopMoving();
	pAgent->LookAtPlayer();
	pAgent->PlayReactionEffect();
	////////////////////////////////

	idleDuration = 0.75f;
	idledSoFar = 0.0f;
}

void AI_Reaction::OnUpdate(float dt){
	if (idledSoFar > idleDuration) {
		pAgent->ChangeState(NPC_ENGAGE);
	}
	else {
		idledSoFar += dt;
	}
}

void AI_Reaction::OnExit(){
	pAgent->ControlAnimationOnVelocity(true);
}


void AI_Reaction::HandleEvent(Event* pEvent) {

}

void AI_Reaction::Serialize(const json& j) {
}