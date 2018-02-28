#include "Agent.h"
#include "AI_IdleForDuration.h"
#include "TetraiderAPI.h"

AI_IdleForDuration::AI_IdleForDuration()
: AI_State(NPC_State_Retreat) {

}

AI_IdleForDuration::~AI_IdleForDuration(){
}

void AI_IdleForDuration::OnEnter(){
	triedSofar = 0.0f;
	tryTime = 2.0f;
	pAgent->StopMoving();
	printf("idleforduration\n");
}

void AI_IdleForDuration::OnUpdate(float dt){
	if (triedSofar > tryTime && pAgent->IsArrivedAtDestination()) {
		pAgent->ChangeState(NPC_IDLE);
	}
	if (triedSofar > tryTime) {
		pAgent->GoToStartingPoint();
	}
	else {
		triedSofar += dt;
	}

}

void AI_IdleForDuration::OnExit(){
}


void AI_IdleForDuration::HandleEvent(Event* pEvent) {

}