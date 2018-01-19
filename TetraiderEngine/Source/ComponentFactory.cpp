#include "ComponentFactory.h"
#include "Transform.h"
#include "Sprite.h"

ComponentFactory::ComponentFactory() { }

Component* ComponentFactory::CreateComponent(std::string component) {
	if (component == "Transform") return new Transform();
	else if (component == "Sprite") return new Sprite();

	return 0;
}