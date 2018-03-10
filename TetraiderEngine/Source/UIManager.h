#pragma once

class Canvas;
class UIManager : public Subscriber
{
public:
	UIManager();
	~UIManager();
	UIManager(const UIManager&) = delete;
	void operator=(const UIManager &) = delete;
	void ActivateCanvas(int);
	void DeactivateCanvas(int);
	void RegisterCanvas(Canvas *);
	void DeregisterCanvas();
	
	enum CanvasType
	{
		CANVAS_PAUSE,
		CANVAS_OPTION,
		CANVAS_CREDITS
	};
private:
	std::vector<Canvas*> m_Canvas;
	
};

