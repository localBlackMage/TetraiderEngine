/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_Retreat::AI_Retreat()
: AI_State(NPC_State_Retreat) {

}

AI_Retreat::~AI_Retreat(){
}

void AI_Retreat::OnEnter(){

}

void AI_Retreat::OnUpdate(float dt){

}

void AI_Retreat::OnExit(){
	
}


void AI_Retreat::HandleEvent(Event* pEvent) {

}

void AI_Retreat::Serialize(const json& j) {
}