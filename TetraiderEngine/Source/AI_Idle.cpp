#include "Agent.h"
#include "AI_Idle.h"
#include "TetraiderAPI.h"
#include "Math\MathFunctions.h"

#include <stdlib.h> // rand()
#include <iostream>

AI_Idle::AI_Idle()
: AI_State(NPC_State_Idle) {
	idleDuration = RandomInt(1, 3); // get 1 - 3 sec
	idledSoFar = 0.0f;
}

AI_Idle::~AI_Idle(){
}

void AI_Idle::OnEnter(){
	// reset idle data
	idleDuration = RandomInt(1, 3); 
	idledSoFar = 0.0f;
}

void AI_Idle::OnUpdate(float dt){
	if (pAgent->IsPlayerInSight()) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_ENGAGE);
		return;
	}
	if (idledSoFar < idleDuration) {
		idledSoFar += dt;
	}
	else {
		pAgent->ChangeState(NPC_MOVETORANDOMPOINTINZONE);
	}
}

void AI_Idle::OnExit(){
	
}


void AI_Idle::HandleEvent(Event* pEvent) {

}