/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

TimeToLive::TimeToLive() :
	Component(ComponentType::C_TimeToLive),
	m_ttl(0.f)
{}

TimeToLive::~TimeToLive() {}

void TimeToLive::Deactivate() { 
	pGO = nullptr;
}

void TimeToLive::Update(float dt) 
{
	m_ttl -= dt;

	if (m_ttl < 0.f)
		pGO->Destroy();
}

void TimeToLive::Serialize(const json& j)
{
	m_ttl = ParseFloat(j, "time");
}

