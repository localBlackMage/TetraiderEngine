#pragma once

#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

class PostProcessing : public Subscriber
{
private:
	std::string m_gbVShader, m_gbHShader;
	Mesh& m_mesh;
	GLuint m_baseFBO_ID, m_gausFBO_ID;
	GLuint m_baseFBO, m_gausFBO;

public:
	PostProcessing();
	~PostProcessing() {};
	PostProcessing(const PostProcessing &) = delete;
	void operator=(const PostProcessing &) = delete;

	virtual void HandleEvent(Event * p_event) {};

	inline void SetGBShaders(std::string gbVShader, std::string gbHShader) {
		m_gbVShader = gbVShader;
		m_gbHShader = gbHShader;
	}

	void InitFBOs();
};

#endif