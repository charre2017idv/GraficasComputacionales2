#pragma once
#include "Header.h"
//--------------------------------------------------------------------------------------
// File: CCam.h
// Version: v 10.0
// Creation Date: 09/04/2019
// Author: Roberto Charreton Kaplun
// Summary: 
// Errors: 
// Bugs: 
// Nice to Have: 
//--------------------------------------------------------------------------------------
#if defined(DIRECTX)


class CCam
{
public:
	CCam();
	~CCam();
public:
	// Properties of the cam matrix
	XMVECTOR							Eye;
	XMVECTOR							At;
	XMVECTOR							Up;
	// Properties of the cam
	XMMATRIX                            g_Projection;
	XMMATRIX                            g_View;
	XMMATRIX                            g_ViewTop;
	XMMATRIX                            g_World;
	// Positions of the mouse in the cam
	FLOAT3								VEye;
	FLOAT3								VAt;
	FLOAT3								VUp;
	POINT								LastPos;
	POINT								ActualPos;

	CBNeverChanges						cbNeverChanges;
	CBChangeOnResize					cbChangesOnResize;
	CBChangesEveryFrame					CameraMatrix;
	bool								isClicked = false;
public:
	void getEye(float x, float y, float z, float w);
	void getUp(float x, float y, float z, float w);
	void getAt(float x, float y, float z, float w);
	void setCameraLookAtLH(
		float Eye_x, float Eye_y, float Eye_z, float Eye_w,
		float Up_x, float Up_y, float Up_z, float Up_w,
		float At_x, float At_y, float At_z, float At_w
	);
	void createView();
	void createNeverChangeData();
	void createProjection(float apertura, float width, float height, float near, float Far);

	void createMeshMatrix();
	XMMATRIX GetMatrix();
public:
};

#elif defined(OPENGL)

class CCam
{
public:
	CCam();
	~CCam();
public:
	void setEye(float x, float y, float z);
	void setAt(float x, float y, float z);
	void setUp(float x, float y, float z);

	void setView();
	void setProj(float width, float height);
	void setMVP(glm::mat4 world);
	void setRotation();

public:
	// Model Properties
	// Camera
	glm::mat4							m_MVP;
	glm::mat4							m_View;
	glm::mat4							m_World;
	glm::mat4							m_Projection;

	glm::vec3							Eye;
	glm::vec3							At;
	glm::vec3							Up;

	glm::vec3							Front;
	glm::vec3							Right;
	glm::vec3							up;

	// Cursor
	POINT								ActualCursor;
	POINT								PastCursor;
	bool								isPress = false;
	
	// Trasform
	float								m_x = 0;
	float								m_y = 0;
	float								m_z = 0;
	// Scale
	glm::vec3							scale;
	float								scala = 1.0f;
	float								m_Scalex = 0.01f;
	float								m_Scaley = 0.01f;
	float								m_Scalez = 0.01f;
	// Projection Properties
	float								m_timer;
	float								m_FOV = 39.0f;
	float								m_rotateY = 1.0f;
	float								m_near = -1.0f;
	float								m_far = 1.0f;

};



#endif