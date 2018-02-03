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

ComponentFactory::ComponentFactory() { }

Component* ComponentFactory::CreateComponent(std::string component) {
	if (component == "Transform") return new Transform();
	else if (component == "Sprite") return new Sprite();
	else if (component == "Animation") return new Animation();
	else if (component == "Controller") return new Controller();
	else if (component == "Body") return new Body();
	else if (component == "Camera") return new Camera();
	else if (component == "CamFollow") return new CamFollow();
	else if (component == "Health") return new Health();
	else if (component == "FlashOnTakeDamage") return new FlashOnTakeDamage();
	else if (component == "Projectile") return new Projectile();
	else if (component == "Weapon") return new Weapon();
	else if (component == "Audio") return new Audio();

	return 0;
}