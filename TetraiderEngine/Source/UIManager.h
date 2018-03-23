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
	Canvas* GetCanvasPause() { return m_Pause; };
	void HandleEvent(Event* pEvent);
	void AddGameObjectToCanvas(CanvasType type, GameObject* _pGO);

private:
	Canvas* m_Canvas[(int)CanvasType::CANVAS_NUM];
	Canvas* m_Pause;
    int m_count;
};

