#include "ComponentFactory.h"
#include "Transform.h"

ComponentFactory::ComponentFactory() { }

Component* ComponentFactory::CreateComponent(std::string component) {
	if (component == "Transform") return new Transform();

	return 0;
}