#include "CManager.h"


#if defined(DIRECTX)

CManager::CManager()
{
}


CManager::~CManager()
{
}

// InitDevice
void CManager::SwapChain(const HWND& hWnd)
{
	//DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&Dev.sd, sizeof(Dev.sd));
	Dev.sd.BufferCount = 1;
	Dev.sd.BufferDesc.Width =  Dev.width;
	Dev.sd.BufferDesc.Height = Dev.height;
	Dev.sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Dev.sd.BufferDesc.RefreshRate.Numerator = 60;
	Dev.sd.BufferDesc.RefreshRate.Denominator = 1;
	Dev.sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	Dev.sd.OutputWindow = hWnd;
	Dev.sd.SampleDesc.Count = 1;
	Dev.sd.SampleDesc.Quality = 0;
	Dev.sd.Windowed = TRUE;
}

bool CManager::createRenderTargetView(HRESULT &hr, ID3D11RenderTargetView* &mRenderTargetView)
{
	// Create a render target view
	//ID3D11Texture2D* pBackBuffer = NULL;
	hr = SwCh.m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&Dev.m_pBackBuffer);
	if (FAILED(hr))
		return false;

	//hr = Dev.m_pd3dDevice->CreateRenderTargetView(Dev.m_pBackBuffer, NULL, &RenTarg.m_pRenderTargetView);
	hr = Dev.m_pd3dDevice->CreateRenderTargetView(Dev.m_pBackBuffer, NULL, &RenTarg.m_pRenderTargetView);
	Dev.m_pBackBuffer->Release();
	if (FAILED(hr))
		return false;
}

void CManager::createDepthStencilTexture(HRESULT &hr)
{

	// Create depth stencil texture
	//D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&Dev.descDepth, sizeof(Dev.descDepth));
	Dev.descDepth.Width =  Dev.width;
	Dev.descDepth.Height = Dev.height;
	Dev.descDepth.MipLevels = 1;
	Dev.descDepth.ArraySize = 1;
	Dev.descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Dev.descDepth.SampleDesc.Count = 1;
	Dev.descDepth.SampleDesc.Quality = 0;
	Dev.descDepth.Usage = D3D11_USAGE_DEFAULT;
	Dev.descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	Dev.descDepth.CPUAccessFlags = 0;
	Dev.descDepth.MiscFlags = 0;
	hr = Dev.m_pd3dDevice->CreateTexture2D(&Dev.descDepth, NULL, &Tex2D.m_pDepthStencil);
}

void CManager::setUpViewport()
{
	
	// Setup the viewport
	Dev.vp.Width  =  (FLOAT)Dev.width;
	Dev.vp.Height = (FLOAT) Dev.height;
	Dev.vp.MinDepth = 0.0f;
	Dev.vp.MaxDepth = 1.0f;
	Dev.vp.TopLeftX = 0;
	Dev.vp.TopLeftY = 0;
	DevContext.m_pImmediateContext->RSSetViewports(1, &Dev.vp);
	Camera.createProjection(XM_PIDIV4, Dev.width, Dev.height, 0.01f, 100.0f);
}

void CManager::createDepthStecilView(HRESULT &hr)
{
	// Create the depth stencil view
	//D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&Dev.descDSV, sizeof(Dev.descDSV));
	Dev.descDSV.Format = Dev.descDepth.Format;
	Dev.descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	Dev.descDSV.Texture2D.MipSlice = 0;
	hr = Dev.m_pd3dDevice->CreateDepthStencilView(Tex2D.m_pDepthStencil, &Dev.descDSV, &RenTarg.m_pDepthStencilView);
	//hr = Dev.m_pd3dDevice->CreateDepthStencilView(Tex2D.m_pDepthStencil, &Dev.descDSV, &RenderTargetID.m_pDepthStencilView);
}

HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void CManager::createTheSampleState(HRESULT &hr)
{
	ZeroMemory(&Dev.sampDesc, sizeof(Dev.sampDesc));
	Dev.sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	Dev.sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Dev.sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Dev.sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Dev.sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Dev.sampDesc.MinLOD = 0;
	Dev.sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = Dev.m_pd3dDevice->CreateSamplerState(&Dev.sampDesc, &Tex2D.m_pSamplerLinear);
}

