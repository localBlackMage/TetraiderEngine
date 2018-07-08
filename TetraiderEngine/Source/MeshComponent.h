/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

class MeshComponent : public Component 
{
protected:	
	std::shared_ptr<Mesh> m_pMesh;
	std::shared_ptr<Scene> m_pScene;
	std::string m_shader;


public:
	MeshComponent();
	~MeshComponent();
	static Component* CreateInstance() { return new MeshComponent(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	void Override(const json& j);

	inline const std::shared_ptr<Mesh> GetMesh() const { return m_pMesh; }
	inline void SetMesh(std::shared_ptr<Mesh> mesh) { m_pMesh = mesh; };

	inline const std::shared_ptr<Scene> GetScene() const { return m_pScene; }
	inline void SetScene(std::shared_ptr<Scene> scene) { m_pScene = scene; };

	inline std::string Shader() const { return m_shader; };
};

#endif

