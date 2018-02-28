#include "Agent.h"
#include "AI_WalkTowardPlayer.h"
#include "TetraiderAPI.h"

AI_WalkTowardPlayer::AI_WalkTowardPlayer()
: AI_State(NPC_State_WalkTowardPlayer) {

}

AI_WalkTowardPlayer::~AI_WalkTowardPlayer(){
}

void AI_WalkTowardPlayer::OnEnter(){
	idleDuration = RandomFloat(0.5f, 0.9f); 
	idledSoFar = 0.0f;
	
}

void AI_WalkTowardPlayer::OnUpdate(float dt){
	if (idledSoFar > idleDuration) {
		pAgent->LookAtPlayer();
		pAgent->MoveToPlayer();
	}
	else {
		idledSoFar += dt;
	}
	if (pAgent->IsPlayerOutOfSight()) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_IDLE);
	}
}

void AI_WalkTowardPlayer::OnExit(){
	pAgent->GoToStartingPoint();
}

void AI_WalkTowardPlayer::HandleEvent(Event* pEvent) {

}