void CManager::Clear()
{
	// Clear the back buffer
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // red, green, blue, alpha
	//float ClearColor[4] = { 0.0f, 0.0f, 255.0f, 1.0f }; // red, green, blue, alpha
   // float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	DevContext.m_pImmediateContext->ClearRenderTargetView(RenTarg.m_pRenderTargetView, ClearColor);
	// Clear the depth buffer to 1.0 (max depth)
	DevContext.m_pImmediateContext->ClearDepthStencilView(RenTarg.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void CManager::InitImGuiWindow()
{
	// Init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(Dev.m_pd3dDevice, DevContext.m_pImmediateContext);
	ImGui::StyleColorsDark();
}

void CManager::SetImGui()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	// Create ImGui Test Window
	//ImGui::Begin("Test");
	float amount = Amount_Meshes;
	float param = m_Param;
	ImGui::Begin("Model Data", 0);
	ImGui::SetWindowSize(ImVec2(200, 200), ImGuiSetCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2(2, 2), ImGuiSetCond_FirstUseEver);
	ImGui::Text("Application average");
	ImGui::Text("%.0f ms/frame (%.1f FPS)", 1000.0 / double(ImGui::GetIO().Framerate), double(ImGui::GetIO().Framerate));
	ImGui::Text("Mesh Count: %.0f", amount);
	ImGui::Text("Inputs: %.0f", param);
	//ImGui::End();
	//ImGui::ShowTestWindow();

	ImGui::End();

	// Assamble together to draw data
	ImGui::Render();

	// Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
// Init Cam
void CManager::initCam()
{
	Eye = { 0.0f, 3.0f, -10.0f, };
	At = { 0.0f, 1.0f, 0.0f, };
	Up = { 0.0f, 1.0f, 0.0f, };
	Camera.VEye = Eye;
	Camera.VAt = At;
	Camera.VUp = Up;

	NEye.x = Eye.x;
	NEye.y = Eye.y;
	NEye.z = Eye.z;
	
	Camera.setCameraLookAtLH(
		Eye.x, Eye.y, Eye.z, 0.0f, 
		At.x, At.y, At.z, 0.0f,
		Up.x, Up.y, Up.z, 0.0f
	);
	Camera.createView();
	Camera.createProjection(XM_PIDIV4, Dev.width, Dev.height, 0.01f, 100.0f);

	DevContext.m_pImmediateContext->UpdateSubresource(ChangeOnResizeBuffer.m_Buffer, 0, NULL, &Camera.cbChangesOnResize, 0, 0);

	// Init Camera 2 (Secure Camera)
	Eye = { 0.0f, 3.0f, -10.0f, };
	At = { 0.0f, 1.0f, 0.0f, };
	Up = { 0.0f, 1.0f, 0.0f, };
	Camera2.setCameraLookAtLH(
		Eye.x, Eye.y, Eye.z, 0.0f,
		At.x, At.y, At.z, 0.0f,
		Up.x, Up.y, Up.z, 0.0f
	);
	Camera2.createView();
	Camera2.createProjection(XM_PIDIV4, Dev.width, Dev.height, 0.01f, 100.0f);

	DevContext.m_pImmediateContext->UpdateSubresource(ChangeOnResizeBuffer.m_Buffer, 0, NULL, &Camera2.cbChangesOnResize, 0, 0);


	onInitpepito = true;
	isDeviceInit = true;
	
}

void CManager::initCam2()
{
	Camera.setCameraLookAtLH(
		0.0f,  6.0f, 1.0f, 0.0f,
		0.0f, -2.0f, 0.0f, 0.0f,
		0.0f,  1.0f, 0.0f, 0.0f
	);
	Camera.createView();
	Camera.createProjection(XM_PIDIV4, Dev.width, Dev.height, 0.01f, 100.0f);

	DevContext.m_pImmediateContext->UpdateSubresource(ChangeOnResizeBuffer.m_Buffer, 0, NULL, &Camera.cbChangesOnResize, 0, 0);
	Camera.isClicked = true;
}
// Camera Change
void CManager::camChange()
{
	camChanges++;
	if (camChanges > 1)
	{
		camChanges = 0;
	}
	if (camChanges == 0)
	{
		//////////////////////////////////////////////////////////////////////////
		// Front Cam
		//////////////////////////////////////////////////////////////////////////
		Eye = { 0.0f, 3.0f, -10.0f, };
		At = { 0.0f, 1.0f, 0.0f, };
		Up = { 0.0f, 1.0f, 0.0f, };
		Camera.VEye = Eye;
		Camera.VAt = At;
		Camera.VUp = Up;
		
		Camera.setCameraLookAtLH(
			Eye.x, Eye.y, Eye.z, 0.0f,
			At.x, At.y, At.z, 0.0f,
			Up.x, Up.y, Up.z, 0.0f
		);
		Camera.g_View = XMMatrixLookAtLH(Camera.Eye, Camera.At, Camera.Up);
	}
	if (camChanges == 1)
	{
		//////////////////////////////////////////////////////////////////////////
		// Superior Cam
		//////////////////////////////////////////////////////////////////////////
		Eye = { 0.0f, 10.0f, 0.0f, };
		At =  { 0.0f, 0.0f,  1.0f, };
		Up =  { 0.0f, 1.0f,  0.0f };
		Camera.VEye = Eye;
		Camera.VAt = At;
		Camera.VUp = Up;
		Camera.setCameraLookAtLH(
			Eye.x, Eye.y, Eye.z, 0.0f,
			At.x, At.y, At.z, 0.0f,
			Up.x, Up.y, Up.z, 0.0f
		);
		
		Camera.g_View = XMMatrixLookAtLH(Camera.Eye, Camera.At, Camera.Up);
	}
}

// Move Camera
void CManager::cameraMove(ID3D11DeviceContext* &pImmediateContext, ID3D11Buffer*&pCBNeverChanges, WPARAM wParam, float xPos, float yPos)
{
	if (wParam == 37) // Left
	{
		Camera.g_View *= XMMatrixTranslation(m_Cx, 0.0f, 0.0f);
		Camera.VEye.x -= m_Cx;
		//DwarfMatrix *= newView;
		//DwarfMatrix = DwarfMatrix * XMMatrixTranslation(m_Cx, 0.0f, 0.0f);
	}
	else if (wParam == 'W') // Front
	{
		Camera.g_View *= XMMatrixTranslation(0.0f, 0.0f, -m_Cz);
		//Camera.VEye.z += m_Cz;
	}
	else if (wParam == 39) // Right
	{
		Camera.g_View *= XMMatrixTranslation(-m_Cx, 0.0f, 0.0f);
		Camera.VEye.x += m_Cx;
		//DwarfMatrix *= translate;
		//DwarfMatrix *= newView;
		//DwarfMatrix = XMMatrixTranslation(-Eye.x, 0.0f,0.0f);
		//DwarfMatrix *= translateN;
	}
	else if (wParam == 'S') // Back
	{
		Camera.g_View *= XMMatrixTranslation(0.0f, 0.0f, m_Cz);
		//Camera.VEye.z -= m_Cz;
	}
	else if (wParam == 38) // Up
	{
		Camera.g_View *= XMMatrixTranslation(0.0f, -m_Cy, 0.0f);
		//Camera.VEye.y += m_Cy;

	}
	else if (wParam == 40) // Down
	{
		Camera.g_View *= XMMatrixTranslation(0.0f, m_Cy, 0.0f);
		//Camera.VEye.y -= m_Cy;
	}
	else if (wParam == 'C') // Change Texture
	{
		camChange();
	}

	Camera.cbNeverChanges.mView = XMMatrixTranspose(Camera.g_View);
	pImmediateContext->UpdateSubresource(NeverChangeBuffer.m_Buffer, 0, NULL, &Camera.cbNeverChanges, 0, 0);
}

// Rotate Camera
void CManager::camRotation(ID3D11DeviceContext* &pImmediateContext, ID3D11Buffer*&pCBNeverChanges)
{
	GetCursorPos(&Camera.ActualPos);
	Camera.isClicked = true;

	if (Camera.LastPos.x < Camera.ActualPos.x)
	{
		Camera.isClicked = true;
		Camera.g_View *= XMMatrixRotationY(.01);
	}
	else if (Camera.LastPos.x > Camera.ActualPos.x)
	{
		Camera.g_View *= XMMatrixRotationY(-.01);
	}
	else if (Camera.LastPos.y < Camera.ActualPos.y)
	{
		Camera.g_View *= XMMatrixRotationX(.01);
	}
	else if (Camera.LastPos.y > Camera.ActualPos.y)
	{
		Camera.g_View *= XMMatrixRotationX(-.01);
	}
	Camera.LastPos = Camera.ActualPos;

	Camera.cbNeverChanges.mView = XMMatrixTranspose(Camera.g_View);
	pImmediateContext->UpdateSubresource(NeverChangeBuffer.m_Buffer, 0, NULL, &Camera.cbNeverChanges, 0, 0);
}


// Resize Camera
void CManager::resizeCam(float width, float height, ID3D11DeviceContext* &pImmediateContext, ID3D11Buffer* & g_pCBChangeOnResize)
{
	if (onInitpepito)
	{
		Dev.vp.Width = (FLOAT)width;
		Dev.vp.Height = (FLOAT)height;
		Dev.vp.MinDepth = 0.0f;
		Dev.vp.MaxDepth = 1.0f;
		Dev.vp.TopLeftX = 0;
		Dev.vp.TopLeftY = 0;
		DevContext.m_pImmediateContext->RSSetViewports(1, &Dev.vp);
		Camera.createProjection(XM_PIDIV4, width, height, 0.01f, 100.0f);
		Camera.cbChangesOnResize.mProjection = XMMatrixTranspose(Camera.g_Projection);
	}
}

 // Texture Resize
void CManager::SwapChainResize(float width, float height, ID3D11DeviceContext* &pImmediateContext, ID3D11Buffer* pBChangeOnResize)
{
	if (isDeviceInit)
	{
		DevContext.m_pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

		HRESULT hr;
		// If the textures are created, free the memory to change it for a new one
		if (Tex2D.m_pDepthStencil)
			Tex2D.m_pDepthStencil->Release();

		if (RenTarg.m_pRenderTargetView)
			RenTarg.m_pRenderTargetView->Release();

		if (RenTarg.m_pDepthStencilView)
			RenTarg.m_pDepthStencilView->Release();

		// Allocate the new information for the swapchain
		Dev.descDepth.Width = width;
		Dev.descDepth.Height = height;
		Dev.sd.BufferDesc.Width = width;
		Dev.sd.BufferDesc.Height = height;
		
		SwCh.m_pSwapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0);

		// Create the texture again
		ZeroMemory(&Dev.descDepth, sizeof(Dev.descDepth));
		Dev.descDepth.Width = width;
		Dev.descDepth.Height = height;
		Dev.descDepth.MipLevels = 1;
		Dev.descDepth.ArraySize = 1;
		Dev.descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		Dev.descDepth.SampleDesc.Count = 1;
		Dev.descDepth.SampleDesc.Quality = 0;
		Dev.descDepth.Usage = D3D11_USAGE_DEFAULT;
		Dev.descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		Dev.descDepth.CPUAccessFlags = 0;
		Dev.descDepth.MiscFlags = 0;

		hr = Dev.m_pd3dDevice->CreateTexture2D(&Dev.descDepth, NULL, &Tex2D.m_pDepthStencil);
		// Create the DepthStencilView
		ZeroMemory(&Dev.descDSV, sizeof(Dev.descDSV));
		Dev.descDSV.Format = Dev.descDepth.Format;
		Dev.descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		Dev.descDSV.Texture2D.MipSlice = 0;
		hr = Dev.m_pd3dDevice->CreateDepthStencilView(Tex2D.m_pDepthStencil, &Dev.descDSV, &RenTarg.m_pDepthStencilView);

		hr = SwCh.m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&Dev.m_pBackBuffer);
		
		hr = Dev.m_pd3dDevice->CreateRenderTargetView(Dev.m_pBackBuffer, NULL, &RenTarg.m_pRenderTargetView);
		RenTarg.m_pBackBuffer = Dev.m_pBackBuffer;
		Dev.m_pBackBuffer->Release();
		// Set the render targets
		DevContext.m_pImmediateContext->OMSetRenderTargets(1, &RenTarg.m_pRenderTargetView, RenTarg.m_pDepthStencilView);
		// Set the viewport
		Dev.vp.Width = (FLOAT)width;
		Dev.vp.Height = (FLOAT)height;
		Dev.vp.MinDepth = 0.0f;
		Dev.vp.MaxDepth = 1.0f;
		Dev.vp.TopLeftX = 0.0f;
		Dev.vp.TopLeftY = 0.0f;
		
		DevContext.m_pImmediateContext->RSSetViewports(1, &Dev.vp);
	}
}

