/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

SubscriberTracker::SubscriberTracker(){}

SubscriberTracker::~SubscriberTracker() {}

void SubscriberTracker::AddSubscriber(Subscriber * subscriber)
{
	if (std::find(m_subscribers.begin(), m_subscribers.end(), subscriber) == m_subscribers.end() )
		m_subscribers.push_back(subscriber);
}

void SubscriberTracker::RemoveSubscriber(Subscriber * subscriber)
{
	m_subscribers.erase(
		std::remove(m_subscribers.begin(), m_subscribers.end(), subscriber),
		m_subscribers.end()
	);
}