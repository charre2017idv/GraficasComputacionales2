#pragma once
#include "CCam.h"

#if defined(DIRECTX)
CCam::CCam()
{
}

CCam::~CCam()
{
}

void CCam::getEye(float x, float y, float z, float w)
{	
	Eye = XMVectorSet(x, y, z, w);
	
}

void CCam::getUp(float x, float y, float z, float w)
{
	Up = XMVectorSet(x, y, z, w);
}

void CCam::getAt(float x, float y, float z, float w)
{
	At = XMVectorSet(x, y, z, w);
}

void CCam::setCameraLookAtLH(
	float Eye_x, float Eye_y, float Eye_z, float Eye_w,
	float Up_x, float Up_y, float Up_z, float Up_w,
	float At_x, float At_y, float At_z, float At_w)
{
	getEye(Eye_x, Eye_y, Eye_z, Eye_w);
	getUp(Up_x, Up_y, Up_z, Up_w);
	getAt(At_x, At_y, At_z, At_w);
}

void CCam::createView()
{
	g_View = XMMatrixLookAtLH(Eye, At, Up);
	createNeverChangeData();
	
}

void CCam::createNeverChangeData()
{
	cbNeverChanges.mView = g_View;
	cbNeverChanges.mView = XMMatrixTranspose(cbNeverChanges.mView);
}

void CCam::createProjection(float apertura, float width, float height, float Near, float Far)
{
	g_Projection = XMMatrixPerspectiveFovLH(apertura, width / height, Near, Far);
	//cbChangesOnResize.mProjection = g_Projection;
	cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
}

void CCam::createMeshMatrix()
{
	CameraMatrix.mWorld = XMMatrixIdentity();
	//CameraMatrix.mWorld = g_View;
	CameraMatrix.mWorld = XMMATRIX(
		.03f, 0.0f, 0.0f, VEye.x + .7,
		0.0f, .03f, 0.0f, VEye.y - 1,
		0.0f, 0.0f, .03f, VEye.z + .8,
		0.0f, 0.0f, 0.0f, 1.0f
	);

}

#elif defined(OPENGL)

CCam::CCam()
{
}


CCam::~CCam()
{
}


void CCam::setEye(float x, float y, float z)
{
	Eye = glm::vec3(x, y, z);
}

void CCam::setAt(float x, float y, float z)
{
	At = glm::vec3(x, y, z);
}

void CCam::setUp(float x, float y, float z)
{
	Up = glm::vec3(x, y, z);
}

void CCam::setView()
{
	Front = glm::normalize(Eye - At);
	Right = glm::normalize(glm::cross(Up, Front));
	up = glm::cross(Front, Right); // Up Verdadero

	// Create View Matriz
	m_View = glm::mat4(1.0f); // Load Identity

	m_View = glm::lookAt(
		Eye,	// Camera Position
		At,		// Where the camera looks
		up		// This is another way to say camera is not rotated 
	);
	//m_View = glm::rotate(m_View, m_timer * glm::radians(180.0f), glm::vec3(m_rotateY, 0.0f, 0.0f));
}

void CCam::setProj(float width, float height)
{
	m_Projection = glm::mat4(1.0f); // Load Identity
	m_Projection *= glm::perspective(glm::radians(m_FOV), width / height, m_near, m_far);
	//m_Projection *= glm::perspectiveFov(m_FOV,width,height,m_near,m_far);
}



void CCam::setMVP(glm::mat4 world)
{	
	m_MVP = m_Projection * m_View * world;
}

void CCam::setRotation()
{
	GetCursorPos(&ActualCursor);
	// Xs
	if (PastCursor.x < ActualCursor.x)
		At -= Right * 0.01f;
	else if (PastCursor.x > ActualCursor.x)
		At += Right * 0.01f;
	// Ys
	if (PastCursor.y < ActualCursor.y)
		At -= Up * 0.01f;
	else if (PastCursor.y > ActualCursor.y)
		At += Up * 0.01f;
	SetCursorPos(PastCursor.x, PastCursor.y);
}
#endif