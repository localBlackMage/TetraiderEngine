#include "Agent.h"
#include "AI_WalkTowardPlayerEngage.h"
#include "TetraiderAPI.h"

AI_WalkTowardPlayerEngage::AI_WalkTowardPlayerEngage()
	: AI_State(NPC_State_WalkTowardPlayerEngage) {

}

AI_WalkTowardPlayerEngage::~AI_WalkTowardPlayerEngage() {
}

void AI_WalkTowardPlayerEngage::OnEnter() {
	idleDuration = 4.0f;
	idledSoFar = 0.0f;
}

void AI_WalkTowardPlayerEngage::OnUpdate(float dt) {
	pAgent->LookAtPlayer();
	pAgent->MoveToPlayer();
	if (idledSoFar > idleDuration) {
		pAgent->ChangeState(NPC_ATTACK);
	}
	else {
		idledSoFar += dt;
	}
}

void AI_WalkTowardPlayerEngage::OnExit() {
}

void AI_WalkTowardPlayerEngage::HandleEvent(Event* pEvent) {

}