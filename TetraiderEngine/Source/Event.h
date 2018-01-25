#pragma once

#ifndef EVENT_H
#define EVENT_H

#include <string>

#define REGISTER_EVENT_NAME(x) x,
typedef enum
{
	#include "EventNames.h"
	EVENT_OnCollide,
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

class Event
{
protected:
	double m_time;
	EventType m_type;
	// Payload
	// Message specific data
public:
	Event(EventType type, double time = 0.0);
	virtual ~Event();

	double Time() const;
	EventType Type() const;
	void DecrementTime(double amt);

	static EventType GetEventTypeFromTitle(std::string eventTitle);
};

#endif