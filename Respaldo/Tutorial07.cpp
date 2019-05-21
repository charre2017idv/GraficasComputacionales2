//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
// Version: v 10.0
// Creation Date: 09/04/2019
// Author: Roberto Charreton Kaplun
// Summary: The hole proyect can run in DirectX and in OpenGL. (This version has Render to texture feature)
// Errors: 0 
// Bugs: 0
// Nice to Have: Pass cleanUp to Manager
//--------------------------------------------------------------------------------------

#pragma once
#include "Header.h"

#if defined(DIRECTX)
#include "CManager.h"
#include "CMesh.h"

//#include "ImGUI/ImGuiDirectX/imgui_impl_win32.h"


CManager g_manager;
int g_Change = 0;

RECT rc;
RECT lastRC;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();
void Draw(static float t);
void Texturas();
  
//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }
	

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_manager.m_hInst = hInstance;
    RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_manager.m_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_manager.m_hWnd )
        return E_FAIL;

    ShowWindow( g_manager.m_hWnd, nCmdShow );
    
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;
	if (!g_manager.initDevide(hr, g_manager.m_hWnd))
		return hr;
	
	g_manager.InitImGuiWindow();
		
    return S_OK;
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    if( g_manager.DevContext.m_pImmediateContext ) g_manager.DevContext.m_pImmediateContext->ClearState();

    if( g_manager.Tex2D.m_pSamplerLinear )   g_manager.Tex2D.m_pSamplerLinear->Release();
    if( g_manager.Tex2D.m_pTextureRV )		 g_manager.Tex2D.m_pTextureRV->Release();

    if( g_manager.m_pCBNeverChanges ) g_manager.m_pCBNeverChanges->Release();
    if( g_manager.m_pCBChangeOnResize ) g_manager.m_pCBChangeOnResize->Release();
    if( g_manager.m_pCBChangesEveryFrame ) g_manager.m_pCBChangesEveryFrame->Release();

	for (int i = 0; i < g_manager.m_mesh.size(); i++)
	{
		if( g_manager.m_mesh[i].VertexBuffer.m_Buffer ) g_manager.m_mesh[i].VertexBuffer.m_Buffer->Release();
		if( g_manager.m_mesh[i].IndexBuffer.m_Buffer )  g_manager.m_mesh[i].IndexBuffer.m_Buffer->Release();
	}

	for (int i = 0; i < g_manager.m_mesh2.size(); i++)
	{
		if (g_manager.m_mesh2[i].VertexBuffer.m_Buffer) g_manager.m_mesh2[i].VertexBuffer.m_Buffer->Release();
		if (g_manager.m_mesh2[i].IndexBuffer.m_Buffer)  g_manager.m_mesh2[i].IndexBuffer.m_Buffer->Release();
	}

	for (int i = 0; i < g_manager.m_mesh3.size(); i++)
	{
		if (g_manager.m_mesh3[i].VertexBuffer.m_Buffer) g_manager.m_mesh3[i].VertexBuffer.m_Buffer->Release();
		if (g_manager.m_mesh3[i].IndexBuffer.m_Buffer)  g_manager.m_mesh3[i].IndexBuffer.m_Buffer->Release();
	}

	for (int i = 0; i < g_manager.m_mesh4.size(); i++)
	{
		if (g_manager.m_mesh4[i].VertexBuffer.m_Buffer) g_manager.m_mesh4[i].VertexBuffer.m_Buffer->Release();
		if (g_manager.m_mesh4[i].IndexBuffer.m_Buffer)  g_manager.m_mesh4[i].IndexBuffer.m_Buffer->Release();
	}

    if( g_manager.LayOut.m_pVertexLayout ) g_manager.LayOut.m_pVertexLayout->Release();

    if( g_manager.VShader.m_pVertexShader ) g_manager.VShader.m_pVertexShader->Release();
    if( g_manager.PShader.m_pPixelShader )  g_manager.PShader.m_pPixelShader->Release();

    if( g_manager.Tex2D.m_pDepthStencil ) g_manager.Tex2D.m_pDepthStencil->Release();

    if( g_manager.RenTarg.m_pDepthStencilView ) g_manager.RenTarg.m_pDepthStencilView->Release();
    if( g_manager.RenTarg.m_pRenderTargetView ) g_manager.RenTarg.m_pRenderTargetView->Release();

    if( g_manager.SwCh.m_pSwapChain ) g_manager.SwCh.m_pSwapChain->Release();

    if( g_manager.DevContext.m_pImmediateContext ) g_manager.DevContext.m_pImmediateContext->Release();

    if( g_manager.Dev.m_pd3dDevice ) g_manager.Dev.m_pd3dDevice->Release();
	for (int i = 0; i <g_manager.m_mesh.size(); i++)
	{
		delete[]g_manager.m_mesh[i].m_vertices, g_manager.m_mesh[i].m_vertices;
	}

	for (int i = 0; i < g_manager.m_mesh2.size(); i++)
	{
		delete[]g_manager.m_mesh2[i].m_vertices, g_manager.m_mesh2[i].m_vertices;
	}

	for (int i = 0; i < g_manager.m_mesh3.size(); i++)
	{
		delete[]g_manager.m_mesh3[i].m_vertices, g_manager.m_mesh3[i].m_vertices;
	}

	for (int i = 0; i < g_manager.m_mesh4.size(); i++)
	{
		delete[]g_manager.m_mesh4[i].m_vertices, g_manager.m_mesh4[i].m_vertices;
	}
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	g_manager.m_Param = wParam;
	if (ImGui_ImplWin32_WndProcHandler(hWnd,message,wParam,lParam))
	{
		return true;
	}

	PAINTSTRUCT ps;
	HDC hdc;
	float xPos = LOWORD(lParam);
	float yPos = HIWORD(lParam);
	GetWindowRect(hWnd, &rc);
	if ((lastRC.right - lastRC.left) != (rc.right - rc.left) || (lastRC.bottom - lastRC.top) != (rc.bottom - rc.top))
	{
		if (!(rc.right - rc.left) <= 160 || (rc.bottom - rc.top) <= 28)
		{
			g_manager.resizeCam(SCREEN_WIDTH,SCREEN_HEIGHT, g_manager.DevContext.m_pImmediateContext,g_manager.ChangeOnResizeBuffer.m_Buffer);
			g_manager.SwapChainResize(rc.right - rc.left, rc.bottom - rc.top, g_manager.DevContext.m_pImmediateContext, g_manager.ChangeOnResizeBuffer.m_Buffer);
			lastRC = rc;
		}
	}

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		SetWindowPos(hWnd, hWnd, 800, 600, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

		break;
	case WM_KEYDOWN:
		if (wParam == 37 || wParam == 38 || wParam == 39 || wParam == 40 || wParam == 'W' || wParam == 'S')
		{
			g_manager.cameraMove(g_manager.DevContext.m_pImmediateContext, g_manager.m_pCBNeverChanges, wParam, xPos, yPos);
		}
		else if (wParam == 'X')
		{
			Texturas();
		}
		else if (wParam == 'C')
		{
			g_manager.cameraMove(g_manager.DevContext.m_pImmediateContext, g_manager.m_pCBNeverChanges, wParam, xPos, yPos);

		}
		else if (wParam == 'Q') // Scale negative
		{
			//g_manager.DwarfMatrix = g_manager.DwarfMatrix * g_manager.DwarfMatrixScale2;
		}
		else if (wParam == 'E') // Scale Positive
		{
			//g_manager.DwarfMatrix = g_manager.DwarfMatrix * g_manager.DwarfMatrixScale;
		}
		//else if (wParam == 'D')	// Rotate X Positive
			//g_manager.DwarfMatrix = g_manager.DwarfMatrix * XMMatrixRotationY(XM_PIDIV4 * g_manager.positiveRotation);
		//else if (wParam == 'A') // Rotate X Negative
			//g_manager.DwarfMatrix = g_manager.DwarfMatrix * XMMatrixRotationY(XM_PIDIV4 * g_manager.negativeRotation);
		//else if (wParam == 'M') // Rotate X Negative
			//g_manager.DwarfMatrix = g_manager.DwarfMatrix * XMMatrixRotationX(XM_PIDIV4 * 90);
		else if (wParam == 'L')
			g_manager.Camera.CameraMatrix.mWorld = g_manager.Camera.CameraMatrix.mWorld* XMMatrixRotationY(XM_PIDIV4 * g_manager.negativeRotation);

			//g_manager.DwarfMatrix = g_manager.DwarfMatrix * g_manager.translate;
		// <! LIGHT DIR >
		else if (wParam == 'M') // Scale Positive
		{
			g_manager.lightDir.y -= 1;
		}
		else if (wParam == 'N') // Scale Positive
		{
			g_manager.lightDir.y += 1;
			//g_manager.DwarfMatrix = g_manager.DwarfMatrix * g_manager.DwarfMatrixScale;
		}
		else if (wParam == 'K') // Scale Positive
		{
			g_manager.lightDir.x -= 1;
		}
		else if (wParam == 'J') // Scale Positive
		{
			g_manager.lightDir.x += 1;
			//g_manager.DwarfMatrix = g_manager.DwarfMatrix * g_manager.DwarfMatrixScale;
		}

		else if (wParam == 'T' && g_manager.isTimePress == false) // Stop time
		{
				g_manager.newTime = 0;
				g_manager.isTimePress = true;
		}
		else if (g_manager.isTimePress)
		{
			g_manager.newTime += .0002;
			g_manager.isTimePress = false;
		}
		else if (wParam == 'P' && g_manager.isParty == false)
		{
			//g_manager.m_R = 0.5f;
			g_manager.m_G = 0.2f;
			g_manager.m_B = 0.8f;
			g_manager.m_A = 1.0f;
			g_manager.isParty = true;
		}
		else if (g_manager.isParty)
		{
			g_manager.m_R = 1.0f;
			g_manager.m_G = 1.0f;
			g_manager.m_B = 1.0f;
			g_manager.m_A = 1.0f;
			g_manager.isParty = false;
		}

		break;
	case WM_RBUTTONDOWN:
		g_manager.camRotation(g_manager.DevContext.m_pImmediateContext,g_manager.m_pCBNeverChanges);
		break;
	case WM_RBUTTONUP:
		g_manager.Camera.isClicked = false;
		break;
	default:
		if (g_manager.Camera.isClicked)
		{
			// Setmouse para poder regresar el mouse al centro
			//GetCursorPos(&g_manager.Cam.ActualPos);
			g_manager.camRotation(g_manager.DevContext.m_pImmediateContext,g_manager.m_pCBNeverChanges);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


void Draw2(float t)
{
	// Cam View and Projection
	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(g_manager.Camera2.g_View);
	g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.NeverChangeBuffer.m_Buffer, 0, NULL, &cbNeverChanges, 0, 0);

	//CBChangeOnResize cbChangesOnResize;
	//cbChangesOnResize.mProjection = g_manager.Camera.cbChangesOnResize.mProjection;
	g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.ChangeOnResizeBuffer.m_Buffer, 0, NULL, &g_manager.Camera2.cbChangesOnResize, 0, 0);

	// Render Stuff
	g_manager.DevContext.m_pImmediateContext->VSSetShader(g_manager.VShader.m_pVertexShader, NULL, 0);
	g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(0, 1, &g_manager.NeverChangeBuffer.m_Buffer);
	g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(1, 1, &g_manager.ChangeOnResizeBuffer.m_Buffer);
	g_manager.DevContext.m_pImmediateContext->PSSetShader(g_manager.PShader.m_pPixelShader, NULL, 0);
	//g_manager.DevContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.Tex2D.m_pTextureRV);
	g_manager.DevContext.m_pImmediateContext->PSSetSamplers(0, 1, &g_manager.Tex2D.m_pSamplerLinear);

	// Set Vertex and Index
	/* Square */
	for (int i = 0; i < g_manager.m_mesh.size(); i++)
	{
		g_manager.m_mesh[i].createMeshMatrix(0.0f, 3, 1, 0, 0.3f, 0.3f, 0.3f);
		g_manager.m_mesh[i].TextureMesh.m_pTextureRV = g_manager.RTT.m_shaderResourceView;

		g_manager.m_mesh[i].m_MeshMatrix.vMeshColor = g_manager.m_vMeshColor;

		g_manager.DevContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_mesh[i].TextureMesh.m_pTextureRV);

		g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.ChangeEveryFrameBuffer.m_Buffer, 0, NULL, &g_manager.m_mesh[i].m_MeshMatrix, 0, 0);

		g_manager.DevContext.m_pImmediateContext->PSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		//// Set vertex buffer
		g_manager.DevContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_mesh[i].VertexBuffer.m_Buffer, &g_manager.m_mesh[i].VertexBuffer.m_stride, &g_manager.m_mesh[i].VertexBuffer.m_offset);

		// Set index buffer
		g_manager.DevContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_mesh[i].IndexBuffer.m_Buffer, DXGI_FORMAT_R16_UINT, 0);

		// Set primitive topology
		g_manager.DevContext.m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_manager.DevContext.m_pImmediateContext->DrawIndexed(g_manager.m_mesh[i].m_numIndex, 0, 0);

	}
	/* Dwarf */
	for (int i = 0; i < g_manager.m_mesh2.size(); i++)
	{

		g_manager.m_mesh2[i].createMeshMatrix(t, -3, 0, 0, 0.03f, 0.03f, 0.03f); // Front view: XM_PIDIV4 * 180

		g_manager.m_mesh2[i].m_MeshMatrix.vMeshColor = g_manager.m_vMeshColor;

		g_manager.DevContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_mesh2[i].TextureMesh.m_pTextureRV);

		g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.ChangeEveryFrameBuffer.m_Buffer, 0, NULL, &g_manager.m_mesh2[i].m_MeshMatrix, 0, 0);

		g_manager.DevContext.m_pImmediateContext->PSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		//// Set vertex buffer
		g_manager.DevContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_mesh2[i].VertexBuffer.m_Buffer, &g_manager.m_mesh2[i].VertexBuffer.m_stride, &g_manager.m_mesh2[i].VertexBuffer.m_offset);

		// Set index buffer
		g_manager.DevContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_mesh2[i].IndexBuffer.m_Buffer, DXGI_FORMAT_R16_UINT, 0);

		// Set primitive topology
		g_manager.DevContext.m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_manager.DevContext.m_pImmediateContext->DrawIndexed(g_manager.m_mesh2[i].m_numIndex, 0, 0);

	}
	/* Arms */
	for (int i = 0; i < g_manager.m_mesh3.size(); i++)
	{
		//g_manager.Camera.createMeshMatrix();

		g_manager.m_mesh3[i].m_MeshMatrix = g_manager.Camera.CameraMatrix;

		//g_manager.m_mesh3[i].createMeshMatrix(1.0f, 0, 0, 0, 0.03f, 0.03f, 0.03f);

		g_manager.m_mesh3[i].m_MeshMatrix.vMeshColor = g_manager.m_vMeshColor;

		g_manager.DevContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_mesh3[i].TextureMesh.m_pTextureRV);

		g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.ChangeEveryFrameBuffer.m_Buffer, 0, NULL, &g_manager.m_mesh3[i].m_MeshMatrix, 0, 0);

		g_manager.DevContext.m_pImmediateContext->PSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		//// Set vertex buffer
		g_manager.DevContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_mesh3[i].VertexBuffer.m_Buffer, &g_manager.m_mesh3[i].VertexBuffer.m_stride, &g_manager.m_mesh3[i].VertexBuffer.m_offset);

		// Set index buffer
		g_manager.DevContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_mesh3[i].IndexBuffer.m_Buffer, DXGI_FORMAT_R16_UINT, 0);

		// Set primitive topology
		g_manager.DevContext.m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_manager.DevContext.m_pImmediateContext->DrawIndexed(g_manager.m_mesh3[i].m_numIndex, 0, 0);

	}
	/* Helmet */
	for (int i = 0; i < g_manager.m_mesh4.size(); i++)
	{
		g_manager.m_mesh4[i].createMeshMatrix(-t, 0, 0, 0, 0.03f, 0.03f, 0.03f);

		g_manager.m_mesh4[i].m_MeshMatrix.vMeshColor = g_manager.m_vMeshColor;

		g_manager.DevContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_mesh4[i].TextureMesh.m_pTextureRV);

		g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.ChangeEveryFrameBuffer.m_Buffer, 0, NULL, &g_manager.m_mesh4[i].m_MeshMatrix, 0, 0);

		g_manager.DevContext.m_pImmediateContext->PSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		//// Set vertex buffer
		g_manager.DevContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_mesh4[i].VertexBuffer.m_Buffer, &g_manager.m_mesh4[i].VertexBuffer.m_stride, &g_manager.m_mesh4[i].VertexBuffer.m_offset);

		// Set index buffer
		g_manager.DevContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_mesh4[i].IndexBuffer.m_Buffer, DXGI_FORMAT_R16_UINT, 0);

		// Set primitive topology
		g_manager.DevContext.m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_manager.DevContext.m_pImmediateContext->DrawIndexed(g_manager.m_mesh4[i].m_numIndex, 0, 0);

	}

	/* Set ImGui */
	g_manager.SetImGui();
	g_manager.m_timer += g_manager.newTime;

	if (g_manager.isParty)
	{
		if (g_manager.m_R > 1.0f)
			g_manager.m_incrase = -g_manager.newTime;
		else if (g_manager.m_R < 0.0f)
			g_manager.m_incrase = g_manager.newTime;
		g_manager.m_R += g_manager.m_incrase;
	}
}

