#pragma once
//--------------------------------------------------------------------------------------
// File: Header.h
// Version: v 10.0
// Creation Date: 09/04/2019
// Author: Roberto Charreton Kaplun
// Summary: This is the resource document that gives the correct data between DirectX and OpengGL
// Errors: 
// Bugs: 
// Nice to Have: Combine structs on OpenGl and DirectX
//--------------------------------------------------------------------------------------

// <! Warning : This is important, uncomment the #define of the framework that you want to use >

#define DIRECTX 
//#define OPENGL

//--------------------------------------------------------------------------------------
// GLOBAL MACROS, HEADERS AND INCLUDES
#define SCREEN_WIDTH  640 // 500
#define SCREEN_HEIGHT 480 // 400
/* Assimp Files */
#include <assimp/scene.h>
#include <assimp/ai_assert.h>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
/* Visual studio files */
#include <windows.h>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
//--------------------------------------------------------------------------------------
// GLOBAL STRUCTS AND RESOURCES
struct TextAndMesh
{
	// Dwarf Proyect
	const char* DwarfMesh = "Dwarf/Dwarf/dwarf.x";

	// SpiderMan HomeComing
	const char* HomecomingMesh = "Dwarf/source/home.fbx";
	LPCWSTR HomecomingText = L"CoC_iOS_HER_PPA_SEN_B_baseColor.png";

	// Iron Man Halmet 
	const char* IronHalmet = "Dwarf/Zelda/Iron_Man_Helmet.obj";
	LPCWSTR IronHalmetTex = L"ALBEDO.jpg";

	// Bocoblin <! ERROR >
	const char* BocoblinMesh = "Dwarf/Bocoblin/Bokoblin.OBJ";

	// Sci - fi Helmet
	const char* SciHeletMesh = "Dwarf/Spacehalmet/Helmet_02.obj";
	LPCWSTR SciHelmetTex = L"ALD_02.tga.png";

	// Tokioo
	const char* tokioMesh = "Dwarf/tokio/Export.fbx";
	LPCWSTR tokioTex = L"Atlas.jpg";

	// Tokyo
	const char* tokyoMesh = "Dwarf/tokyo/Sm_TokyoStores-Lowpoly[5].fbx";
	LPCWSTR tokyoTex = L"DefaultMaterial_Base_Color.png";

	// Arms
	const char* ArmsMesh = "Dwarf/Arms/arms.fbx";
	LPCWSTR ArmsTex = L"arm1Color.png";
	
	const char* ArmsRMesh = "Dwarf/Arms/armsrifle1.fbx";
	LPCWSTR ArmsRTex = L"rifle1Color.png";

	// FPS
	const char* FPSMesh = "Dwarf/Arms/FPS.fbx";
	// FPS 2	
	const char* FPS2Mesh = "Dwarf/Arms/FPS2.obj";
	LPCWSTR FPSTex = L"01_-_Default_Base_Color.png";
	// SMG <! Offical FPS>
	const char* SMGMesh = "Dwarf/Arms/armssmg.fbx";
	LPCWSTR SMGTex = L"raptorColor.png";

	// Square
	const char* squareMesh = "Dwarf/Plane/PlaneToOpenGL.fbx";
	LPCWSTR squareTex = L"photo.jpg";

};
struct FLOAT3
{
	float x, y, z;
};
struct FLOAT2
{
	float x, y;
};
#if defined(DIRECTX)
// DirectX Files
#include "resource.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <d3dcompiler.h>
/* ImGui Files */
#include "ImGUI/ImGuiDirectX/imgui.h"
#include "ImGUI/ImGuiDirectX/imgui_impl_dx11.h"
#include "ImGUI/ImGuiDirectX/imgui_impl_win32.h"
using namespace std;

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
	XMFLOAT4 kDiffuse;
	XMFLOAT4 kAmbient;
	XMFLOAT4 kSpecular;
};


//--------------------------------------------------------------------------------------
#elif defined(OPENGL)
//--------------------------------------------------------------------------------------
// <! Warning : This only works in OpenGL >
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
x;\
ASSERT(GLLogCall(#x,__FILE__,__LINE__))
//--------------------------------------------------------------------------------------
/* Glew Files */
#include <GL/glew.h>
/* Glut Files */
#include <GL/freeglut.h>
/* GLM Files */
#include "glm-0.9.3.3/glm/glm.hpp"
#include "glm-0.9.3.3/glm/gtc/matrix_transform.hpp"
/* SOIL Files */
#include "SOIL.h"
/* ImGui Files */
#include "ImGUI/ImGuiOpenGL/imgui.h"
#include "ImGUI/ImGuiOpenGL/imgui_impl_win32.h"
#include "ImGUI/ImGuiOpenGL/imgui_impl_opengl3.h"
using namespace std;
/* Mesh struct */
struct Vector3
{
	FLOAT3 Pos;
	FLOAT2 Tex;
};
/* Shader Recognition Struct */
struct ShaderSources
{
	string VertexSource;
	string PixelSource;
};

// Error Checker
static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError()
{
	stringstream sstream;

	while (GLenum error = glGetError())
	{
		sstream << std::hex << (int)error;
		std::string result = sstream.str();
		cout << "(!) OpenGL Error ( 0x" << result << " ) of glew.h" << endl;
	}
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	stringstream sstream;

	while (GLenum error = glGetError())
	{
		sstream << std::hex << (int)error;
		std::string result = sstream.str();
		cout << "(!) OpenGL Error ( 0x" << result << " ) : " << function << " " << file << " : " << line << endl;
		return false;
	}
	return true;
}

#endif
