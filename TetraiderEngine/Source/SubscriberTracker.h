/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef SUBSCRIBER_TRACKER_H
#define SUBSCRIBER_TRACKER_H

#include "EventManager.h"
//#include <vector>

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