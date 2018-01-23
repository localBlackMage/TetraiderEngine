#pragma once

#ifndef SUBSCRIBER_TRACKER_H
#define SUBSCRIBER_TRACKER_H

#include "EventManager.h"
#include <vector>

class Subscriber;

class SubscriberTracker
{
private:
	std::vector<Subscriber*> m_subscribers;

	friend EventManager;

	SubscriberTracker();
	~SubscriberTracker();
public:
	SubscriberTracker(const SubscriberTracker &) = delete;
	void operator=(const SubscriberTracker &) = delete;

	static SubscriberTracker& GetInstance()
	{
		static SubscriberTracker instance;
		return instance;
	}

	void AddSubscriber(Subscriber* subscriber);
	void RemoveSubscriber(Subscriber* subscriber);
};
#endif