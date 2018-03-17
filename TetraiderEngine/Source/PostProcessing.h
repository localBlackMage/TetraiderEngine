#pragma once

#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

class PostProcessing : public Subscriber
{
private:
	ShaderProgram * m_fboShader;
	ShaderProgram * m_gausHShader;
	ShaderProgram * m_gausVShader;
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
	inline void SetGBShaders(ShaderProgram* gbHShader, ShaderProgram* gbVShader) { 
		m_gausHShader = gbHShader; 
		m_gausVShader = gbVShader;
	}
	inline void EnableGBHShader() const { glUseProgram(m_gausHShader->GetProgramID()); }
	inline void EnableGBVShader() const { glUseProgram(m_gausVShader->GetProgramID()); }

	bool IsEnabled() const { return m_enabled; }
	inline void Enable() { m_enabled = true; }
	inline void Disable() { m_enabled = false; }
	inline void Toggle() { m_enabled = !m_enabled; };

	void InitFBOs();
	void ClearBaseFBO(const Vector3D& color = Vector3D());
	void BindBaseFBO();
	void UnbindBaseFBO();

	void ClearGausFBO(const Vector3D& color = Vector3D());
	void BindGausFBO();
	void UnbindGausFBO();

	void DoPostProcessing();
};

#endif