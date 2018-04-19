/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef TIME_TO_LIVE_H
#define TIME_TO_LIVE_H

class TimeToLive : public Component 
{
protected:	
	float m_ttl;

public:
	TimeToLive();
	~TimeToLive();
	static Component* CreateInstance() { return new TimeToLive(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	void Override(const json& j) {};

};

#endif

