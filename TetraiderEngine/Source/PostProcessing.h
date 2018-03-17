#pragma once

#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

struct ImageRenderersData {
	ShaderProgram * pBaseShader;
	ShaderProgram * pGausHShader;
	ShaderProgram * pGausVShader;
};

class PostProcessing : public Subscriber
{
private:
	ShaderProgram * m_pBaseShader;
	//ShaderProgram * m_gausHShader;
	//ShaderProgram * m_gausVShader;
	Mesh& m_mesh;

	//FrameBufferObject* m_pBaseFBO;

	ImageRenderer * m_pBaseIR;
	ImageRenderer * m_pGaussianHIR;
	ImageRenderer * m_pGaussianVIR;


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
	void InitImageRenderers(ImageRenderersData metadata);

	inline void SetBaseShader(ShaderProgram* fboShader) { m_pBaseShader = fboShader; }
	inline void EnableBaseShader() const { glUseProgram(m_pBaseShader->GetProgramID()); }

	//inline void SetGBShaders(ShaderProgram* gbHShader, ShaderProgram* gbVShader) { 
	//	m_gausHShader = gbHShader; 
	//	m_gausVShader = gbVShader;
	//}
	//inline void EnableGBHShader() const { glUseProgram(m_gausHShader->GetProgramID()); }
	//inline void EnableGBVShader() const { glUseProgram(m_gausVShader->GetProgramID()); }

	bool IsEnabled() const { return m_enabled; }
	inline void Enable() { m_enabled = true; }
	inline void Disable() { m_enabled = false; }
	inline void Toggle() { m_enabled = !m_enabled; };

	//void InitFBOs();

	void RenderBaseFBO() const;
	void ClearBaseFBO(const Vector3D& color = Vector3D());
	void BindBaseFBO();
	void UnbindBaseFBO();

	/*
		Does Post Processing work to 
	*/
	void DoPostProcessing();
};

#endif