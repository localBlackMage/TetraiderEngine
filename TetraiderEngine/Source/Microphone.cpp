#include "Microphone.h"
#include "GameObject.h"
#include "TetraiderAPI.h"
#include "Transform.h"
#include "Body.h"

Microphone::Microphone():Component(ComponentType::C_Microphone)
{
	
}


Microphone::~Microphone()
{
}

void Microphone::Update(float dt)
{
	//const Vector3D tempPos, tempVel;
	//tempPos = ;
	Vector3D pos = m_pObjTrans->GetPosition();
	TETRA_AUDIO.Set3dListener(pos/*, m_pBody->GetVelocity()*/ );
}

void Microphone::Serialize(const json & j)
{
}

void Microphone::LateInitialize()
{
	m_pObjTrans = pGO->GetComponent<Transform>(ComponentType::C_Transform);
	m_pBody = pGO->GetComponent<Body>(ComponentType::C_Body);
}
