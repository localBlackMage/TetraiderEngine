#pragma once

#ifndef EVENT_H
#define EVENT_H

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

struct WindowResizedData : public EventData {
	WindowResizedData(int _width, int _height) :
		width(_width), height(_height) {}

	int width, height;
};

struct OnCollideData : public EventData {
	OnCollideData(GameObject* _pGO, MTV _mtv) :
		pGO(_pGO), mtv(_mtv) {}
	GameObject* pGO;
	MTV mtv;
};

struct HealthChangeData : public EventData {
	HealthChangeData(int _currentHealth, int _maxHealth, const Vector3D& sourceOfAttack, float knockBackSpeed, bool isForceKnockBack) :
		mCurrentHealth(_currentHealth), mMaxHealth(_maxHealth), m_directionOfAttack(sourceOfAttack), mknockBackSpeed(knockBackSpeed), mIsForceKnockBack(isForceKnockBack) {}
	int mCurrentHealth;
	int mMaxHealth;
	float mknockBackSpeed;
	bool mIsForceKnockBack;
	Vector3D m_directionOfAttack;
};

struct StaminaChangeData : public EventData {
	StaminaChangeData(float _currentStamina, float _maxStamina) :
		mCurrentStamina(_currentStamina), mMaxStamina(_maxStamina) {}
	float mCurrentStamina;
	float mMaxStamina;
};

struct FloatData : public EventData {
	FloatData(float value) : mValue(value) {}
	float mValue;
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

struct CanvasData : public EventData {
	CanvasData(int type) : m_canvasType(type) {}
	int m_canvasType;
};

struct ChoiceData : public EventData {
	ChoiceData(int value) : m_choiceData(value) {}
	int m_choiceData;
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