void CManager::LoadMesh(int numVertices, int numIndices, aiVector3D* VertexArray, aiVector3D* textCoord, std::vector<std::uint32_t>& IndexArray)
{
	if (m_Vertex != nullptr)
		delete[]m_Vertex;
	if (m_Index != nullptr)
		delete[]m_Index;

	m_numVertex = numVertices;
	m_numIndex = numIndices;

	m_Vertex = new SimpleVertex[m_numVertex];
	for (size_t i = 0; i < m_numVertex; i++)
	{
		m_Vertex[i].Pos.x = VertexArray[i].x;
		m_Vertex[i].Pos.y = VertexArray[i].y;
		m_Vertex[i].Pos.z = VertexArray[i].z;

		m_Vertex[i].Tex.x = textCoord[i].x;
		m_Vertex[i].Tex.y = textCoord[i].y; // <! Wanrning must change for other textures, this is specific for the squere>

	}
	m_Index = new WORD[numIndices];
	for (size_t i = 0; i < numIndices; i++)
	{
		m_Index[i] = (WORD)IndexArray[i];
	}
	m_mesh.push_back(CMesh(m_numVertex,m_numIndex,m_Vertex,m_Index));
}

void CManager::createAssimpMesh()
{
	Model = aiImportFile(TM.HomecomingMesh, aiProcessPreset_TargetRealtime_MaxQuality);
	//Model = aiImportFile("Dwarf/Dwarf/dwarf.x", aiProcessPreset_TargetRealtime_MaxQuality);
	
	for (size_t i = 0; i <Model->mNumMeshes; i++)
	{
		std::vector<std::uint32_t> mIndices;
		mIndices.reserve(Model->mMeshes[i]->mNumFaces * 3);

		for (size_t j = 0; j < Model->mMeshes[i]->mNumFaces; j++)
		{
			mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[0u]);
			mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[1u]);
			mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[2u]);
		}
		if (Model->mMeshes[i]->mTextureCoords[0] != NULL)
		{
			LoadMesh(
				Model->mMeshes[i]->mNumVertices,
				Model->mMeshes[i]->mNumFaces * 3,
				Model->mMeshes[i]->mVertices,
				Model->mMeshes[i]->mTextureCoords[0],
				mIndices
			);
		}
	}
}

