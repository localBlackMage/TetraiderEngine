#include <Stdafx.h>

AI_Reaction::AI_Reaction()
: AI_State(NPC_State_Reaction) {

}

AI_Reaction::~AI_Reaction(){
}

void AI_Reaction::OnEnter(){
	// play reaction animation here//

	////////////////////////////////

	idleDuration = 1.0f;
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
	
}


void AI_Reaction::HandleEvent(Event* pEvent) {

}