//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
    // Update our time
    static float t = 0.0f;
    if( g_manager.DevContext.m_driverType == D3D_DRIVER_TYPE_REFERENCE )
    {
        t += ( float )XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if( dwTimeStart == 0 )
            dwTimeStart = dwTimeCur;
        t = ( dwTimeCur - dwTimeStart ) / 1000.0f;
    }
	
    // Modify the color
	g_manager.m_vMeshColor.x =g_manager.m_R;
	g_manager.m_vMeshColor.y =g_manager.m_G;
	g_manager.m_vMeshColor.z =g_manager.m_B;
	g_manager.m_vMeshColor.w =g_manager.m_A;
   
	g_manager.difuseColor.x = g_manager.m_R;
	g_manager.difuseColor.y = g_manager.m_G;
	g_manager.difuseColor.z = g_manager.m_B;
	g_manager.difuseColor.w = g_manager.m_A;


	g_manager.Clear();

    // Render to texture
	g_manager.RTT.SetRenderTarget(g_manager.DevContext.m_pImmediateContext);
	g_manager.RTT.ClearRenderTarget(g_manager.DevContext.m_pImmediateContext);
	//Draw2(t); // Square Draw
	g_manager.DevContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.RenTarg.m_pRenderTargetView, g_manager.RenTarg.m_pDepthStencilView);
	
	// Normal Render
	Draw(t); // Original Draw

    // Present our back buffer to our front buffer
    g_manager.SwCh.m_pSwapChain->Present( 0, 0 );
}

