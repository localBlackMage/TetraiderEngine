//#include "Agent.h"
//#include "AI_HitNRunRetreat.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

AI_HitNRunRetreat::AI_HitNRunRetreat()
: AI_State(NPC_State_Retreat) {
	minimumRetreat = 1.5;

}

AI_HitNRunRetreat::~AI_HitNRunRetreat(){

}

void AI_HitNRunRetreat::OnEnter(){
	minDistanceToPlayer = 250.0f;
	tryTimeDuration = RandomFloat(1.0f, 2.2f);
	triedMovingSoFar = 0.0f;
	sinceRetreat = 0.0f;
	pAgent->SetSpeedMultiplier(1.75f);
	pAgent->MoveAwayFromPlayer(minDistanceToPlayer);
	pAgent->LookInDirectionOfMovement();
}

void AI_HitNRunRetreat::OnUpdate(float dt){
	pAgent->LookInDirectionOfMovement();
	if (!pAgent->IsPlayerTooClose(minDistanceToPlayer) && (sinceRetreat > minimumRetreat)){
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_ATTACK);
		return;
	}
	if ((triedMovingSoFar > tryTimeDuration) && (sinceRetreat > minimumRetreat)) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_ATTACK);
		//pAgent->GoToPositionAroundPlayer(minDistanceToPlayer);
		//triedMovingSoFar = 0.0f;
		return;
	}
	if (pAgent->IsArrivedAtDestination()) {
		if (pAgent->IsPlayerTooClose(minDistanceToPlayer)) {
			pAgent->ChangeState(NPC_ATTACK);
		}
	}
	triedMovingSoFar += dt;
	sinceRetreat += dt;
}

void AI_HitNRunRetreat::OnExit(){
	pAgent->SetSpeedMultiplier(1.0f);
}


void AI_HitNRunRetreat::HandleEvent(Event* pEvent) {

}