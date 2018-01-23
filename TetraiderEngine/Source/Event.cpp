#include "Event.h"
#include <unordered_map>

static std::unordered_map<std::string, EventType> TitleMap = {

};

Event::Event(EventType type, double time) :
	m_time(time), m_type(type)
{}

Event::~Event()
{
}

double Event::Time() const
{
	return m_time;
}

EventType Event::Type() const
{
	return m_type;
}

void Event::DecrementTime(double amt)
{
	m_time -= amt;
}

EventType Event::GetEventTypeFromTitle(std::string eventTitle)
{
	return TitleMap[eventTitle];
}
