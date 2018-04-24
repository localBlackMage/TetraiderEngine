/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Sujay Shah>
- End Header --------------------------------------------------------*/

#pragma once

class Microphone : public Component
{
public:
	Microphone();
	~Microphone();
	static Component* CreateInstance() { return new Microphone(); }
	
	// Inherited via Component
	void Update(float dt) ;
	void Serialize(const json & j);
	void LateInitialize();
private:
	Transform* m_pObjTrans;
	Body* m_pBody;
};

