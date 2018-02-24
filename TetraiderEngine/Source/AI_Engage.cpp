#include "Agent.h"
#include "AI_Engage.h"
#include "TetraiderAPI.h"

AI_Engage::AI_Engage()
: AI_State(NPC_State_Engage) {

}

AI_Engage::~AI_Engage(){
}

void AI_Engage::OnEnter(){
	pAgent->LookAtPlayer();
	pAgent->MoveAroundPlayer();
}

void AI_Engage::OnUpdate(float dt){
	if (pAgent->IsPlayerOutOfSight()) {
		pAgent->ChangeState(NPC_IDLE);
	}
	pAgent->LookAtPlayer();
	//if (pAgent->) {
	//	pAgent->ChangeState(NPC_ATTACK);
	//}
	if (pAgent->IsArrivedAtDestination()) {
		pAgent->MoveAroundPlayer();
	}
}

void AI_Engage::OnExit(){
	
}