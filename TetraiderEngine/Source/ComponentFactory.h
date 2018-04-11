#pragma once
#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

class ComponentFactory {
public:
	ComponentFactory();
	~ComponentFactory() {};

	Component* CreateComponent(std::string component);
private:
	typedef Component* CreationFunction();
	std::unordered_map <std::string, CreationFunction*> m_creationFunctions;
};

#endif 