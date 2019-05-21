#pragma once
#include "Header.h"
#if defined(DIRECTX)
class CLayout
{
public:
	CLayout();
	~CLayout();
public:
	ID3D11InputLayout*                  m_pVertexLayout = NULL;

};
#elif defined(OPENGL)
#endif