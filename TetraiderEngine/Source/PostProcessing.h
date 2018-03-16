#pragma once

#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

class PostProcessing : public Subscriber
{
private:
	ShaderProgram * m_fboShader;
	ShaderProgram * m_gausShader;
	Mesh& m_mesh;

	FrameBufferObject* m_pBaseFBO;
	FrameBufferObject* m_pGausFBO;

	bool m_enabled;

	void _Start();
	void _End();
public:
	friend class RenderManager;

	PostProcessing();
	~PostProcessing();
	PostProcessing(const PostProcessing &) = delete;
	void operator=(const PostProcessing &) = delete;

	virtual void HandleEvent(Event * p_event) {};

	inline Mesh& GetMesh() const { return m_mesh; }

	inline void SetFBOShader(ShaderProgram* fboShader) { m_fboShader = fboShader; }
	inline void EnableFBOShader() const { glUseProgram(m_fboShader->GetProgramID()); }
	inline void SetGBShaders(ShaderProgram* gbShader) { m_gausShader = gbShader; }
	inline void EnableGBShader() const { glUseProgram(m_gausShader->GetProgramID()); }

	bool IsEnabled() const { return m_enabled; }
	inline void Enable() { m_enabled = true; }
	inline void Disable() { m_enabled = false; }
	inline void Toggle() { m_enabled = !m_enabled; };

	void InitFBOs();
	void ClearBaseFBO();
	void BindBaseFBO();
	void UnbindBaseFBO();

	void ClearGausFBO();
	void BindGausFBO();
	void UnbindGausFBO();

	void DoPostProcessing();

	//GLuint GetBaseFBOTexture() const { return m_pBaseFBO->Buffer(); }

	//FrameBufferObject* BaseFBO() const { return m_pBaseFBO; }
};

#endif