#include <Stdafx.h>

AI_EnemyBatReaction::AI_EnemyBatReaction()
: AI_State(NPC_State_EnemyBatReaction) {

}

AI_EnemyBatReaction::~AI_EnemyBatReaction(){
}

void AI_EnemyBatReaction::OnEnter(){
	// play reaction animation here//
	pAgent->StopMoving();
	pAgent->LookAtPlayer();
	pAgent->PlayReactionEffect();
	pAgent->ControlAnimationOnVelocity(false);
	////////////////////////////////

	idleDuration = 0.75f;
	idledSoFar = 0.0f;
}

void AI_EnemyBatReaction::OnUpdate(float dt){
	if (idledSoFar > idleDuration) {
		pAgent->ChangeState(NPC_ENGAGE);
	}
	else {
		idledSoFar += dt;
	}
}

void AI_EnemyBatReaction::OnExit(){
	pAgent->ControlAnimationOnVelocity(true);
}


void AI_EnemyBatReaction::HandleEvent(Event* pEvent) {

}