void Draw(static float t)
{
	// Cam View and Projection
	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(g_manager.Camera.g_View);
	g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.NeverChangeBuffer.m_Buffer, 0, NULL, &cbNeverChanges, 0, 0);

	//CBChangeOnResize cbChangesOnResize;
	//cbChangesOnResize.mProjection = g_manager.Camera.cbChangesOnResize.mProjection;
	g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.ChangeOnResizeBuffer.m_Buffer, 0, NULL, &g_manager.Camera.cbChangesOnResize, 0, 0);

	// Shader Stuff
	g_manager.DevContext.m_pImmediateContext->VSSetShader(g_manager.VShader.m_pVertexShader, NULL, 0);
	g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(0, 1, &g_manager.NeverChangeBuffer.m_Buffer);
	g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(1, 1, &g_manager.ChangeOnResizeBuffer.m_Buffer);
	g_manager.DevContext.m_pImmediateContext->PSSetShader(g_manager.PShader.m_pPixelShader, NULL, 0);
	//g_manager.DevContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.Tex2D.m_pTextureRV);
	g_manager.DevContext.m_pImmediateContext->PSSetSamplers(0, 1, &g_manager.Tex2D.m_pSamplerLinear);

	// Set Vertex and Index
	/* Square 
	for (int i = 0; i < g_manager.m_mesh.size(); i++)
	{
		g_manager.m_mesh[i].createMeshMatrix(0.0f, 3, 1, 0, 0.3f, 0.3f, 0.3f);
		g_manager.m_mesh[i].TextureMesh.m_pTextureRV = g_manager.RTT.m_shaderResourceView;

		g_manager.m_mesh[i].m_MeshMatrix.vMeshColor = g_manager.m_vMeshColor;

		g_manager.DevContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_mesh[i].TextureMesh.m_pTextureRV);

		g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.ChangeEveryFrameBuffer.m_Buffer, 0, NULL, &g_manager.m_mesh[i].m_MeshMatrix, 0, 0);

		g_manager.DevContext.m_pImmediateContext->PSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		//// Set vertex buffer
		g_manager.DevContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_mesh[i].VertexBuffer.m_Buffer, &g_manager.m_mesh[i].VertexBuffer.m_stride, &g_manager.m_mesh[i].VertexBuffer.m_offset);

		// Set index buffer
		g_manager.DevContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_mesh[i].IndexBuffer.m_Buffer, DXGI_FORMAT_R16_UINT, 0);

		// Set primitive topology
		g_manager.DevContext.m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		g_manager.DevContext.m_pImmediateContext->DrawIndexed(g_manager.m_mesh[i].m_numIndex, 0, 0);

	}
	/* Dwarf 
	for (int i = 0; i < g_manager.m_mesh2.size(); i++)
	{

		g_manager.m_mesh2[i].createMeshMatrix(t, -3, 0, 0, 0.03f, 0.03f, 0.03f); // Front view: XM_PIDIV4 * 180

		g_manager.m_mesh2[i].m_MeshMatrix.vMeshColor = g_manager.m_vMeshColor;

		g_manager.DevContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_mesh2[i].TextureMesh.m_pTextureRV);

		g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.ChangeEveryFrameBuffer.m_Buffer, 0, NULL, &g_manager.m_mesh2[i].m_MeshMatrix, 0, 0);

		g_manager.DevContext.m_pImmediateContext->PSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		//// Set vertex buffer
		g_manager.DevContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_mesh2[i].VertexBuffer.m_Buffer, &g_manager.m_mesh2[i].VertexBuffer.m_stride, &g_manager.m_mesh2[i].VertexBuffer.m_offset);

		// Set index buffer
		g_manager.DevContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_mesh2[i].IndexBuffer.m_Buffer, DXGI_FORMAT_R16_UINT, 0);

		// Set primitive topology
		g_manager.DevContext.m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_manager.DevContext.m_pImmediateContext->DrawIndexed(g_manager.m_mesh2[i].m_numIndex, 0, 0);

	}
	/* Arms 
	for (int i = 0; i < g_manager.m_mesh3.size(); i++)
	{
		g_manager.Camera.createMeshMatrix();
		g_manager.m_mesh3[i].m_MeshMatrix.mWorld = g_manager.Camera.CameraMatrix.mWorld;
		//g_manager.m_mesh3[i].createMeshMatrix(1.0f, 0, 0, 0, 0.03f, 0.03f, 0.03f);

		g_manager.m_mesh3[i].m_MeshMatrix.vMeshColor = g_manager.m_vMeshColor;

		g_manager.DevContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_mesh3[i].TextureMesh.m_pTextureRV);

		g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.ChangeEveryFrameBuffer.m_Buffer, 0, NULL, &g_manager.m_mesh3[i].m_MeshMatrix, 0, 0);

		g_manager.DevContext.m_pImmediateContext->PSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		//// Set vertex buffer
		g_manager.DevContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_mesh3[i].VertexBuffer.m_Buffer, &g_manager.m_mesh3[i].VertexBuffer.m_stride, &g_manager.m_mesh3[i].VertexBuffer.m_offset);

		// Set index buffer
		g_manager.DevContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_mesh3[i].IndexBuffer.m_Buffer, DXGI_FORMAT_R16_UINT, 0);

		// Set primitive topology
		g_manager.DevContext.m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_manager.DevContext.m_pImmediateContext->DrawIndexed(g_manager.m_mesh3[i].m_numIndex, 0, 0);

	}

	/* Helmeth */
	for (int i = 0; i < g_manager.m_mesh4.size(); i++)
	{
		g_manager.m_mesh4[i].createMeshMatrix(0, .2, 0, 0, 1.0f,1.0f, 1.0f);
		
		//g_manager.m_mesh4[i].m_MeshMatrix.vMeshColor = g_manager.m_vMeshColor;
		// Set Shader attributes
		g_manager.m_mesh4[i].setMaterial(
			g_manager.lightDir,
			g_manager.SpecularPower,
			g_manager.SpecularColor,
			g_manager.difuseColor,
			g_manager.NEye,
			XMFLOAT4(0, 0, 1, .7),
			XMFLOAT4(0, 0, 1, .7),
			XMFLOAT4(0, 0, 1, .7));
		

		g_manager.DevContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_mesh4[i].TextureMesh.m_pTextureRV);

		g_manager.DevContext.m_pImmediateContext->UpdateSubresource(g_manager.ChangeEveryFrameBuffer.m_Buffer, 0, NULL, &g_manager.m_mesh4[i].m_MeshMatrix, 0, 0);

		g_manager.DevContext.m_pImmediateContext->PSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		g_manager.DevContext.m_pImmediateContext->VSSetConstantBuffers(2, 1, &g_manager.ChangeEveryFrameBuffer.m_Buffer);
		//// Set vertex buffer
		g_manager.DevContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_mesh4[i].VertexBuffer.m_Buffer, &g_manager.m_mesh4[i].VertexBuffer.m_stride, &g_manager.m_mesh4[i].VertexBuffer.m_offset);

		// Set index buffer
		g_manager.DevContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_mesh4[i].IndexBuffer.m_Buffer, DXGI_FORMAT_R16_UINT, 0);

		// Set primitive topology
		g_manager.DevContext.m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_manager.DevContext.m_pImmediateContext->DrawIndexed(g_manager.m_mesh4[i].m_numIndex, 0, 0);

	}

	g_manager.SetImGui();
	g_manager.m_timer += g_manager.newTime;

	if (g_manager.isParty)
	{
		if (g_manager.m_R > 1.0f)
			g_manager.m_incrase = -g_manager.newTime;
		else if (g_manager.m_R < 0.0f)
			g_manager.m_incrase = g_manager.newTime;
		g_manager.m_R += g_manager.m_incrase;
	}

}

