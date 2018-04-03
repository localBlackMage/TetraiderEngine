//#include "AI_StateFactory.h"
//#include "AI_StateTypes.h"
//
//#include "AI_State.h"
//
//#include "AI_Idle.h"
//#include "AI_IdleForDuration.h"
//#include "AI_Engage.h"
//#include "AI_Attack.h"
//#include "AI_Retreat.h"
//#include "AI_MoveToRandomPointInZone.h"
//#include "AI_WalkTowardPlayerAttack.h"
//#include "AI_WalkTowardPlayerEngage.h"
//#include "AI_DashingAttack.h"
//#include "AI_DashingEngage.h"
//#include "AI_DashingStunned.h"

#include <Stdafx.h>

AIStateFactory::AIStateFactory(){
	m_CreationFunctions[StateText[NPC_State_Idle]] = AI_Idle::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_IdleForDuration]] = AI_IdleForDuration::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_Engage]] = AI_Engage::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_Attack]] = AI_Attack::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_RangeAttack]] = AI_RangeAttack::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_Retreat]] = AI_Retreat::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_MoveToRandomPointInZone]] = AI_MoveToRandomPointInZone::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_WalkTowardPlayerAttack]] = AI_WalkTowardPlayerAttack::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_WalkTowardPlayerEngage]] = AI_WalkTowardPlayerEngage::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_DashingAttack]] = AI_DashingAttack::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_DashingEngage]] = AI_DashingEngage::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_DashingStunned]] = AI_DashingStunned::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_HitNRunAttack]] = AI_HitNRunAttack::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_HitNRunEngage]] = AI_HitNRunEngage::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_HitNRunRetreat]] = AI_HitNRunRetreat::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_BomberAttack]] = AI_BomberAttack::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_BomberRetreat]] = AI_BomberRetreat::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_Reaction]] = AI_Reaction::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_EnemyBatReaction]] = AI_EnemyBatReaction::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_RatBossIdle]] = AI_RatBossIdle::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_RatBossReaction]] = AI_RatBossReaction::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_RatBossEngage]] = AI_RatBossEngage::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_RatBossAttack]] = AI_RatBossAttack::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_RatBossRetreat]] = AI_RatBossRetreat::CreateInstance;
}

AI_State* AIStateFactory::CreateState(std::string state){
	CreationFunction *Create = m_CreationFunctions[state];
	if (Create){
		return Create();
	}
	return nullptr;
}