/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_WalkTowardPlayerIdle::AI_WalkTowardPlayerIdle()
: AI_State(NPC_State_WalkTowardPlayerIdle) {
	idleDuration = RandomFloat(1, 3); // get 1 - 3 sec
	idledSoFar = 0.0f;
}

AI_WalkTowardPlayerIdle::~AI_WalkTowardPlayerIdle(){
}

void AI_WalkTowardPlayerIdle::OnEnter(){
	// reset idle data
	idleDuration = RandomFloat(1, 3); 
	idledSoFar = 0.0f;
}

void AI_WalkTowardPlayerIdle::OnUpdate(float dt){
	if (pAgent->IsPlayerInSight()) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_REACTION);
		return;
	}
	if (idledSoFar < idleDuration) {
		idledSoFar += dt;
	}
	else {
		pAgent->ChangeState(NPC_MOVETORANDOMPOINTINZONE);
	}
}

void AI_WalkTowardPlayerIdle::OnExit(){
	
}


void AI_WalkTowardPlayerIdle::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
	case EventType::EVENT_OnTakeDamage:
		pAgent->ChangeState(NPC_ENGAGE);
	}
}

void AI_WalkTowardPlayerIdle::Serialize(const json& j) {

}