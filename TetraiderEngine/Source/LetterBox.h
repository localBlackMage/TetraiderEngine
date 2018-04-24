/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef LETTERBOX_H
#define LETTERBOX_H

enum AnchorSide {
	A_LEFT = 0,
	A_RIGHT,
	A_TOP,
	A_BOTTOM
};

class LetterBox : public Component {
protected:
	Transform* m_pTransform;
	AnchorSide m_anchor;
	float m_scalePerResolution[MAX_RESOLUTIONS];
	float m_posPerResolution[MAX_RESOLUTIONS];

	void _SetPosAndScale();
public:
	LetterBox();
	~LetterBox() {};

	static Component* CreateInstance() { return new LetterBox(); }

	virtual void Update(float dt) {};
	virtual void LateUpdate(float dt) {};
	virtual void Deactivate();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j) {};
	virtual void LateInitialize();
	virtual void HandleEvent(Event* pEvent);
};

#endif

