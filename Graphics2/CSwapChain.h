#pragma once
#include "Header.h"
#if defined(DIRECTX)
class CSwapChain
{
public:
	CSwapChain();
	~CSwapChain();

	IDXGISwapChain*                     m_pSwapChain = NULL;
	ID3D11Texture2D*                    m_pBackBuffer = NULL;

};

#elif defined(OPENGL)
#endif