void Texturas()
{
	g_Change++;
	if (g_Change > 3)
		g_Change = 0;
	if (g_Change == 0)
		D3DX11CreateShaderResourceViewFromFile(g_manager.Dev.m_pd3dDevice, L"seafloor.dds", NULL, NULL, & g_manager.Tex2D.m_pTextureRV, NULL);
	if (g_Change == 1)
		D3DX11CreateShaderResourceViewFromFile(g_manager.Dev.m_pd3dDevice, L"Spider.dds", NULL, NULL, &g_manager.Tex2D.m_pTextureRV, NULL);
	if (g_Change == 2)
		D3DX11CreateShaderResourceViewFromFile(g_manager.Dev.m_pd3dDevice, L"Grey-square.dds", NULL, NULL, &g_manager.Tex2D.m_pTextureRV, NULL);
	if (g_Change == 3)
		D3DX11CreateShaderResourceViewFromFile(g_manager.Dev.m_pd3dDevice, L"wire.dds", NULL, NULL, &g_manager.Tex2D.m_pTextureRV, NULL);
}


#elif defined(OPENGL)
#include "CManager.h"

CManager Manager;
int frame = 1;
int t = 0;
void render();

struct simpleVertex
{
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(frame, timer, 0);
}

void WindowResize(int width, int height)
{
	if (width == 0 || height == 0) return;

	glViewport(0, 0, width, height);
	Manager.Cam.setProj(width, height);
}

