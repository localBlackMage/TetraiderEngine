#pragma once

#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

class PostProcessing : public Subscriber
{
private:
	std::string m_gbVShader, m_gbHShader;
	Mesh& m_mesh;
	FrameBufferObject* m_pBaseFBO;
	FrameBufferObject* m_pGausFBO;

public:
	PostProcessing();
	~PostProcessing();
	PostProcessing(const PostProcessing &) = delete;
	void operator=(const PostProcessing &) = delete;

	virtual void HandleEvent(Event * p_event) {};

	inline Mesh& GetMesh() const { return m_mesh; }
	inline void SetGBShaders(std::string gbVShader, std::string gbHShader) {
		m_gbVShader = gbVShader;
		m_gbHShader = gbHShader;
	}

	void InitFBOs();
	void BindBaseFBO();
	void BindGuasFBO();

	GLuint GetBaseFBOTexture() const { return m_pBaseFBO->Buffer(); }
};

#endif