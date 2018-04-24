/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_MoveToRandomPointInZone::AI_MoveToRandomPointInZone()
: AI_State(NPC_State_MoveToRandomPointInZone) {

}

AI_MoveToRandomPointInZone::~AI_MoveToRandomPointInZone(){
}

void AI_MoveToRandomPointInZone::OnEnter(){
	pAgent->SetDestinationToRandomPointInZone();
	triedSofar = 0.0f;
	tryTime = 1.5f;
}

void AI_MoveToRandomPointInZone::OnUpdate(float dt){
	if (triedSofar > tryTime) {
		pAgent->ChangeState(NPC_IDLE);
	}
	else {
		triedSofar += dt;
	}
	pAgent->LookInDirectionOfMovement();
	if (pAgent->IsPlayerInSight()) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_REACTION);
		return;
	}
	if (pAgent->IsArrivedAtDestination()) {
		pAgent->ChangeState(NPC_IDLE);
	}
}

void AI_MoveToRandomPointInZone::OnExit(){
	
}

void AI_MoveToRandomPointInZone::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
	case EventType::EVENT_OnTakeDamage:
		pAgent->ChangeState(NPC_ENGAGE);
	}

}

void AI_MoveToRandomPointInZone::Serialize(const json& j) {
}