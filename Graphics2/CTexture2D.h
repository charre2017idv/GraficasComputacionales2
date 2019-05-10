#pragma once
#include "Header.h"
#if defined(DIRECTX)
class CTexture2D
{
public:
	CTexture2D();
	~CTexture2D();

	ID3D11Texture2D*                    m_pDepthStencil = NULL;
	ID3D11ShaderResourceView*           m_pTextureRV = NULL;
	ID3D11ShaderResourceView*           m_pTextureRV2 = NULL;
	ID3D11SamplerState*                 m_pSamplerLinear = NULL;
};

#elif defined(OPENGL)
#endif