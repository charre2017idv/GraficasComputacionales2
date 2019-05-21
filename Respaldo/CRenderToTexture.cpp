#include "CRenderToTexture.h"


#if defined (DIRECTX)
CRenderToTexture::CRenderToTexture()
{
	m_renderTargetTexture = 0;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;
}

CRenderToTexture::~CRenderToTexture()
{
	Release();
}

void CRenderToTexture::Release()
{
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_renderTargetTexture)
	{
		m_renderTargetTexture->Release();
		m_renderTargetTexture = nullptr;
	}
}

bool CRenderToTexture::Initialize(ID3D11Device* pd3dDevice)
{
	HRESULT hr;

	/* Initialize the render target texture description */
	ZeroMemory(&m_textureDesc, sizeof(m_textureDesc));

	/* Setup the render target texture description */
	m_textureDesc.Width = SCREEN_WIDTH;
	m_textureDesc.Height = SCREEN_HEIGHT;
	m_textureDesc.MipLevels = 1;
	m_textureDesc.ArraySize = 1;
	m_textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_textureDesc.SampleDesc.Count = 1;
	m_textureDesc.Usage = D3D11_USAGE_DEFAULT;
	m_textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_textureDesc.CPUAccessFlags = 0;
	m_textureDesc.MiscFlags = 0;

	/* Create the render target texture */
	hr = pd3dDevice->CreateTexture2D(&m_textureDesc, NULL, &m_renderTargetTexture);

	if (FAILED(hr))
		return false;

	/* Setup the description of the render target view */
	m_renderTargetViewDesc.Format = m_textureDesc.Format;
	m_renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	m_renderTargetViewDesc.Texture2D.MipSlice = 0;

	/* Create the render target view */
	hr = pd3dDevice->CreateRenderTargetView(m_renderTargetTexture, &m_renderTargetViewDesc, &m_renderTargetView);

	if (FAILED(hr))
		return false;

	/* Setup the description of the shader resource view */
	m_shaderResourceViewDesc.Format = m_textureDesc.Format;
	m_shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	m_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	m_shaderResourceViewDesc.Texture2D.MipLevels = 1;

	/* Create the shader resource view */
	hr = pd3dDevice->CreateShaderResourceView(m_renderTargetTexture, &m_shaderResourceViewDesc, &m_shaderResourceView);
	if (FAILED(hr))
		return false;

	return true;
}

void CRenderToTexture::SetRenderTarget(ID3D11DeviceContext* pImmediateContext)
{
	pImmediateContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
}

void CRenderToTexture::ClearRenderTarget(ID3D11DeviceContext* pImmediateContext)
{
	pImmediateContext->ClearRenderTargetView(m_renderTargetView, ClearColor);
}
#endif

#if defined( OPENGL)
CRenderToTexture::CRenderToTexture()
{
}

CRenderToTexture::~CRenderToTexture()
{
}


bool CRenderToTexture::Initialize()
{

	glGenFramebuffers(1, &m_frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &m_depthId);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthId);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthId);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureId, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	return true;
}

void CRenderToTexture::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void CRenderToTexture::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CRenderToTexture::Clear()
{
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


#endif