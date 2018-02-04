#include "ComponentFactory.h"
#include "Component.h"
#include "Transform.h"
#include "Sprite.h"
#include "Animation.h"
#include "Controller.h"
#include "Body.h"
#include "Camera.h"
#include "CamFollow.h"
#include "Health.h"
#include "FlashOnTakeDamage.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Audio.h"

ComponentFactory::ComponentFactory() {
	m_componentFunctions["Transform"] = Transform::CreateInstance;
	m_componentFunctions["Sprite"] = Sprite::CreateInstance;
	m_componentFunctions["Animation"] = Animation::CreateInstance;
	m_componentFunctions["Controller"] = Controller::CreateInstance;
	m_componentFunctions["Body"] = Body::CreateInstance;
	m_componentFunctions["Camera"] = Camera::CreateInstance;
	m_componentFunctions["CamFollow"] = CamFollow::CreateInstance;
	m_componentFunctions["Health"] = Health::CreateInstance;
	m_componentFunctions["FlashOnTakeDamage"] = FlashOnTakeDamage::CreateInstance;
	m_componentFunctions["Projectile"] = Projectile::CreateInstance;
	m_componentFunctions["Weapon"] = Weapon::CreateInstance;
	m_componentFunctions["Audio"] = Audio::CreateInstance;
}

Component* ComponentFactory::CreateComponent(std::string component) {
	CreationFunction* Create = m_componentFunctions[component];
	if (Create)
		return Create();

	return NULL;
}