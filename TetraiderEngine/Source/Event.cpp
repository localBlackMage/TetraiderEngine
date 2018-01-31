#include "Event.h"
#include <unordered_map>

static std::unordered_map<std::string, EventType> TitleMap = {

};

Event::Event(EventType type, double time) :
	m_time(time), m_type(type), m_data(nullptr)
{}

Event::Event(EventType type, EventData * eventData, double time) : 
	m_time(time), m_type(type), m_data(eventData)
{
}

Event::~Event()
{
}

EventType Event::GetEventTypeFromTitle(std::string eventTitle)
{
	return TitleMap[eventTitle];
}
