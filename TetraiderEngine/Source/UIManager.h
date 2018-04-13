#pragma once
#include "CanvasTypeName.h"
#include "Canvas.h"

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
	void DeActivateAllCanvas();
	bool IsCanvasAvailable(const CanvasType);
	void HandleEvent(Event* pEvent);
	void AddGameObjectToCanvas(CanvasType type, GameObject* _pGO);
	bool m_ignoreButtonsForOneFrame;
private:
	Canvas* m_Canvas[(int)CanvasType::CANVAS_NUM];
    int m_count;
};

