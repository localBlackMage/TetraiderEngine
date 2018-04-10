#pragma once

#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

struct ImageRenderersData {
	ShaderProgram * pBaseShader;
	ShaderProgram * pGausHShader;
	ShaderProgram * pGausVShader;
	ShaderProgram * pMiniMapShader;
	ShaderProgram * pMaskShader;
};

class PostProcessing : public Subscriber
{
private:
	ShaderProgram * m_pBaseShader;
	Mesh& m_mesh;

	ImageRenderer * m_pBaseIR;
	ImageRenderer * m_pSecondBaseIR;
	ImageRenderer * m_pGaussianHIR;
	ImageRenderer * m_pGaussianVIR;
	ImageRenderer * m_pMiniMapMaskIR;
	ImageRenderer * m_pMiniMapOriginalIR;
	ImageRenderer * m_pMiniMapFinalIR;

	bool m_enabled;

	void _Start();
	void _End();

	void _RenderMiniMap();
public:
	friend class RenderManager;

	PostProcessing();
	~PostProcessing();
	PostProcessing(const PostProcessing &) = delete;
	void operator=(const PostProcessing &) = delete;

	virtual void HandleEvent(Event * p_event);

	inline Mesh& GetMesh() const { return m_mesh; }
	void DebugInitialize();
	void InitImageRenderers(const ImageRenderersData& metadata, const Resolution& resolution);

	inline void SetBaseShader(ShaderProgram* fboShader) { m_pBaseShader = fboShader; }
	inline void EnableBaseShader() const { glUseProgram(m_pBaseShader->GetProgramID()); }

	bool IsEnabled() const { return m_enabled; }
	inline void Enable() { m_enabled = true; }
	inline void Disable() { m_enabled = false; }
	inline void Toggle() { m_enabled = !m_enabled; };
	
	void RenderBaseFBO() const;
	void ClearBaseFBO(const Vector3D& color = Vector3D(0,0,0,0));
	void BindBaseFBO();
	void UnbindBaseFBO();

	/*
		Does Post Processing work to Second Base FBO
	*/
	void DoPostProcessing();

	void CreateMiniMapTexture(const std::vector<RoomNodeData>& roomNodeData, unsigned short rows, unsigned short cols);
};

#endif