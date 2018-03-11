#pragma once

class Canvas;
enum class CanvasType
{
	CANVAS_PAUSE,
	CANVAS_OPTION,
	CANVAS_CREDITS,
	
	CANVAS_NUM
};

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

private:
	Canvas* m_Canvas[(int)CanvasType::CANVAS_NUM];
	
};

