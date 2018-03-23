#pragma once
#ifndef NPCCONTROLLER_H
#define NPCCONTROLLER_H

class Weapon;

#define ObstacleAvoidanceFrontCheack 125.0f
#define ObstacleAvoidanceSideCheck 125.0f
#define ObstacleAvoidanceSideCheckAngle 40.0f

enum class ObstacleAvoidanceDirection {
	RIGHT,
	LEFT
};

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

	void ChangeState(NPC_CONTROLLER_AI);
	void MoveToPlayer();
	void GoToPositionAroundPlayer();
	void MoveAwayFromPlayer(float dist);
	void GoToStartingPoint();
	bool UseAttack(int attack);
	bool IsArrivedAtDestination();
	bool IsPlayerInSight();
	bool IsPlayerOutOfSight();
	bool IsPlayerTooClose(float distance);
	bool IsInAttackRange();
	bool IsTooFarFromStartingPoint();
	void PlayAnimation(int animation);
	void SetDestinationToRandomPointInZone();
	void SetVelocityToZero();
	void StopMoving();
	void SetTargetDestination(const Vector3D& pos);
	void SetPositionBehindPlayer(float distance);
	void LookInDirectionOfMovement();
	void LookAtPlayer();
	void LookAtPlayer(float offsetAngle);
	void SetSpeedMultiplier(float speedMultiplier) { m_speedMultiplier = speedMultiplier; }
	bool RollDie(float probability);
	void SetAvoidObstacles(bool active) { m_isAvoidObstacles = active; }
	void ControlAnimationOnVelocity(bool active) { m_isControlAnimationOnVelocity = active; }
	bool IsAttackAnimComplete();
	void PlayAttackAnim();
protected:
	Weapon* m_pWeapon;
	float GetSquareDistanceToPlayer();
private:
	AIStateFactory AIStateFactory;
	AI_State* m_AIStates[NPC_NUM_BEHAVIOR];
	NPC_CONTROLLER_AI m_currentState;
	NPC_CONTROLLER_AI m_previousState;
	float m_detectionRadius;
	float m_outOfSightRadius;
	float m_zoneWidth;
	float m_zoneHeight;
	float m_attackRange;
	float m_speedMultiplier;
	Vector3D m_startingPoint;
	Vector3D m_targetDestination;
	Vector3D m_secondaryDestination;
	bool m_arrivedAtDestination;
	bool m_isPlayerDead;
	bool m_isAvoidObstacles;
	bool m_isAvoidingObstacle;
	bool m_isDeathAnim;
	int m_deathAnim;
	bool m_isAttackAnim;
	int m_attackAnim;
	const Transform* m_pPlayerTransform;
	GameObjectTag m_tagsToIgnore[3];
	GameObjectTag m_tagsToIgnoreForObstacleAvoidance[3];
	ObstacleAvoidanceDirection m_avoidDirection;

	bool IsPlayerOutOfScreen();
	void SetSecondaryTargetDestination(const Vector3D& pos) { m_secondaryDestination = pos; }
	void CheckForObstacleAvoidance();
	void AvoidObstacles(float dt);
	float m_stuckTimer = 0.0f;
	Vector3D m_prevPos;

	std::string m_healthBarPrefab;
	Vector3D m_healthBarPosOffset;
	float m_healthScale;
	GameObject* m_healthBarUI;
};

#endif

