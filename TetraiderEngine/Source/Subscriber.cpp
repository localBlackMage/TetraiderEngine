#include "Subscriber.h"
#include "TetraiderAPI.h"
#include "SubscriberTracker.h"
#include <unordered_map>
#include <string>

using namespace std;
static unordered_map<string, int> pointerCount;


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

void* Subscriber::operator new(std::size_t size)
{
	void* val = TETRA_MEMORY.Alloc(size);

	char str[40];
	sprintf_s(str, "%p\n", val);
	++pointerCount[str];

	if (pointerCount[str] > 1) {
		printf("Duplicate address returned!");
	}
	return val;
}

void Subscriber::operator delete(void *ptr)
{
	char str[40];
	sprintf_s(str, "%p\n", ptr);
	--pointerCount[str];
	TETRA_MEMORY.Free(ptr);
}
void* Subscriber::operator new[](std::size_t size)
{
	printf("Allocating array.\n");
	return TETRA_MEMORY.Alloc(size);
}

void Subscriber::operator delete[](void* arrayPtr)
{
	printf("Deallocating array.\n");
	TETRA_MEMORY.Free(arrayPtr);
}