void CManager::MeshGenerator()
{
	for (size_t i = 0; i < filePaths.size(); i++)
	{
		Model = aiImportFile(filePaths[i], aiProcessPreset_TargetRealtime_MaxQuality);
		//Model = aiImportFile(TM.HomecomingMesh, aiProcessPreset_TargetRealtime_MaxQuality);
		//Model = aiImportFile("Dwarf/Dwarf/dwarf.x", aiProcessPreset_TargetRealtime_MaxQuality);

		for (size_t i = 0; i < Model->mNumMeshes; i++)
		{
			std::vector<std::uint32_t> mIndices;
			mIndices.reserve(Model->mMeshes[i]->mNumFaces * 3);

			for (size_t j = 0; j < Model->mMeshes[i]->mNumFaces; j++)
			{
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[0u]);
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[1u]);
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[2u]);
			}
			if (Model->mMeshes[i]->mTextureCoords[0] != NULL)
			{
				LoadMesh(
					Model->mMeshes[i]->mNumVertices,
					Model->mMeshes[i]->mNumFaces * 3,
					Model->mMeshes[i]->mVertices,
					Model->mMeshes[i]->mTextureCoords[0],
					mIndices
				);
			}
			readTextMesh();
		}

	}
}

void CManager::readTextMesh()
{
	for (unsigned int i = 0; i < Model->mNumMeshes; i++)
	{
		const aiMaterial* modelMaterial = Model->mMaterials[Model->mMeshes[i]->mMaterialIndex];
		if (modelMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;
			if (modelMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				m_mesh[i].textName += (std::string)Path.data;
			}
		}
		m_mesh[i].loadMeshText(Dev.m_pd3dDevice);
	}
}

// Model 2
void CManager::MeshGenerator2()
{
	for (size_t i = 0; i < filePaths2.size(); i++)
	{
		Model = aiImportFile(filePaths2[i], aiProcessPreset_TargetRealtime_MaxQuality);
		//Model = aiImportFile(TM.HomecomingMesh, aiProcessPreset_TargetRealtime_MaxQuality);
		//Model = aiImportFile("Dwarf/Dwarf/dwarf.x", aiProcessPreset_TargetRealtime_MaxQuality);

		for (size_t i = 0; i < Model->mNumMeshes; i++)
		{
			std::vector<std::uint32_t> mIndices;
			mIndices.reserve(Model->mMeshes[i]->mNumFaces * 3);

			for (size_t j = 0; j < Model->mMeshes[i]->mNumFaces; j++)
			{
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[0u]);
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[1u]);
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[2u]);
			}
			if (Model->mMeshes[i]->mTextureCoords[0] != NULL)
			{
				LoadMesh2(
					Model->mMeshes[i]->mNumVertices,
					Model->mMeshes[i]->mNumFaces * 3,
					Model->mMeshes[i]->mVertices,
					Model->mMeshes[i]->mTextureCoords[0],
					mIndices
				);
			}
			readTextMesh2(i);
		}

	}
}
void CManager::readTextMesh2(int i)
{
		const aiMaterial* modelMaterial = Model->mMaterials[Model->mMeshes[i]->mMaterialIndex];
		if (modelMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;
			if (modelMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				m_mesh2[i].textName += (std::string)Path.data;
			}
		}
		m_mesh2[i].loadMeshText(Dev.m_pd3dDevice);
	
}
void CManager::LoadMesh2(int numVertices, int numIndices, aiVector3D* VertexArray, aiVector3D* textCoord, std::vector<std::uint32_t>& IndexArray)
{
	if (m_Vertex != nullptr)
		delete[]m_Vertex;
	if (m_Index != nullptr)
		delete[]m_Index;

	m_numVertex = numVertices;
	m_numIndex = numIndices;

	m_Vertex = new SimpleVertex[m_numVertex];
	for (size_t i = 0; i < m_numVertex; i++)
	{
		m_Vertex[i].Pos.x = VertexArray[i].x;
		m_Vertex[i].Pos.y = VertexArray[i].y;
		m_Vertex[i].Pos.z = VertexArray[i].z;

		m_Vertex[i].Tex.x = textCoord[i].x;
		m_Vertex[i].Tex.y = -textCoord[i].y;

	}
	m_Index = new WORD[numIndices];
	for (size_t i = 0; i < numIndices; i++)
	{
		m_Index[i] = (WORD)IndexArray[i];
	}
	m_mesh2.push_back(CMesh(m_numVertex, m_numIndex, m_Vertex, m_Index));
}

// Model 3
void CManager::MeshGenerator3()
{
	for (size_t i = 0; i < filePaths3.size(); i++)
	{
		Model = aiImportFile(filePaths3[i], aiProcessPreset_TargetRealtime_MaxQuality);
		//Model = aiImportFile(TM.HomecomingMesh, aiProcessPreset_TargetRealtime_MaxQuality);
		//Model = aiImportFile("Dwarf/Dwarf/dwarf.x", aiProcessPreset_TargetRealtime_MaxQuality);

		for (size_t i = 0; i < Model->mNumMeshes; i++)
		{
			std::vector<std::uint32_t> mIndices;
			mIndices.reserve(Model->mMeshes[i]->mNumFaces * 3);

			for (size_t j = 0; j < Model->mMeshes[i]->mNumFaces; j++)
			{
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[0u]);
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[1u]);
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[2u]);
			}
			if (Model->mMeshes[i]->mTextureCoords[0] != NULL)
			{
				LoadMesh3(
					Model->mMeshes[i]->mNumVertices,
					Model->mMeshes[i]->mNumFaces * 3,
					Model->mMeshes[i]->mVertices,
					Model->mMeshes[i]->mTextureCoords[0],
					mIndices
				);
			}
			readTextMesh3(i);
		}

	}
}
void CManager::readTextMesh3(int i)
{
	const aiMaterial* modelMaterial = Model->mMaterials[Model->mMeshes[i]->mMaterialIndex];
	if (modelMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString Path;
		if (modelMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			m_mesh3[i].textName += (std::string)Path.data;
		}
	}
	m_mesh3[i].loadMeshText3(Dev.m_pd3dDevice);

}
void CManager::LoadMesh3(int numVertices, int numIndices, aiVector3D* VertexArray, aiVector3D* textCoord, std::vector<std::uint32_t>& IndexArray)
{
	if (m_Vertex != nullptr)
		delete[]m_Vertex;
	if (m_Index != nullptr)
		delete[]m_Index;

	m_numVertex = numVertices;
	m_numIndex = numIndices;

	m_Vertex = new SimpleVertex[m_numVertex];
	for (size_t i = 0; i < m_numVertex; i++)
	{
		m_Vertex[i].Pos.x = VertexArray[i].x;
		m_Vertex[i].Pos.y = VertexArray[i].y;
		m_Vertex[i].Pos.z = VertexArray[i].z;

		m_Vertex[i].Tex.x = textCoord[i].x;
		m_Vertex[i].Tex.y = -textCoord[i].y;

	}
	m_Index = new WORD[numIndices];
	for (size_t i = 0; i < numIndices; i++)
	{
		m_Index[i] = (WORD)IndexArray[i];
	}
	m_mesh3.push_back(CMesh(m_numVertex, m_numIndex, m_Vertex, m_Index));
}

