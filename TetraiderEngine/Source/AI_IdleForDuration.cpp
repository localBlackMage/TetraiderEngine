//#include "Agent.h"
//#include "AI_IdleForDuration.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

AI_IdleForDuration::AI_IdleForDuration()
: AI_State(NPC_State_IdleForDuration) {

}

AI_IdleForDuration::~AI_IdleForDuration(){
}

void AI_IdleForDuration::OnEnter(){
	triedSofar = 0.0f;
	tryTime = 1.5f;
	pAgent->StopMoving();
	pAgent->ControlAnimationOnVelocity(false);
}

void AI_IdleForDuration::OnUpdate(float dt){
	if (pAgent->IsPlayerInSight()) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_REACTION);
		return;
	}
	if (triedSofar > tryTime) {
		pAgent->GoToStartingPoint();
	}
	else {
		triedSofar += dt;
	}
	if (triedSofar > tryTime && pAgent->IsArrivedAtDestination()) {
		pAgent->ChangeState(NPC_IDLE);
	}
}

void AI_IdleForDuration::OnExit(){
	pAgent->ControlAnimationOnVelocity(true);
}


void AI_IdleForDuration::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
	case EventType::EVENT_OnTakeDamage:
		pAgent->ChangeState(NPC_REACTION);
	}
}

void AI_IdleForDuration::Serialize(const json& j) {
}