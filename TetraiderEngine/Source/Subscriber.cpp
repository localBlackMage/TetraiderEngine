/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Subscriber::Subscriber()
{
	// This may need to be removed later. 
	_AddSubscriberToTracker();
}

Subscriber::~Subscriber()
{
	DeactivateSubscriber();
}

void Subscriber::_AddSubscriberToTracker()
{
	SubscriberTracker::GetInstance().AddSubscriber(this);
}

void Subscriber::_SubscribeToEvents(std::vector<std::string> events)
{
	for (std::string eventType : events)
		TETRA_EVENTS.Subscribe(eventType, this);
}

void Subscriber::AddEventSubscriptionToSubscribeTo(std::string eventType)
{
	m_eventsToSubscribeTo.push_back(eventType);
}

void Subscriber::SubscribeToEvent(std::string eventType)
{
	TETRA_EVENTS.Subscribe(eventType, this);
}

void Subscriber::SubscribeToEvent(EventType eventType)
{
	TETRA_EVENTS.Subscribe(eventType, this);
}

void Subscriber::ActivateSubscriber()
{
	SubscriberTracker::GetInstance().AddSubscriber(this);
}

void Subscriber::DeactivateSubscriber()
{
	SubscriberTracker::GetInstance().RemoveSubscriber(this);
	TETRA_EVENTS.UnsubscribeAll(this);
}

void* Subscriber::operator new(std::size_t size)
{
	return TETRA_MEMORY.Alloc(size);
}

void Subscriber::operator delete(void *ptr)
{
	TETRA_MEMORY.Free(ptr);
}
void* Subscriber::operator new[](std::size_t size)
{
	return TETRA_MEMORY.Alloc(size);
}

void Subscriber::operator delete[](void* arrayPtr)
{
	TETRA_MEMORY.Free(arrayPtr);
}