// Model 4
void CManager::MeshGenerator4()
{
	for (size_t i = 0; i < filePaths4.size(); i++)
	{
		Model = aiImportFile(filePaths4[i], aiProcessPreset_TargetRealtime_MaxQuality);
		//Model = aiImportFile(TM.HomecomingMesh, aiProcessPreset_TargetRealtime_MaxQuality);
		//Model = aiImportFile("Dwarf/Dwarf/dwarf.x", aiProcessPreset_TargetRealtime_MaxQuality);

		for (size_t i = 0; i < Model->mNumMeshes; i++)
		{
			std::vector<std::uint32_t> mIndices;
			mIndices.reserve(Model->mMeshes[i]->mNumFaces * 3);

			for (size_t j = 0; j < Model->mMeshes[i]->mNumFaces; j++)
			{
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[0u]);
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[1u]);
				mIndices.push_back(Model->mMeshes[i]->mFaces[j].mIndices[2u]);
			}
			if (Model->mMeshes[i]->mTextureCoords[0] != NULL)
			{
				LoadMesh4(
					Model->mMeshes[i]->mNumVertices,
					Model->mMeshes[i]->mNumFaces * 3,
					Model->mMeshes[i]->mVertices,
					Model->mMeshes[i]->mTextureCoords[0],
					mIndices
				);
			}
			readTextMesh4(i);
		}

	}
}
void CManager::readTextMesh4(int i)
{
	const aiMaterial* modelMaterial = Model->mMaterials[Model->mMeshes[i]->mMaterialIndex];
	if (modelMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString Path;
		if (modelMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			m_mesh4[i].textName += (std::string)Path.data;
		}
	}
	m_mesh4[i].loadMeshText2(Dev.m_pd3dDevice);

}
void CManager::LoadMesh4(int numVertices, int numIndices, aiVector3D* VertexArray, aiVector3D* textCoord, std::vector<std::uint32_t>& IndexArray)
{
	if (m_Vertex != nullptr)
		delete[]m_Vertex;
	if (m_Index != nullptr)
		delete[]m_Index;

	m_numVertex = numVertices;
	m_numIndex = numIndices;

	m_Vertex = new SimpleVertex[m_numVertex];
	for (size_t i = 0; i < m_numVertex; i++)
	{
		m_Vertex[i].Pos.x = VertexArray[i].x;
		m_Vertex[i].Pos.y = VertexArray[i].y;
		m_Vertex[i].Pos.z = VertexArray[i].z;

		m_Vertex[i].Tex.x = textCoord[i].x;
		m_Vertex[i].Tex.y = -textCoord[i].y;

	}
	m_Index = new WORD[numIndices];
	for (size_t i = 0; i < numIndices; i++)
	{
		m_Index[i] = (WORD)IndexArray[i];
	}
	m_mesh4.push_back(CMesh(m_numVertex, m_numIndex, m_Vertex, m_Index));
}