void specialKeys(int key, int x, int y)
{
	// Move Camera
	if (key == GLUT_KEY_RIGHT)		// Derecha
	{
		Manager.Cam.Eye += Manager.Cam.Right * 0.1f; // Velocity
		Manager.Cam.At += Manager.Cam.Right * 0.1f;
	}

	if (key == GLUT_KEY_LEFT)		// Izquierda
	{
		Manager.Cam.Eye -= Manager.Cam.Right * 0.1f; // Velocity
		Manager.Cam.At -= Manager.Cam.Right * 0.1f;
	}
	if (key == GLUT_KEY_UP)			// Arriba
	{
		Manager.Cam.Eye += Manager.Cam.Up * 0.1f; // Velocity
		Manager.Cam.At += Manager.Cam.Up * 0.1f;
	}
	if (key == GLUT_KEY_DOWN)		// Abajo
	{
		Manager.Cam.Eye -= Manager.Cam.Up * 0.1f; // Velocity
		Manager.Cam.At -= Manager.Cam.Up * 0.1f;
	}
	if (key == GLUT_KEY_PAGE_UP)	// Zoom adelante
	{
		Manager.Cam.Eye += Manager.Cam.Front * 50.0f; // Velocity
		Manager.Cam.At += Manager.Cam.Front * 50.0f;
	}
	if (key == GLUT_KEY_PAGE_DOWN)	// Zoom atras
	{
		Manager.Cam.Eye -= Manager.Cam.Front * 50.0f; // Velocity
		Manager.Cam.At -= Manager.Cam.Front * 50.0f;
	}

	glutPostRedisplay();
}

