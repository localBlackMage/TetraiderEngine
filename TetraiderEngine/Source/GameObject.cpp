//#include "GameObject.h"
//#include "Component.h"
//#include "Event.h"
//#include "Transform.h"
//#include "TetraiderAPI.h"
//#include <algorithm>

#include <Stdafx.h>

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

	m_activeComponents.clear();
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

void GameObject::Deactivate() {
	m_isActive = true;
	m_isRender = true;
	m_destroyTimer = 0;
	m_destroySetTimeStamp = 0;
	m_isDestroy = false;
	m_isSetToDestroy = false;
	for (int i = 0; i < ComponentType::NUM_COMPONENTS; ++i) {
		if (mComponents[i]) {
			TETRA_MEMORY.DeleteComponent(mComponents[i]);
			mComponents[i] = nullptr;
		}
	}

	m_activeComponents.clear();
}

void GameObject::OverrideComponents(const json & j)
{
	for (unsigned int i = 0; i < m_activeComponents.size(); ++i) {
		mComponents[(int)m_activeComponents[i]]->Override(j);
	}
}

void GameObject::Update(float dt) {
	for (unsigned int i = 0; i < m_activeComponents.size(); ++i) {
		mComponents[(int)m_activeComponents[i]]->Update(dt);
	}

	if (m_isSetToDestroy) {
		if (TETRA_FRAMERATE.GetElapsedTime() - m_destroySetTimeStamp > m_destroyTimer)
			Destroy();
	}
}

void GameObject::LateUpdate(float dt) {
	for (unsigned int i = 0; i < m_activeComponents.size(); ++i) {
		mComponents[(int)m_activeComponents[i]]->LateUpdate(dt);
	}
}

void GameObject::LateInitialize() {
	for (unsigned int i = 0; i < m_activeComponents.size(); ++i) {
		mComponents[(int)m_activeComponents[i]]->LateInitialize();
	}
}

void GameObject::AddComponent(Component* pComponent) {
	pComponent->pGO = this;
	mComponents[pComponent->Type()] = pComponent;
	m_activeComponents.push_back(pComponent->Type());
}

bool GameObject::HasComponent(ComponentType type) const
{
	return mComponents[type];
}

void GameObject::HandleEvent(Event* pEvent) {
	for (unsigned int i = 0; i < m_activeComponents.size(); ++i) {
		mComponents[(int)m_activeComponents[i]]->HandleEvent(pEvent);
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

void GameObject::SetActive(bool active) {
	m_isActive = active;
	m_isRender = active;
	m_isCollisionDisabled = !active;
}

void GameObject::UpdateForLevelEditor(float dt) {
	if (mComponents[ComponentType::C_Transform])
		mComponents[ComponentType::C_Transform]->Update(dt);
	if (mComponents[ComponentType::C_Sprite])
		mComponents[ComponentType::C_Sprite]->Update(dt);
	if (mComponents[ComponentType::C_Body])
		mComponents[ComponentType::C_Body]->Update(dt);
	if (mComponents[ComponentType::C_Camera])
		mComponents[ComponentType::C_Camera]->Update(dt);
	if (mComponents[ComponentType::C_GateTriggerBox])
		mComponents[ComponentType::C_GateTriggerBox]->Update(dt);

	if (m_isSetToDestroy) {
		if (TETRA_FRAMERATE.GetElapsedTime() - m_destroySetTimeStamp > m_destroyTimer)
			Destroy();
	}
}

void GameObject::LateUpdateForLevelEditor(float dt) {
	if (mComponents[ComponentType::C_Transform])
		mComponents[ComponentType::C_Transform]->LateUpdate(dt);
	if (mComponents[ComponentType::C_Sprite])
		mComponents[ComponentType::C_Sprite]->LateUpdate(dt);
	if (mComponents[ComponentType::C_Body])
		mComponents[ComponentType::C_Body]->LateUpdate(dt);
	if (mComponents[ComponentType::C_Camera])
		mComponents[ComponentType::C_Camera]->LateUpdate(dt);
	if (mComponents[ComponentType::C_GateTriggerBox])
		mComponents[ComponentType::C_GateTriggerBox]->LateUpdate(dt);
}
