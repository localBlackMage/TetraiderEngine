#pragma once

#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

class PostProcessing : public Subscriber
{
private:
public:
	PostProcessing() {};
	~PostProcessing() {};
	PostProcessing(const PostProcessing &) = delete;
	void operator=(const PostProcessing &) = delete;

	virtual void HandleEvent(Event * p_event) {};
};

#endif