void MouseInput(int button, int state, int x, int y)
{
	if (button == 2 && state == 0)
	{
		GetCursorPos(&Manager.Cam.PastCursor);
		Manager.Cam.isPress = true;
	}
	if (state == 1)
	{
		Manager.Cam.isPress = false;
	}
}

void keyPressed(unsigned char key, int x, int y)
{
	// Move Cube
	if (key == 'W')		// UP
		Manager.Mesh[0].m_y += 1.0f;
	if (key == 'S')		// DOWN
		Manager.Mesh[0].m_y -= 1.0f;
	if (key == 'A')		// LEFT
		Manager.Mesh[0].m_x -= 1.0f;
	if (key == 'D')		// RIGHT
		Manager.Mesh[0].m_x += 1.0f;
	// Scale Mesh
	if (key == 'E')
	{
		Manager.Mesh[0].m_Scalex += 0.01;
		Manager.Mesh[0].m_Scaley += 0.01;
		Manager.Mesh[0].m_Scalez += 0.01;
		Manager.Mesh[0].MeshMatrix = glm::scale(Manager.Mesh[0].MeshMatrix, Manager.Mesh[0].newScale);
	}
	if (key == 'Q')
	{
		Manager.Mesh[0].m_Scalex -= 0.01;
		Manager.Mesh[0].m_Scaley -= 0.01;
		Manager.Mesh[0].m_Scalez -= 0.01;
		Manager.Mesh[0].MeshMatrix = glm::scale(Manager.Mesh[0].MeshMatrix, Manager.Mesh[0].newScale);
	}
	// Zoom con grados
	if (key == 'Z')
	{
		Manager.Cam.setProj(SCREEN_WIDTH, SCREEN_HEIGHT);
		Manager.Cam.m_FOV += 3;
	}
	if (key == 'X')
	{
		Manager.Cam.setProj(SCREEN_WIDTH, SCREEN_HEIGHT);
		Manager.Cam.m_FOV -= 3;
	}

	// Change Camera
	if (key == 'C')
	{
		Manager.CamChange();
	}

	// Interpolation scale 
	if (key == 'I' && Manager.isScaling == false && Manager.isTimePress == false)
	{
		Manager.Mesh[0].m_Scalex = 0.01f;
		Manager.Mesh[0].m_Scaley = 0.01f;
		Manager.Mesh[0].m_Scalez = 0.01f;
		Manager.isScaling = true;
	}
	else if (Manager.isScaling)
	{
		Manager.Mesh[0].m_Scalex = sin(Manager.Mesh[0].m_timer) / 50;
		Manager.Mesh[0].m_Scaley = sin(Manager.Mesh[0].m_timer) / 50;
		Manager.Mesh[0].m_Scalez = sin(Manager.Mesh[0].m_timer) / 50;
		Manager.isScaling = false;
	}

	// Rotate mesh according to the time
	if (key == 'T' & Manager.isTimePress == false)
	{
		Manager.newTime = 0;
		Manager.isTimePress = true;
	}
	else if (Manager.isTimePress)
	{
		Manager.newTime += .2;
		Manager.isTimePress = false;
	}
	// LIGHT DIRECTION
	else if (key == 'M') // Scale Positive
	{
		Manager.ligthDir[1] -= 1;
	}
	else if (key == 'N') // Scale Positive
	{
		Manager.ligthDir[1] += 1;
	}
	else if (key == 'K') // Scale Positive
	{
		Manager.ligthDir[0] -= 1;
	}
	else if (key == 'J') // Scale Positive
	{
		Manager.ligthDir[0] += 1;
	}
	// Set Party Mode
	if (key == 'P' && Manager.isParty == false)
	{
		Manager.m_G = 0.2f;
		Manager.m_B = 0.8f;
		Manager.m_A = 1.0f;
		Manager.isParty = true;
		//Manager.isScaling = true; //  For parting and scaling
	}
	else if (Manager.isParty)
	{
		Manager.m_R = 1.0f;
		Manager.m_G = 1.0f;
		Manager.m_B = 1.0f;
		Manager.m_A = 1.0f;
		Manager.isParty = false;
	}

	glutPostRedisplay();
}

