/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef UI_COUNTER_H
#define UI_COUNTER_H

class UICounter : public Component {
public:
	UICounter();
	~UICounter();
	static Component* CreateInstance() { return new UICounter(); }
	virtual void DeActivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
private:
	Text* m_pText;
	EventType m_eventType;
	int m_counter;
	bool m_isSubscribed;
};

#endif

