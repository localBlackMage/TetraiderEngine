/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_HitNRunEngage::AI_HitNRunEngage()
	: AI_State(NPC_State_HitNRunEngage) {

}

AI_HitNRunEngage::~AI_HitNRunEngage() {
}

void AI_HitNRunEngage::OnEnter() {
	pAgent->LookAtPlayer();
	pAgent->SetSpeedMultiplier(1.3f);
	tryTimeDuration = RandomFloat(1.0f, 2.2f);
	minimumEngage = 2;
	engageTimer = RandomFloat(minimumEngage, 4);
	triedMovingSoFar = 0.0f;
	sinceEngage = 0.0f;
	minDistanceToPlayer = 210.0f;
	outOfSightTime = 0.0f;
}

void AI_HitNRunEngage::OnUpdate(float dt) {
	// always face player on engage
	pAgent->LookAtPlayer();

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

	// if player too close run away
	if (pAgent->IsPlayerTooClose(minDistanceToPlayer)) {
		pAgent->ChangeState(NPC_RETREAT);
		return;
	}
	// if this engage has gone too long, just go attack!
	if (sinceEngage > engageTimer) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_ATTACK);
		return;
	}
	// if this move has taken too much, change destination
	if (triedMovingSoFar > tryTimeDuration){
		pAgent->GoToPositionAroundPlayer();
		triedMovingSoFar = 0.0f;
		return;
	}

	// if player is in attack range, attack!
	if (pAgent->IsInAttackRange() && minimumEngage < sinceEngage) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_ATTACK);
		return;
	}
	triedMovingSoFar += dt;
	sinceEngage += dt;
}

void AI_HitNRunEngage::OnExit(){
	
}


void AI_HitNRunEngage::HandleEvent(Event* pEvent) {

}

void AI_HitNRunEngage::Serialize(const json& j) {
}