void Draw()
{
	/* Set camera propeties */
	Manager.Cam.setView();
	Manager.Clear();

	/* Shader data */
	glm::vec4 Color2 = glm::vec4(Manager.m_R, Manager.m_G, Manager.m_B, Manager.m_A); // Create a vector to allocate color
	Manager.SHADER->setUniform4f("u_Color", Color2.x, Color2.y, Color2.z, Color2.w);		// Texture Color
	Manager.SHADER->setUniformMat4f("u_MVP", Manager.Cam.m_MVP);
	//Manager.SHADER->setUniform3f("u_LightDir", 0.0f,-1.0f,0.0f);		// Light Direction
	Manager.SHADER->setUniform4fv("u_LightDir", 1, &Manager.ligthDir[0]);
	Manager.SHADER->setUniform4fv("u_ViewPosition", 1, &Manager.Cam.Eye[0]);
	Manager.SHADER->setUniform4fv("u_SpecularPower", 1, &Manager.specularPowe[0]);
	Manager.SHADER->setUniform4fv("u_SpecularColor", 1, &Manager.specularColor[0]);
	Manager.SHADER->setUniform4fv("u_DifuseColor", 1, &Manager.difuseColo[0]);

	/* Render the Objects */
	for (size_t i = 0; i < Manager.Mesh.size(); i++)
	{
		//Manager.m_timer
		Manager.Mesh[i].createMeshMatrix( 0, 0, 0, 0, 0.01f, 0.01f, 0.01f);

		Manager.Cam.setMVP(Manager.Mesh[i].setMatrix());
		glBindVertexArray(Manager.Mesh[i].vertexArrayID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, Manager.Mesh[i].buuferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), 0);

		GLuint texID = glGetUniformLocation(Manager.SHADER->m_handle_ID, "u_Texture");
		glUniform1i(texID, Manager.Mesh[i].textID);
		glActiveTexture(GL_TEXTURE0 + Manager.Mesh[i].textID);

		glBindTexture(GL_TEXTURE_2D, Manager.Mesh[i].textID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (3 * sizeof(float)));

		glBindTexture(GL_TEXTURE_2D, Manager.Mesh[i].buuferID);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (5 * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, Manager.Mesh[i].m_tris * 3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);


	}
	/*
	for (size_t i = 0; i < Manager.Mesh2.size(); i++)
	{
		Manager.Cam.setMVP(Manager.Mesh2[i].setMatrix());
		Manager.SHADER->setUniformMat4f("u_MVP", Manager.Cam.m_MVP);

		glBindVertexArray(Manager.Mesh2[i].vertexArrayID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Manager.Mesh2[i].buuferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), 0);

		GLuint texID2 = glGetUniformLocation(Manager.SHADER->m_handle_ID, "u_Texture");

		glUniform1i(texID2, Manager.Mesh2[i].textID);
		glActiveTexture(GL_TEXTURE0 + Manager.Mesh2[i].textID);
		glBindTexture(GL_TEXTURE_2D, Manager.Mesh2[i].textID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (3 * sizeof(float)));
		glDrawArrays(GL_TRIANGLES, 0, Manager.Mesh2[i].m_tris * 3);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
	for (size_t i = 0; i < Manager.Mesh3.size(); i++)
	{
		Manager.Cam.setMVP(Manager.Mesh3[i].setMatrix());
		Manager.SHADER->setUniformMat4f("u_MVP", Manager.Cam.m_MVP);

		glBindVertexArray(Manager.Mesh3[i].vertexArrayID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Manager.Mesh3[i].buuferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), 0);

		GLuint texID3 = glGetUniformLocation(Manager.SHADER->m_handle_ID, "u_Texture");
		glUniform1i(texID3, Manager.Mesh3[i].textID);
		glActiveTexture(GL_TEXTURE0 + Manager.Mesh3[i].textID);
		glBindTexture(GL_TEXTURE_2D, Manager.Mesh3[i].textID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (3 * sizeof(float)));
		glDrawArrays(GL_TRIANGLES, 0, Manager.Mesh3[i].m_tris * 3);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
	for (size_t i = 0; i < Manager.Mesh4.size(); i++)
	{
		Manager.Cam.setMVP(Manager.Mesh4[i].setMatrix());
		Manager.SHADER->setUniformMat4f("u_MVP", Manager.Cam.m_MVP);

		glBindVertexArray(Manager.Mesh4[i].vertexArrayID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Manager.Mesh4[i].buuferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), 0);

		GLuint texID4 = glGetUniformLocation(Manager.SHADER->m_handle_ID, "u_Texture");
		glUniform1i(texID4, Manager.Mesh4[i].textID);
		glActiveTexture(GL_TEXTURE0 + Manager.Mesh4[i].textID);
		glBindTexture(GL_TEXTURE_2D, Manager.Mesh4[i].textID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (3 * sizeof(float)));
		glDrawArrays(GL_TRIANGLES, 0, Manager.Mesh4[i].m_tris * 3);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
	*/

	Manager.Mesh[0].m_timer += Manager.newTime; // Update time
	Manager.Mesh2[0].m_timer += Manager.newTime; // Update time
	Manager.Mesh3[0].m_timer += Manager.newTime; // Update time
	Manager.Mesh4[0].m_timer += Manager.newTime; // Update time

	Manager.m_timer += .2;
	/* Set Party Mode */
	if (Manager.isParty)
	{
		if (Manager.m_R > 1.0f)
			Manager.m_incrase = -0.05f;
		else if (Manager.m_R < 0.0f)
			Manager.m_incrase = 0.05f;
		Manager.m_R += Manager.m_incrase;
	}

	if (Manager.isScaling)
	{
		Manager.Mesh[0].m_Scalex = sin(Manager.Mesh[0].m_timer) / 50;
		Manager.Mesh[0].m_Scaley = sin(Manager.Mesh[0].m_timer) / 50;
		Manager.Mesh[0].m_Scalez = sin(Manager.Mesh[0].m_timer) / 50;
	}

	Manager.setImGui(); // Set ImGui information window
}

