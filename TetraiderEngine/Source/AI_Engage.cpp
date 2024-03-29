/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_Engage::AI_Engage()
	: AI_State(NPC_State_Engage) {

}

AI_Engage::~AI_Engage() {
}

void AI_Engage::OnEnter() {
	pAgent->LookAtPlayer();
	pAgent->GoToPositionAroundPlayer();
	pAgent->SetSpeedMultiplier(1.3f);
	tryTimeDuration = RandomFloat(1.0f, 2.2f);
	minimumEngage = 2;
	engageTimer = RandomFloat(minimumEngage, 4);
	triedMovingSoFar = 0.0f;
	sinceEngage = 0.0f;
	outOfSightTime = 0.0f;
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
		if (outOfSightTime > OUT_OF_SIGHTTIME) {
			pAgent->ChangeState(NPC_IDLE);
			return;
		}
		else {
			outOfSightTime += dt;
			return;
		}
	}
	else {
		outOfSightTime = 0;
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


void AI_Engage::HandleEvent(Event* pEvent) {

}

void AI_Engage::Serialize(const json& j) {
}