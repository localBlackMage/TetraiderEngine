/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_BomberRetreat::AI_BomberRetreat()
: AI_State(NPC_State_BomberRetreat) {
	sinceRetreatMax = 0.5f;
}

AI_BomberRetreat::~AI_BomberRetreat(){
}

void AI_BomberRetreat::OnEnter(){
	pAgent->MoveAwayFromPlayer(200.0f);
	sinceRetreat = 0.0f;
}

void AI_BomberRetreat::OnUpdate(float dt){
	if (sinceRetreat > sinceRetreatMax) {
		pAgent->ChangeState(NPC_ENGAGE);
		return;
	}
	sinceRetreat += dt;
}

void AI_BomberRetreat::OnExit(){
	
}


void AI_BomberRetreat::HandleEvent(Event* pEvent) {

}

void AI_BomberRetreat::Serialize(const json& j) {
}