bool CManager::initDevide(HRESULT &hr, const HWND& hWnd)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	Dev.width = rc.right - rc.left;
	Dev.height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	Dev.numDriverTypes   = ARRAYSIZE( Dev.driverTypes );
	
	Dev.numFeatureLevels = ARRAYSIZE( Dev.featureLevels );

	SwapChain(hWnd);
	
	for (UINT driverTypeIndex = 0; driverTypeIndex < Dev.numDriverTypes; driverTypeIndex++)
	{
		DevContext.m_driverType = Dev.driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, DevContext.m_driverType, NULL, createDeviceFlags, Dev.featureLevels, Dev.numFeatureLevels,
			D3D11_SDK_VERSION, &Dev.sd, &SwCh.m_pSwapChain, &Dev.m_pd3dDevice, &m_featureLevel, &DevContext.m_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;
	
	// Create the Depth Stencil Texture
	createDepthStencilTexture(hr);
	if (FAILED(hr))
		return hr;

	// Create Depth Stencil View
	createDepthStecilView(hr);
	if (FAILED(hr))
		return hr;
	
	// Create Render Target View
	createRenderTargetView(hr,RenTarg.m_pRenderTargetView);
	if (FAILED(hr))
		return hr;
	// <! Important>
	//CreateRenderTexture();
	RTT.Initialize(Dev.m_pd3dDevice);

	DevContext.m_pImmediateContext->OMSetRenderTargets( 1, &RenTarg.m_pRenderTargetView, RenTarg.m_pDepthStencilView );

	// New render target 
	setUpViewport();
	//DevContext.m_pImmediateContext->RSSetViewports( 1, &Dev.vp );

	// Compile the vertex shader
	hr = CompileShaderFromFile(L"Tutorial07.fx", "VS", "vs_4_0", &Dev.pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = Dev.m_pd3dDevice->CreateVertexShader(Dev.pVSBlob->GetBufferPointer(), Dev.pVSBlob->GetBufferSize(), NULL, &VShader.m_pVertexShader);
	if (FAILED(hr))
	{
		Dev.pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	Dev.numElements = ARRAYSIZE( Dev.layout );

	// Create the input layout
	hr = Dev.m_pd3dDevice->CreateInputLayout(Dev.layout, Dev.numElements, Dev.pVSBlob->GetBufferPointer(), Dev.pVSBlob->GetBufferSize(), &LayOut.m_pVertexLayout);
	Dev.pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Set the input layout
	DevContext.m_pImmediateContext->IASetInputLayout(LayOut.m_pVertexLayout );

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = Dev.m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &PShader.m_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Create Objects
	filePaths.push_back(TM.squareMesh);
	MeshGenerator();

	//filePaths2.push_back("Dwarf/Dwarf/dwarf.x");
	filePaths2.push_back("Dwarf/Dwarf/dwarf.x");
	MeshGenerator2();

	filePaths3.push_back(TM.SMGMesh);
	MeshGenerator3();

	filePaths4.push_back(TM.ninjaMesh);
	MeshGenerator4();

	// Create the Vertex and Pixel Buffers
	// Mesh 1
	for (int i = 0; i < m_mesh.size(); i++)
	{
		// Create vertex buffer
		hr = Dev.m_pd3dDevice->CreateBuffer(&m_mesh[i].VertexBuffer.GetDescription(), &m_mesh[i].VertexBuffer.m_InitData, &m_mesh[i].VertexBuffer.m_Buffer);
		if (FAILED(hr))
			return hr;

		// Create index buffer
		hr = Dev.m_pd3dDevice->CreateBuffer(&m_mesh[i].IndexBuffer.GetDescription(), &m_mesh[i].IndexBuffer.m_InitData, &m_mesh[i].IndexBuffer.m_Buffer);
		if (FAILED(hr))
			return hr;
	}
	// Mesh 2
	for (int i = 0; i < m_mesh2.size(); i++)
	{
		// Create vertex buffer
		hr = Dev.m_pd3dDevice->CreateBuffer(&m_mesh2[i].VertexBuffer.GetDescription(), &m_mesh2[i].VertexBuffer.m_InitData, &m_mesh2[i].VertexBuffer.m_Buffer);
		if (FAILED(hr))
			return hr;

		// Create index buffer
		hr = Dev.m_pd3dDevice->CreateBuffer(&m_mesh2[i].IndexBuffer.GetDescription(), &m_mesh2[i].IndexBuffer.m_InitData, &m_mesh2[i].IndexBuffer.m_Buffer);
		if (FAILED(hr))
			return hr;
	}
	// Mesh 3
	for (int i = 0; i < m_mesh3.size(); i++)
	{
		// Create vertex buffer
		hr = Dev.m_pd3dDevice->CreateBuffer(&m_mesh3[i].VertexBuffer.GetDescription(), &m_mesh3[i].VertexBuffer.m_InitData, &m_mesh3[i].VertexBuffer.m_Buffer);
		if (FAILED(hr))
			return hr;

		// Create index buffer
		hr = Dev.m_pd3dDevice->CreateBuffer(&m_mesh3[i].IndexBuffer.GetDescription(), &m_mesh3[i].IndexBuffer.m_InitData, &m_mesh3[i].IndexBuffer.m_Buffer);
		if (FAILED(hr))
			return hr;
	}
	// Mesh 4
	for (int i = 0; i < m_mesh4.size(); i++)
	{
		// Create vertex buffer
		hr = Dev.m_pd3dDevice->CreateBuffer(&m_mesh4[i].VertexBuffer.GetDescription(), &m_mesh4[i].VertexBuffer.m_InitData, &m_mesh4[i].VertexBuffer.m_Buffer);
		if (FAILED(hr))
			return hr;

		// Create index buffer
		hr = Dev.m_pd3dDevice->CreateBuffer(&m_mesh4[i].IndexBuffer.GetDescription(), &m_mesh4[i].IndexBuffer.m_InitData, &m_mesh4[i].IndexBuffer.m_Buffer);
		if (FAILED(hr))
			return hr;
	}

	// Create the constant buffers & Initialize The constant buffers
	ChangeEveryFrameBuffer = CBuffer(sizeof(CBChangesEveryFrame));
	hr = Dev.m_pd3dDevice->CreateBuffer(&ChangeEveryFrameBuffer.GetDescription(), NULL, &ChangeEveryFrameBuffer.m_Buffer);
	if (FAILED(hr))
		return hr;

	NeverChangeBuffer = CBuffer(sizeof(CBNeverChanges));
	hr = Dev.m_pd3dDevice->CreateBuffer(&NeverChangeBuffer.GetDescription(), NULL, &NeverChangeBuffer.m_Buffer);
	if (FAILED(hr))
		return hr;

	ChangeOnResizeBuffer = CBuffer(sizeof(CBChangeOnResize));
	//bd.ByteWidth = sizeof(CBChangeOnResize);
	hr = Dev.m_pd3dDevice->CreateBuffer(&ChangeOnResizeBuffer.GetDescription(), NULL, &ChangeOnResizeBuffer.m_Buffer);
	if (FAILED(hr))
		return hr;

	// Load the Texture

	hr = D3DX11CreateShaderResourceViewFromFile(Dev.m_pd3dDevice, L"seafloor.dds", NULL, NULL, &Tex2D.m_pTextureRV, NULL);
	if (FAILED(hr))
		return hr;

	// Create the sample state
	createTheSampleState(hr);
	if (FAILED(hr))
		return hr;

	// Initialize the world matrix
	m_World = XMMatrixIdentity();


	// Initialize the camera
	initCam();
	Amount_Meshes = m_mesh.size();
}

#elif defined(OPENGL)

CManager::CManager()
{
}


CManager::~CManager()
{
}
// Clear
void CManager::Clear() const
{
	glClearColor(0, 0, 255, 1);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
// Cambio de camara
void CManager::CamChange()
{
	m_ChangeCam++;
	if (m_ChangeCam > 1)
		m_ChangeCam = 0;
	if (m_ChangeCam == 0)
	{
		Cam.setEye(0.0f, 0.0f, 400.0f);
		Cam.setAt( 0.0f, 0.0f, -1.0f );
		Cam.setUp( 0.0f, 1.0f, 0.0f  );
	}
	if (m_ChangeCam == 1)
	{
		//Cam.m_World = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));
		Cam.setEye(0.0f, -400.0f, 0.0f);
		Cam.setAt(0.0f, 1.0f, 0.0f);
		Cam.setUp(0.0f, 0.0f, 1.0f);
	}
}
// Matriz del modelo


// Carga de un modelo en assimp
void CManager::LoadScene()
{
	
	Device.Model = aiImportFile("Dwarf/Dwarf/dwarf.x", aiProcessPreset_TargetRealtime_MaxQuality );

	if (!Device.Model)
	{
		std::cerr << "error: importing " << "dwarf.x" << " failed: " << aiGetErrorString() << std::endl;

	}
	for (size_t i = 0; i < Device.Model->mNumMeshes; i++)
	{
		CMesh Meshes;
		Mesh.push_back(Meshes);
		glGenVertexArrays(1, &Mesh[i].vertexArrayID);
		glBindVertexArray(Mesh[i].vertexArrayID);
		std::vector<std::uint32_t> mIndices;
		mIndices.reserve(Device.Model->mMeshes[i]->mNumFaces * 3);

		for (size_t j = 0; j < Device.Model->mMeshes[i]->mNumFaces; j++)
		{
			mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[0u]);
			mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[1u]);
			mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[2u]);
		}

		Mesh[i].LoadMeshV2(
			Device.Model->mMeshes[i]->mNumVertices,
			Device.Model->mMeshes[i]->mNumFaces * 3,
			Device.Model->mMeshes[i]->mVertices,
			Device.Model->mMeshes[i]->mTextureCoords[0],
			mIndices
		);

		m_tris1 = Device.Model->mMeshes[0]->mNumVertices;
		m_tris2 = Device.Model->mMeshes[1]->mNumVertices;
		//m_tris = Model->mMeshes[i]->mNumFaces * 3;
		glBindVertexArray(0);
	}
}
// Busqueda de textura
void CManager::loadTexture(int i)
{
		// Diffuse
		const aiMaterial* material = Device.Model->mMaterials[Device.Model->mMeshes[i]->mMaterialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				Mesh[i].texname += (string)path.data;
				Mesh[i].loadTexture(NULL);
			}
		}
}
void CManager::loadTexture2(int i)
{
	// Diffuse
	const aiMaterial* material = Device.Model->mMaterials[Device.Model->mMeshes[i]->mMaterialIndex];
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			Mesh2[i].texname += (string)path.data;
			Mesh2[i].loadTexture(TexBufferID2);
		}
	}
}
void CManager::loadTexture3(int i)
{
	// Diffuse
	const aiMaterial* material = Device.Model->mMaterials[Device.Model->mMeshes[i]->mMaterialIndex];
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			Mesh3[i].texname += (string)path.data;
			Mesh3[i].loadTexture(NULL);
		}
	}
}
void CManager::loadTexture4(int i)
{
	// Diffuse
	const aiMaterial* material = Device.Model->mMaterials[Device.Model->mMeshes[i]->mMaterialIndex];
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			Mesh4[i].texname += (string)path.data;
			Mesh4[i].loadTexture(NULL);
		}
	}
}
// Inicializacion de la camara
void CManager::initCamera()
{
	Cam.setEye(0.0f, 0.0f, 400.0f);
	Cam.setAt(0.0f, 0.0f, -1.0f);
	Cam.setUp(0.0f, 1.0f, 0.0f);
	Cam.setView();
	Cam.setProj(windowSize.x, windowSize.y);
	for (size_t i = 0; i < Mesh.size(); i++)
	{
		Cam.setMVP(Mesh[i].setMatrix());
	}
	for (size_t i = 0; i < Mesh2.size(); i++)
	{
		Cam.setMVP(Mesh2[i].setMatrix());
	}
	for (size_t i = 0; i < Mesh3.size(); i++)
	{
		Cam.setMVP(Mesh3[i].setMatrix());
	}
	for (size_t i = 0; i < Mesh4.size(); i++)
	{
		Cam.setMVP(Mesh4[i].setMatrix());
	}

	Cam2.setEye(0.0f, 0.0f, 400.0f);
	Cam2.setAt(0.0f, 0.0f, -1.0f);
	Cam2.setUp(0.0f, 1.0f, 0.0f);
	Cam2.setView();
	Cam2.setProj(windowSize.x, windowSize.y);
	
	//Cam.setEye(-300.0f, 0.0f, 400.0f);
	//Cam.setAt(0.0f, 0.0f, -1.0f);
	//Cam.setUp(0.0f, 1.0f, 0.0f);
	//Cam.setView();
	//Cam.setProj(windowSize.x, windowSize.y);
	//for (size_t i = 0; i < Mesh.size(); i++)
	//{
	//	Cam2.setMVP(Mesh[i].setMatrix());
	//}
	//for (size_t i = 0; i < Mesh2.size(); i++)
	//{
	//	Cam2.setMVP(Mesh2[i].setMatrix());
	//}
	//for (size_t i = 0; i < Mesh3.size(); i++)
	//{
	//	Cam2.setMVP(Mesh3[i].setMatrix());
	//}
	//for (size_t i = 0; i < Mesh4.size(); i++)
	//{
	//	Cam2.setMVP(Mesh4[i].setMatrix());
	//}
}
// Resize de la ventana
void CManager::WindowResize(int width, int height)
{
	int AspectRatio = float(windowSize.x) / float(windowSize.y);
	int W = height * AspectRatio;
	float left = (width - W) / 2;
	glViewport(left, 0, W, height);
	Cam.setProj(width, height);
	glutPostRedisplay();
}
// Inicializacion de la ventana
void CManager::initwindow(float width, float height)
{
	// Get the size of the window
	windowSize.x = width;
	windowSize.y = height;
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Create the window with the title "Hello,GL"
	glutInitWindowSize(width, height);
	glutCreateWindow("3D-CUBE");

}
// Inicializacion de la ventana ImGui
void CManager::initImGuiWindow()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	HWND windowHandle = FindWindow(NULL, L"3D-CUBE");
	HDC DevCont = GetDC(windowHandle);
	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplOpenGL3_Init("#version 400 core");
	ImGui::StyleColorsDark();
}
// Asignacion de los datos de ImGui
void CManager::setImGui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	// Create ImGui Test Window
	float amount = m_amountMeshes;
	float tris2 =  m_tris2;
	float tris1 =  m_tris1;
	ImGui::Begin("Model Data", 0);
	ImGui::SetWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2(2, 2), ImGuiSetCond_FirstUseEver);
	ImGui::Text("Application average");
	ImGui::Text("%.0f ms/frame (%.1f FPS)", 1000.0 / double(ImGui::GetIO().Framerate), double(ImGui::GetIO().Framerate));
	ImGui::Text("Mesh Count: %.0f", amount);
	ImGui::Text("Tris Mesh [0] Count: %.0f", tris1);
	ImGui::Text("Tris Mesh [1] Count: %.0f", tris2);
	ImGui::End();

	// Assamble together to draw data
	ImGui::Render();

	// Render Draw Data
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
// Generador de Meshes
void CManager::MeshLoader(const char* filepath)
{
	// Will allocate the different paths the will be used to charge diffent models
	for (size_t i = 0; i < filePaths.size(); i++)
	{
		//Mesh.clear();
		Device.Model = aiImportFile(filePaths[i], aiProcessPreset_TargetRealtime_MaxQuality);
		if (!Device.Model)
		{
			std::cerr << "error: importing " << "dwarf.x" << " failed: " << aiGetErrorString() << std::endl;

		}

		for (size_t i = 0; i < Device.Model->mNumMeshes; i++)
		{
		
		Mesh.push_back(Meshes);
		glGenVertexArrays(1, &Mesh[i].vertexArrayID);
		glBindVertexArray(Mesh[i].vertexArrayID);
		std::vector<std::uint32_t> mIndices;
		mIndices.reserve(Device.Model->mMeshes[i]->mNumFaces * 3);

		for (size_t j = 0; j < Device.Model->mMeshes[i]->mNumFaces; j++)
		{
			mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[0u]);
			mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[1u]);
			mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[2u]);
		}

		Mesh[i].LoadMeshV2(
			Device.Model->mMeshes[i]->mNumVertices,
			Device.Model->mMeshes[i]->mNumFaces * 3,
			Device.Model->mMeshes[i]->mVertices,
			Device.Model->mMeshes[i]->mTextureCoords[0],
			mIndices
		);

		//m_tris1 = Device.Model->mMeshes[0]->mNumVertices;
		//m_tris2 = Device.Model->mMeshes[1]->mNumVertices;
		//m_tris = Model->mMeshes[i]->mNumFaces * 3;

		glBindVertexArray(0);
		loadTexture(i);
	}
	}

}

