#include "AI_StateFactory.h"
#include "AI_StateTypes.h"

#include "AI_State.h"

#include "AI_Idle.h"
#include "AI_Engage.h"
#include "AI_Attack.h"
#include "AI_Retreat.h"

AIStateFactory::AIStateFactory(){
	m_CreationFunctions[StateText[NPC_State_Idle]] = AI_Idle::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_Engage]] = AI_Engage::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_Attack]] = AI_Attack::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_Retreat]] = AI_Retreat::CreateInstance;
}

AI_State* AIStateFactory::CreateState(std::string state){
	CreationFunction *Create = m_CreationFunctions[state];
	if (Create){
		return Create();
	}
	return nullptr;
}