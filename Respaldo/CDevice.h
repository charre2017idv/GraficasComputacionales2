#pragma once
#include "Header.h"
#if defined(DIRECTX)
class CDevice
{
public:
	CDevice();
	~CDevice();
public:
	ID3D11Device*                       m_pd3dDevice = NULL;
	ID3D11Texture2D*                    m_pBackBuffer = NULL;

	UINT createDeviceFlags = 0;
	UINT width;
	UINT height;

	D3D_DRIVER_TYPE driverTypes[3] ={
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	D3D_FEATURE_LEVEL featureLevels[3] ={
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	
	UINT numDriverTypes;
	UINT numFeatureLevels;

	DXGI_SWAP_CHAIN_DESC sd;
	D3D11_TEXTURE2D_DESC descDepth;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	D3D11_VIEWPORT vp;
	ID3DBlob* pVSBlob = NULL;

	D3D11_INPUT_ELEMENT_DESC layout[3] ={
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements;


	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT3 Nor;

	};

	struct CBNeverChanges
	{
		XMMATRIX mView;
	};

	struct CBChangeOnResize
	{
		XMMATRIX mProjection;
	};

	struct CBChangesEveryFrame
	{
		XMMATRIX mWorld;
		XMFLOAT4 vMeshColor;
		XMFLOAT4 ligthDir;
		XMFLOAT4 SpecularPower;
		XMFLOAT4 SpecularColor;
		XMFLOAT4 difuseColor;
		XMFLOAT4 vViewPosition;
		//XMFLOAT4 kDiffuse;
		//XMFLOAT4 kAmbient;
		//XMFLOAT4 kSpecular;
	};

	D3D11_SAMPLER_DESC sampDesc;

};

#elif defined(OPENGL)
class CDevice
{
public:
	CDevice();
	~CDevice();
public:
	const aiScene * Model;
};
#endif