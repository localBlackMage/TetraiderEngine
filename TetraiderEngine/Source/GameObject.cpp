#include "GameObject.h"
#include "Component.h"
#include "Event.h"
#include <algorithm>

GameObject::GameObject(unsigned int id) : 
	m_id(id), 
	m_isDestroy(false), m_isActive(true), 
	m_isCollisionDisabled(false), 
	m_isRender(true) 
{
	std::fill_n(mComponents, ComponentType::NUM_COMPONENTS, nullptr);
}

GameObject::~GameObject() {
	for (int i = 0; i < ComponentType::NUM_COMPONENTS; ++i)
	{
		if (mComponents[i] != nullptr)
			delete mComponents[i];
	}
}

bool GameObject::operator==(const GameObject& rhs) const
{
	return rhs.m_id == m_id;
}

void GameObject::Destroy() {
	m_isDestroy = true;
}

void GameObject::Update(float dt) {
	for (auto &comp : mComponents)
		comp->Update(dt);
}

void GameObject::LateUpdate(float dt) {
	for (auto &comp : mComponents)
		comp->LateUpdate(dt);
}

void GameObject::LateInitialize() {
	for (auto &comp : mComponents)
		comp->LateInitialize();
}

void GameObject::AddComponent(Component* pComponent) {
	// Check if component exists before adding
	pComponent->pGO = this;
	mComponents[pComponent->Type()] = pComponent;
	//mComponents.push_back(pComponent);
}

bool GameObject::HasComponent(ComponentType type) const
{
	for (auto &comp : mComponents) {
		if (comp->Type() == type)
			return true;
	}
	return false;
}

void GameObject::HandleEvent(Event* pEvent) {
	for (auto &comp : mComponents) {
		comp->HandleEvent(pEvent);
	}
}