void CManager::MeshLoader2(const char* filepath)
{
	// Will allocate the different paths the will be used to charge diffent models
	for (size_t i = 0; i < filePaths2.size(); i++)
	{
		//Mesh.clear();
		Device.Model = aiImportFile(filePaths2[i], aiProcessPreset_TargetRealtime_MaxQuality);
		if (!Device.Model)
		{
			std::cerr << "error: importing " << "dwarf.x" << " failed: " << aiGetErrorString() << std::endl;

		}

		for (size_t i = 0; i < Device.Model->mNumMeshes; i++)
		{

			Mesh2.push_back(Meshes2);
			glGenVertexArrays(1, &Mesh2[i].vertexArrayID);
			glBindVertexArray(Mesh2[i].vertexArrayID);
			std::vector<std::uint32_t> mIndices;
			mIndices.reserve(Device.Model->mMeshes[i]->mNumFaces * 3);

			for (size_t j = 0; j < Device.Model->mMeshes[i]->mNumFaces; j++)
			{
				mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[0u]);
				mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[1u]);
				mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[2u]);
			}

			Mesh2[i].LoadMeshV2(
				Device.Model->mMeshes[i]->mNumVertices,
				Device.Model->mMeshes[i]->mNumFaces * 3,
				Device.Model->mMeshes[i]->mVertices,
				Device.Model->mMeshes[i]->mTextureCoords[0],
				mIndices
			);

			glBindVertexArray(0);
			loadTexture2(i);
		}
	}

}

