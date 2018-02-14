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

	//! void HandleEvent(Event * p_event)
	/*!
	\param p_event Pointer to an Event object 
	Class method to be overridden by inherited classes. This method will be called when the Subscriber needs
	to respond to an event either through broadcasts or subscription.
	*/
	virtual void HandleEvent(Event * p_event) {};

	//! void UnsubscribeChildren(std::string eventType)
	/*!
	\param eventType String that matches with an EventType for this Subscriber to unsubscribe child objects from
	Class method to be overridden by inherited classes. This method should unscubscribe any child objects
	from the specified eventType and then call to it's children to do the same.
	*/
	virtual void UnsubscribeChildren(std::string eventType) {};

	//! void UnsubscribeChildrenFromAll()
	/*!
	\param eventType String that matches with an EventType for this Subscriber to unsubscribe child objects from
	Class method to be overridden by inherited classes. This method should unscubscribe any child objects
	from the specified eventType and then call to it's children to do the same.
	*/
	virtual void UnsubscribeChildrenFromAll() {};

	virtual void AddEventSubscriptionToSubscribeTo(std::string eventType);
	virtual void SubscribeToEvent(std::string eventType);
	virtual void SubscribeToEvent(EventType eventType);


	//void *operator new(std::size_t size);
	//void operator delete(void *ptr);
	//void* operator new[](std::size_t size);
	//void operator delete[](void* arrayPtr);
};

#endif