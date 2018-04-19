/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

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
	m_CreationFunctions[StateText[NPC_State_WalkTowardPlayerIdle]] = AI_WalkTowardPlayerIdle::CreateInstance;
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
	m_CreationFunctions[StateText[NPC_State_RockBossIdle]] = AI_RockBossIdle::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_RockBossReaction]] = AI_RockBossReaction::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_RockBossEngage]] = AI_RockBossEngage::CreateInstance;
	m_CreationFunctions[StateText[NPC_State_RockBossAttack]] = AI_RockBossAttack::CreateInstance;
}

AI_State* AIStateFactory::CreateState(std::string state){
	CreationFunction *Create = m_CreationFunctions[state];
	if (Create){
		return Create();
	}
	return nullptr;
}