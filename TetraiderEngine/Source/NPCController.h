#pragma once
#ifndef NPCCONTROLLER_H
#define NPCCONTROLLER_H

#include "Agent.h"
#include <unordered_map>
#include "AI_State.h"
#include "AI_StateTypes.h"
#include "AI_StateFactory.h"

class Weapon;

class NPCController : public Agent {
public:
	NPCController();
	~NPCController();
	static Component* CreateInstance() { return new NPCController(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
	void MoveToPlayer();
	void MoveAroundPlayer();
	bool UseAttack(int attack);
protected:
	Weapon* m_pWeapon;
	float GetSquareDistanceToPlayer();
	bool IsArrivedAtDestination();
	bool IsPlayerWithinDistance();
	bool IsPlayerOutOfSight();
	void SetDestinationToRandomPointInZone();
	void StopMoving();
	void SetTargetDestination(const Vector3D& pos);
	void LookInDirectionOfMovement();
	void LookAtPlayer();
	void LookAtPlayer(float offsetAngle);
	bool RollDie(float probability);
private:
	AIStateFactory AIStateFactory;
	AI_State* m_AIStates[NPC_NUM_BEHAVIOR];
	NPC_CONTROLLER_AI m_currentState;
	NPC_CONTROLLER_AI m_previousState;
	float m_detectionRadius;
	float m_outOfSightRadius;
	float m_zoneWidth;
	float m_zoneHeight;
	Vector3D m_startingPoint;
	Vector3D m_targetDestination;
	bool m_arrivedAtDestination;

	const Transform* m_pPlayerTransform;
};

#endif

