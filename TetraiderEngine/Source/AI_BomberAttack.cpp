//#include "Agent.h"
//#include "AI_BomberAttack.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

AI_BomberAttack::AI_BomberAttack()
: AI_State(NPC_State_BomberAttack) {
	attackLimit = 3;
}

AI_BomberAttack::~AI_BomberAttack(){
}

void AI_BomberAttack::OnEnter(){
	attackCounter = RandomInt(1, attackLimit+1);
}

void AI_BomberAttack::OnUpdate(float dt){
	pAgent->LookAtPlayer();
	if (attackCounter > attackLimit) {
		pAgent->ChangeState(NPC_RETREAT);
	}
	pAgent->LookAtPlayer(RandomFloat(-15, 15));
	if (pAgent->UseAttack(0)) {
		attackCounter++;
	}
	pAgent->LookAtPlayer();
}

void AI_BomberAttack::OnExit(){
	pAgent->SetSpeedMultiplier(1.0f);
}

void AI_BomberAttack::HandleEvent(Event* pEvent) {

}

void AI_BomberAttack::Serialize(const json& j) {
}