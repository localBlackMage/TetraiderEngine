/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

class HeightMap : public Component {
protected:
	float m_maxHeight;
	SurfaceTextureBuffer * m_pHeightMap;

public:
	HeightMap();
	~HeightMap();

	static Component* CreateInstance() { return new HeightMap(); }

	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Deactivate();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);
	virtual void LateInitialize();
	virtual void HandleEvent(Event* pEvent);

	inline float MaxHeight() const { return m_maxHeight; }

	inline const GLuint GetHeightMapBuffer() const { return m_pHeightMap ? m_pHeightMap->bufferId : 0; }
	inline const SurfaceTextureBuffer * GetHeightMap() const { return m_pHeightMap; }
};

