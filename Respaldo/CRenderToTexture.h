#pragma once
#include "Header.h"
//--------------------------------------------------------------------------------------
// File: CRenderToTexture.h
// Version: v 10.0
// Creation Date: 08/04/2019
// Author: Roberto Charreton Kaplun
// Summary: This class will allow to pass the render to a texture
// Errors: 
// Bugs: 
// Nice to Have:  
//--------------------------------------------------------------------------------------

class CRenderToTexture
{
	
#if defined(DIRECTX)
public:
	CRenderToTexture();
	~CRenderToTexture();
public:
	ID3D11Texture2D*								m_renderTargetTexture;
	D3D11_TEXTURE2D_DESC							m_textureDesc;

	ID3D11RenderTargetView*							m_renderTargetView;
	D3D11_RENDER_TARGET_VIEW_DESC					m_renderTargetViewDesc;

	ID3D11ShaderResourceView*						m_shaderResourceView;
	D3D11_SHADER_RESOURCE_VIEW_DESC					m_shaderResourceViewDesc;

	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // red, green, blue, alpha
public:
	bool Initialize(ID3D11Device* pd3dDevice);

	void SetRenderTarget(ID3D11DeviceContext* pImmediateContext);
	void ClearRenderTarget(ID3D11DeviceContext* pImmediateContext);

	void Release();


#elif defined (OPENGL)
public:
	CRenderToTexture();
	~CRenderToTexture();
public:
	GLuint m_frameBufferId;
	GLuint m_textureId;
	GLuint m_depthId;

	bool Initialize();
	void Clear();
	void Bind();
	void Unbind();
public:
#endif


};