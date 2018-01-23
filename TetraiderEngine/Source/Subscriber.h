#pragma once

#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "Event.h"
#include <vector>

class Subscriber
{
protected:
	std::vector<std::string> m_eventsToSubscribeTo;

	void _AddSubscriberToTracker();
	void _SubscribeToEvents(std::vector<std::string> events);
public:
	Subscriber();
	virtual ~Subscriber();
	virtual void HandleEvent(Event * p_event) {};
	virtual void AddEventSubscriptionToSubscribeTo(std::string eventType);
	virtual void SubscribeToEvent(std::string eventType);
	virtual void SubscribeToEvent(EventType eventType);
	virtual void UnsubscribeChildren(std::string eventType) {};
	virtual void UnsubscribeChildrenFromAll() {};
};

#endif