void Draw2()
{
	/* Set camera propeties */
	Manager.Cam2.setView();
	/* Clear background */
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Shader data */
	glm::vec4 Color = glm::vec4(Manager.m_R, Manager.m_G, Manager.m_B, Manager.m_A); // Create a vector to allocate color
	Manager.SHADER->setUniform4f("u_Color", Color.x, Color.y, Color.z, Color.w);		// Texture Color

	/* Render the Objects
	for (size_t i = 0; i < Manager.Mesh.size(); i++)
	{
		Manager.Cam2.setMVP(Manager.Mesh[i].setMatrix());
		Manager.SHADER->setUniformMat4f("u_MVP", Manager.Cam2.m_MVP);

		glBindVertexArray(Manager.Mesh[i].vertexArrayID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Manager.Mesh[i].buuferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), 0);

		GLuint texID = glGetUniformLocation(Manager.SHADER->m_handle_ID, "u_Texture");
		glUniform1i(texID, Manager.Mesh[i].textID);
		glActiveTexture(GL_TEXTURE0 + Manager.Mesh[i].textID);
		glBindTexture(GL_TEXTURE_2D, Manager.Mesh[i].textID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (3 * sizeof(float)));
		glDrawArrays(GL_TRIANGLES, 0, Manager.Mesh[i].m_tris * 3);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);


	}
	for (size_t i = 0; i < Manager.Mesh2.size(); i++)
	{
		Manager.Cam2.setMVP(Manager.Mesh2[i].setMatrix());
		Manager.SHADER->setUniformMat4f("u_MVP", Manager.Cam2.m_MVP);

		glBindVertexArray(Manager.Mesh2[i].vertexArrayID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Manager.Mesh2[i].buuferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), 0);

		GLuint texID2 = glGetUniformLocation(Manager.SHADER->m_handle_ID, "u_Texture");

		glUniform1i(texID2, Manager.Mesh2[i].textID);
		glActiveTexture(GL_TEXTURE0 + Manager.Mesh2[i].textID);
		glBindTexture(GL_TEXTURE_2D, Manager.Mesh2[i].textID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (3 * sizeof(float)));
		glDrawArrays(GL_TRIANGLES, 0, Manager.Mesh2[i].m_tris * 3);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
	for (size_t i = 0; i < Manager.Mesh3.size(); i++)
	{
		Manager.Cam2.setMVP(Manager.Mesh3[i].setMatrix());
		Manager.SHADER->setUniformMat4f("u_MVP", Manager.Cam2.m_MVP);

		glBindVertexArray(Manager.Mesh3[i].vertexArrayID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Manager.Mesh3[i].buuferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), 0);

		GLuint texID3 = glGetUniformLocation(Manager.SHADER->m_handle_ID, "u_Texture");
		glUniform1i(texID3, Manager.Mesh3[i].textID);
		glActiveTexture(GL_TEXTURE0 + Manager.Mesh3[i].textID);
		glBindTexture(GL_TEXTURE_2D, Manager.Mesh3[i].textID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (3 * sizeof(float)));
		glDrawArrays(GL_TRIANGLES, 0, Manager.Mesh3[i].m_tris * 3);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
	for (size_t i = 0; i < Manager.Mesh4.size(); i++)
	{
		Manager.Cam2.setMVP(Manager.Mesh4[i].setMatrix());
		Manager.SHADER->setUniformMat4f("u_MVP", Manager.Cam2.m_MVP);

		glBindVertexArray(Manager.Mesh4[i].vertexArrayID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Manager.Mesh4[i].buuferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), 0);

		GLuint texID4 = glGetUniformLocation(Manager.SHADER->m_handle_ID, "u_Texture");
		glUniform1i(texID4, Manager.Mesh4[i].textID);
		glActiveTexture(GL_TEXTURE0 + Manager.Mesh4[i].textID);
		glBindTexture(GL_TEXTURE_2D, Manager.Mesh4[i].textID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (3 * sizeof(float)));
		glDrawArrays(GL_TRIANGLES, 0, Manager.Mesh4[i].m_tris * 3);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}


	Manager.Mesh[0].m_timer += Manager.newTime; // Update time
	Manager.Mesh2[0].m_timer += Manager.newTime; // Update time
	Manager.Mesh3[0].m_timer += Manager.newTime; // Update time
	Manager.Mesh4[0].m_timer += Manager.newTime; // Update time

	/* Set Party Mode */
	if (Manager.isParty)
	{
		if (Manager.m_R > 1.0f)
			Manager.m_incrase = -0.05f;
		else if (Manager.m_R < 0.0f)
			Manager.m_incrase = 0.05f;
		Manager.m_R += Manager.m_incrase;
	}

	if (Manager.isScaling)
	{
		Manager.Mesh[0].m_Scalex = sin(Manager.Mesh[0].m_timer) / 50;
		Manager.Mesh[0].m_Scaley = sin(Manager.Mesh[0].m_timer) / 50;
		Manager.Mesh[0].m_Scalez = sin(Manager.Mesh[0].m_timer) / 50;
	}

	Manager.setImGui(); // Set ImGui information window
}

void render()
{
	// Check if is clicked the right button
	if (Manager.Cam.isPress)
		Manager.Cam.setRotation();
	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	/* Draw for Render to Texture */

	// <! Warning glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH); >

	Manager.RTT.Bind();
	Manager.RTT.Clear();

	//Draw2();

	glFlush();
	Manager.RTT.Unbind();

	/* Original Draw */
	Draw();

	glutSwapBuffers(); 	// sawp buffers called because we are using double buffering 
}

int main(int argc, char* argv[])
{
	/* Initialize GLUT */
	glutInit(&argc, argv);

	/* Init The Window */
	Manager.initwindow(SCREEN_WIDTH, SCREEN_HEIGHT);

	/* Init ImGui */
	Manager.initImGuiWindow();

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}

	/* Initialize all the functions */
	Manager.init();

	glutDisplayFunc(render);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(MouseInput);
	glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses  
	glutReshapeFunc(WindowResize);

	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}
#endif