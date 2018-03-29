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

