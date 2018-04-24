/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

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