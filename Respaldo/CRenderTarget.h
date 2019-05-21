#pragma once
#include "Header.h"
#if defined(DIRECTX)
class CRenderTarget
{
public:
	CRenderTarget();
	~CRenderTarget();
public:

	ID3D11RenderTargetView*             m_pRenderTargetView = NULL;
	ID3D11DepthStencilView*             m_pDepthStencilView = NULL;
	ID3D11Texture2D*                    m_pBackBuffer = NULL;

};

#elif defined(OPENGL)
#endif