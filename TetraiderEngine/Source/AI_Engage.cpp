#include "Agent.h"
#include "AI_Engage.h"
#include "TetraiderAPI.h"

AI_Engage::AI_Engage()
	: AI_State(NPC_State_Engage) {

}

AI_Engage::~AI_Engage() {
}

void AI_Engage::OnEnter() {
	pAgent->LookAtPlayer();
	pAgent->GoToPositionAroundPlayer();
	tryTimeDuration = 5;
	minimumEngage = 3;
	engageTimer = RandomInt(minimumEngage, 10);
	triedMovingSoFar = 0.0f;
	sinceEngage = 0.0f;
}

void AI_Engage::OnUpdate(float dt) {
	// always face player on engage
	pAgent->LookAtPlayer();
	// if this engage has gone too long, just go attack!
	if (sinceEngage > engageTimer) {
		pAgent->ChangeState(NPC_ATTACK);
		return;
	}
	// if this move has taken too much, change destination
	if (triedMovingSoFar > tryTimeDuration){
		pAgent->GoToPositionAroundPlayer();
		triedMovingSoFar = 0.0f;
		return;
	}
	// if player is out of sight, go back to idle
	if (pAgent->IsPlayerOutOfSight()) {
		pAgent->ChangeState(NPC_IDLE);
		return;
	}
	// if player is in attack range, attack!
	if (pAgent->IsInAttackRange() && minimumEngage < sinceEngage) {
		pAgent->ChangeState(NPC_ATTACK);
		return;
	}
	// if next destination reached, pick another destination
	if (pAgent->IsArrivedAtDestination()) {
		pAgent->GoToPositionAroundPlayer();
		triedMovingSoFar = 0.0f;
		return;
	}
	triedMovingSoFar += dt;
	sinceEngage += dt;
}

void AI_Engage::OnExit(){
	
}