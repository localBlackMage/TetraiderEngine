#include "Subscriber.h"
#include "TetraiderAPI.h"
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

//void* Subscriber::operator new(std::size_t size)
//{
//	return TETRA_MEMORY.Alloc(size);
//}
//
//void Subscriber::operator delete(void *ptr)
//{
//	TETRA_MEMORY.Free(ptr);
//}
//void* Subscriber::operator new[](std::size_t size)
//{
//	return TETRA_MEMORY.Alloc(size);
//}
//
//void Subscriber::operator delete[](void* arrayPtr)
//{
//	TETRA_MEMORY.Free(arrayPtr);
//}
