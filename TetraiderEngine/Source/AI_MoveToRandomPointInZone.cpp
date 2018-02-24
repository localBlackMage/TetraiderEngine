#include "Agent.h"
#include "AI_MoveToRandomPointInZone.h"
#include "TetraiderAPI.h"

AI_MoveToRandomPointInZone::AI_MoveToRandomPointInZone()
: AI_State(NPC_State_MoveToRandomPointInZone) {

}

AI_MoveToRandomPointInZone::~AI_MoveToRandomPointInZone(){
}

void AI_MoveToRandomPointInZone::OnEnter(){
	pAgent->SetDestinationToRandomPointInZone();
}

void AI_MoveToRandomPointInZone::OnUpdate(float dt){
	pAgent->LookInDirectionOfMovement();
	if (pAgent->IsPlayerInSight()) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_ENGAGE);
		return;
	}
	if (pAgent->IsArrivedAtDestination()) {
		pAgent->ChangeState(NPC_IDLE);
	}
}

void AI_MoveToRandomPointInZone::OnExit(){
	
}