#pragma once

#ifndef EVENT_H
#define EVENT_H

#include <string>
#include "Math\MathLibs.h"

// Macro trick to make Event names enums from the file EventNames.h
#define REGISTER_EVENT_NAME(x) x,
typedef enum
{
	#include "EventNames.h"
	EVENT_NUM
} EventType;
#undef REGISTER_EVENT_NAME

#define REGISTER_EVENT_NAME(x) #x,
static const char* EventNameText[] =
{
#include "EventNames.h"
	"Invalid"
};
#undef REGISTER_EVENT_NAME

class GameObject;

// Abstract struct, data objects should inherit from this
struct EventData {};

struct OnCollideData : public EventData {
	OnCollideData(GameObject* _pGO, MTV _mtv) :
		pGO(_pGO), mtv(_mtv) {}
	GameObject* pGO;
	MTV mtv;
};

struct HealthChangeData : public EventData {
	HealthChangeData(int _currentHealth, int _maxHealth, const Vector3D& sourceOfAttack, float knockBackSpeed) :
		mCurrentHealth(_currentHealth), mMaxHealth(_maxHealth), m_directionOfAttack(sourceOfAttack), mknockBackSpeed(knockBackSpeed){}
	int mCurrentHealth;
	int mMaxHealth;
	float mknockBackSpeed;
	Vector3D m_directionOfAttack;
};

struct StaminaChangeData : public EventData {
	StaminaChangeData(float _currentStamina, float _maxStamina) :
		mCurrentStamina(_currentStamina), mMaxStamina(_maxStamina) {}
	float mCurrentStamina;
	float mMaxStamina;
};

struct FPSData : public EventData {
	FPSData(float fps) : mFPS(fps) {}
	float mFPS;
};

struct InputButtonData : public EventData {
	InputButtonData(bool _isPressed, bool _isTrigger, bool _isReleased) : m_isPressed(_isPressed), m_isTrigger(_isTrigger), m_isReleased(_isReleased) {}
	bool m_isPressed;
	bool m_isTrigger;
	bool m_isReleased;
};

struct InputAxisData : public EventData {
	InputAxisData(const Vector3D& dir) : m_dir(dir) {}
	Vector3D m_dir;
};

struct CollectibleData : public EventData {
	CollectibleData(int value) : m_value(value) {}
	int m_value;
};

class Event
{
protected:
	double m_time;		// Time left until event is broadcast to subscribers
	EventType m_type;	// Type of event
	EventData* m_data;	// Data for event, if any
public:
	Event(EventType type, double time = 0.0);
	Event(EventType type, EventData* eventData, double time = 0.0);
	virtual ~Event();

	double Time() const { return m_time; }
	EventType Type() const { return m_type; }

	template <typename T>
	T* Data() const { return static_cast<T*>(m_data); }

	void DecrementTime(double amt) { m_time -= amt; }

	static EventType GetEventTypeFromTitle(std::string eventTitle);
};

#endif