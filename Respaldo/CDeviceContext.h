#pragma once
#include "Header.h"

#if defined(DIRECTX)
class CDeviceContext
{
public:
	CDeviceContext();
	~CDeviceContext();
public:
	D3D_DRIVER_TYPE                     m_driverType = D3D_DRIVER_TYPE_NULL;
	ID3D11DeviceContext*                m_pImmediateContext = NULL;

};

#elif defined(OPENGL)

#endif