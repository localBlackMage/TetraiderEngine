/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

class Subscriber;

class EventManager
{
private:
	std::unordered_map<EventType, std::vector<Subscriber*> > m_listeners;
	std::vector<Event*> m_events;
	double m_time;

public:
	EventManager();
	~EventManager();
	EventManager(const EventManager &) = delete;
	void operator=(const EventManager &) = delete;

	void Update(double deltaTime);
	void AddDelayedEvent(Event * newEvent);

	void Subscribe(const std::string eType, Subscriber* subscriber);
	void Subscribe(EventType eType, Subscriber* subscriber);

	void Unsubscribe(Subscriber* subscriber);
	void Unsubscribe(const EventType eType, Subscriber* subscriber);

	void UnsubscribeAll(Subscriber* subscriber);

	void BroadcastEvent(Event * aEvent);
	void BroadcastEventToSubscribers(Event * aEvent);

	void ClearDelayedEvents();

	/* This function does nothing, it is purely to have the EventManager be initialized first */
	inline void Init() const {};
};

#endif