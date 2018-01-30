#include "Subscriber.h"
#include "EventManager.h"
#include "SubscriberTracker.h"

Subscriber::Subscriber()
{
	// This may need to be removed later. 
	_AddSubscriberToTracker();
}

Subscriber::~Subscriber()
{
	SubscriberTracker::GetInstance().RemoveSubscriber(this);
}

void Subscriber::_AddSubscriberToTracker()
{
	SubscriberTracker::GetInstance().AddSubscriber(this);
}

void Subscriber::_SubscribeToEvents(std::vector<std::string> events)
{
	for (std::string eventType : events)
		EventManager::GetInstance().Subscribe(eventType, this);
}

void Subscriber::AddEventSubscriptionToSubscribeTo(std::string eventType)
{
	m_eventsToSubscribeTo.push_back(eventType);
}

void Subscriber::SubscribeToEvent(std::string eventType)
{
	EventManager::GetInstance().Subscribe(eventType, this);
}

void Subscriber::SubscribeToEvent(EventType eventType)
{
	EventManager::GetInstance().Subscribe(eventType, this);
}
