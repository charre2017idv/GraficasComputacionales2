#pragma once
#include "Header.h"
#if defined(DIRECTX)
class CVertexShader
{
public:
	CVertexShader();
	~CVertexShader();
public:
	ID3D11VertexShader*                 m_pVertexShader = NULL;

};
#elif defined(OPENGL)
#endif