/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_RockBossIdle::AI_RockBossIdle()
: AI_State(NPC_State_RockBossIdle) {
}

AI_RockBossIdle::~AI_RockBossIdle(){
}

void AI_RockBossIdle::OnEnter(){
	//printf("IDLE ENTERED\n");
	pAgent->SetSpeedMultiplier(1.0f);
	idleDuration = RandomFloat(1, 3); 
	idledSoFar = 0.0f;
}

void AI_RockBossIdle::OnUpdate(float dt){
	if (pAgent->IsPlayerInSight()) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_ENGAGE);
		return;
	}
	if (idledSoFar < idleDuration) {
		idledSoFar += dt;
	}
	else {
		pAgent->ChangeState(NPC_MOVETORANDOMPOINTINZONE);
	}
}

void AI_RockBossIdle::OnExit(){
	
}


void AI_RockBossIdle::HandleEvent(Event* pEvent) {
}

void AI_RockBossIdle::Serialize(const json& j) {

}