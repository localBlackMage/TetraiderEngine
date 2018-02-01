#include "Event.h"
#include <unordered_map>

Event::Event(EventType type, double time) :
	m_time(time), m_type(type), m_data(nullptr)
{}

Event::Event(EventType type, EventData * eventData, double time) : 
	m_time(time), m_type(type), m_data(eventData)
{
}

Event::~Event(){}

EventType Event::GetEventTypeFromTitle(std::string eventTitle)
{
	unsigned int idx = 0;
	for (const char* title : EventNameText) {
		if (eventTitle == title) {
			return EventType(idx);
		}
		++idx;
	}
	return EventType::INVALID_EVENT;
}
