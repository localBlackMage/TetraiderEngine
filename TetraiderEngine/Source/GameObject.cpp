#include "GameObject.h"
#include "Component.h"
#include "Event.h"

GameObject::GameObject(unsigned int id) : 
	m_id(id), 
	m_isDestroy(false), m_isActive(true), 
	m_isCollisionDisabled(false), 
	m_isRender(true) {}

GameObject::~GameObject() {
	for (auto &comp : mComponents)
		delete comp;

	mComponents.clear();
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
	mComponents.push_back(pComponent);
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
