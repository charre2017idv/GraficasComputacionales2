#pragma once
#include "Header.h"
#if defined(DIRECTX)
class CPixelShader
{
public:
	CPixelShader();
	~CPixelShader();
	ID3D11PixelShader*                  m_pPixelShader = NULL;

};

#elif defined(OPENGL)
#endif