void CManager::MeshLoader3(const char* filepath)
{
	// Will allocate the different paths the will be used to charge diffent models
	for (size_t i = 0; i < filePaths3.size(); i++)
	{
		//Mesh.clear();
		Device.Model = aiImportFile(filePaths3[i], aiProcessPreset_TargetRealtime_MaxQuality);
		if (!Device.Model)
		{
			std::cerr << "error: importing " << "dwarf.x" << " failed: " << aiGetErrorString() << std::endl;

		}

		for (size_t i = 0; i < Device.Model->mNumMeshes; i++)
		{

			Mesh3.push_back(Meshes3);
			glGenVertexArrays(1, &Mesh3[i].vertexArrayID);
			glBindVertexArray(Mesh3[i].vertexArrayID);
			std::vector<std::uint32_t> mIndices;
			mIndices.reserve(Device.Model->mMeshes[i]->mNumFaces * 3);

			for (size_t j = 0; j < Device.Model->mMeshes[i]->mNumFaces; j++)
			{
				mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[0u]);
				mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[1u]);
				mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[2u]);
			}

			Mesh3[i].LoadMeshV2(
				Device.Model->mMeshes[i]->mNumVertices,
				Device.Model->mMeshes[i]->mNumFaces * 3,
				Device.Model->mMeshes[i]->mVertices,
				Device.Model->mMeshes[i]->mTextureCoords[0],
				mIndices
			);

			

			glBindVertexArray(0);
			loadTexture3(i);
		}
	}

}

void CManager::MeshLoader4(const char* filepath)
{
	// Will allocate the different paths the will be used to charge diffent models
	for (size_t i = 0; i < filePaths4.size(); i++)
	{
		//Mesh.clear();
		Device.Model = aiImportFile(filePaths4[i], aiProcessPreset_TargetRealtime_MaxQuality);
		if (!Device.Model)
		{
			std::cerr << "error: importing " << "dwarf.x" << " failed: " << aiGetErrorString() << std::endl;

		}

		for (size_t i = 0; i < Device.Model->mNumMeshes; i++)
		{

			Mesh4.push_back(Meshes3);
			glGenVertexArrays(1, &Mesh4[i].vertexArrayID);
			glBindVertexArray(Mesh4[i].vertexArrayID);
			std::vector<std::uint32_t> mIndices;
			mIndices.reserve(Device.Model->mMeshes[i]->mNumFaces * 3);

			for (size_t j = 0; j < Device.Model->mMeshes[i]->mNumFaces; j++)
			{
				mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[0u]);
				mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[1u]);
				mIndices.push_back(Device.Model->mMeshes[i]->mFaces[j].mIndices[2u]);
			}

			Mesh4[i].LoadMeshV2(
				Device.Model->mMeshes[i]->mNumVertices,
				Device.Model->mMeshes[i]->mNumFaces * 3,
				Device.Model->mMeshes[i]->mVertices,
				Device.Model->mMeshes[i]->mTextureCoords[0],
				mIndices
			);

			glBindVertexArray(0);
			loadTexture4(i);
		}
	}

}

void CManager::init()
{
	SHADER = new CShader("ShaderGL.fx");
	SHADER->Bind();

	// Render To Texture
	RTT.Initialize();
	RTT.Unbind();

	// Load Mesh 1
	filePaths.push_back("Dwarf/Dwarf/dwarf.x");
	MeshLoader("Dwarf/Dwarf/dwarf.x");
	for (size_t i = 0; i < Mesh.size(); i++)
	{
		Mesh[i].createMeshMatrix(0.0f, 0, 0, 0, 0.01f, 0.01f, 0.01f);

	}
	// Load Mesh 2
	filePaths2.push_back("Dwarf/Plane/PlaneToOpenGL.fbx");
	MeshLoader2("Dwarf/Dwarf/dwarf.x");
	for (size_t i = 0; i < Mesh2.size(); i++)
	{
		Mesh2[i].createMeshMatrix(0.0f, 2, 0, 0, 0.1f, 0.1f, 0.1f);
		//Mesh2[i].textID = TexBufferID2;
		Mesh2[i].textID = RTT.m_textureId;

	}
	// Load Mesh 3
	filePaths3.push_back("Dwarf/Plane/PlaneToOpenGL.fbx");
	MeshLoader3("Dwarf/Dwarf/dwarf.x");
	for (size_t i = 0; i < Mesh3.size(); i++)
	{
		Mesh3[i].createMeshMatrix(0.0f, -1, 0, 0, 0.01f, 0.01f, 0.01f);
	}
	// Load Mesh 4
	filePaths4.push_back("Dwarf/Dwarf/dwarf.x");
	MeshLoader4("Dwarf/Dwarf/dwarf.x");
	for (size_t i = 0; i < Mesh4.size(); i++)
	{
		Mesh4[i].createMeshMatrix(0.0f, 0, -1, 0, 0.01f, 0.01f, 0.01f);
	}
	initCamera();
	//m_amountMeshes = Mesh.size();
}




#endif