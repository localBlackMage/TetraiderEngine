#include "GameObject.h"
#include "Component.h"
#include "Event.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include <algorithm>

GameObject::GameObject(unsigned int id) : 
	m_id(id), 
	m_isDestroy(false), m_isActive(true), 
	m_isCollisionDisabled(false), 
	m_isRender(true),
	m_isSetToDestroy(false),
	m_destroyTimer(0),
	m_destroySetTimeStamp(0)
{
	std::fill_n(mComponents, int(ComponentType::NUM_COMPONENTS), nullptr);
}

GameObject::~GameObject() {
	for (int i = 0; i < ComponentType::NUM_COMPONENTS; ++i) {
		if (mComponents[i])
			TETRA_MEMORY.DeleteComponent(mComponents[i]);
	}
}

bool GameObject::operator==(const GameObject& rhs) const
{
	return rhs.m_id == m_id;
}

void GameObject::Destroy() {
	m_isDestroy = true;
}

void GameObject::DestroyIn(float time) {
	m_isSetToDestroy = true;
	m_destroySetTimeStamp = TETRA_FRAMERATE.GetElapsedTime();
	m_destroyTimer = time;
}

void GameObject::Update(float dt) {
	for (int i = 0; i < ComponentType::NUM_COMPONENTS; ++i) {
		if (mComponents[i])
			mComponents[i]->Update(dt);
	}

	if (m_isSetToDestroy) {
		if (TETRA_FRAMERATE.GetElapsedTime() - m_destroySetTimeStamp > m_destroyTimer)
			Destroy();
	}
}

void GameObject::LateUpdate(float dt) {
	for (int i = 0; i < ComponentType::NUM_COMPONENTS; ++i) {
		if (mComponents[i])
			mComponents[i]->LateUpdate(dt);
	}
}

void GameObject::LateInitialize() {
	for (int i = 0; i < ComponentType::NUM_COMPONENTS; ++i) {
		if (mComponents[i])
			mComponents[i]->LateInitialize();
	}
}

void GameObject::AddComponent(Component* pComponent) {
	pComponent->pGO = this;
	mComponents[pComponent->Type()] = pComponent;
}

bool GameObject::HasComponent(ComponentType type) const
{
	return mComponents[type];
}

void GameObject::HandleEvent(Event* pEvent) {
	for (int i = 0; i < ComponentType::NUM_COMPONENTS; ++i) {
		if (mComponents[i])
			mComponents[i]->HandleEvent(pEvent);
	}
}

void GameObject::SetParent(GameObject* pParent) {
	Transform* myTransform = GetComponent<Transform>(ComponentType::C_Transform);
	if (myTransform) {
		Transform* parentTransform = pParent->GetComponent<Transform>(ComponentType::C_Transform);
		if (parentTransform) 
			myTransform->SetParent(parentTransform);
	}
}
