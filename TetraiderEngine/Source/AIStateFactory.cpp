#include "AIStateFactory.h"

AIStateFactory::AIStateFactory(){
	CreationFunctions[StateText[AI_Idle]] = AI_Idle::CreateInstance;
	CreationFunctions[StateText[AI_Engage]] = AI_Engage::CreateInstance;
	CreationFunctions[StateText[AI_Attack]] = AI_Attack::CreateInstance;
	CreationFunctions[StateText[AI_Retreat]] = AI_Retreat::CreateInstance;
}

AIState* AIStateFactory::CreateState(std::string state){
	AIState* Create = m_CreationFunctions[state];
	if (Create){
		return Create();
	}
	return nullptr;
}