#pragma once
#include "CanvasTypeName.h"
class Canvas;


class UIManager : public Subscriber
{
public:
	UIManager();
	~UIManager();
	UIManager(const UIManager&) = delete;
	void operator=(const UIManager &) = delete;
	void ActivateCanvas(const CanvasType);
	void DeactivateCanvas(const CanvasType);
	void RegisterCanvas(Canvas *);
	void DeregisterCanvas(Canvas *);
	void AddGameObjectToCanvas(CanvasType type, GameObject* _pGO);

private:
	Canvas* m_Canvas[(int)CanvasType::CANVAS_NUM];
	
};

