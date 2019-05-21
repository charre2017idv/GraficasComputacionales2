#pragma once
#include "CMesh.h"

#if defined(DIRECTX)

CMesh::CMesh(D3D11_USAGE Usage, SimpleVertex* VertexArray, WORD* IndexArray, int numRealVertex, int numIndex, D3D11_BIND_FLAG BindFlags, int CPUAccesFlags, int Offset)
{
	// Vertex buffer
	VertexBuffer = CBuffer(Usage, VertexArray, numRealVertex, BindFlags, CPUAccesFlags, Offset);
	IndexBuffer = CBuffer(Usage, IndexArray, numIndex, BindFlags, CPUAccesFlags);
	m_numIndex = numIndex;
}

CMesh::CMesh(int numRealVertex, int numIndex, SimpleVertex* VertexArray, WORD* IndexArray)
{
	m_indices = new WORD[numIndex];
	m_vertices = new SimpleVertex[numRealVertex];
	for (int i = 0; i < numIndex; i++)
	{
		m_indices[i] = IndexArray[i];
	}
	for (int i = 0; i < numRealVertex; i++)
	{
		m_vertices[i] = VertexArray[i];
	}
	VertexBuffer = CBuffer(numRealVertex, m_vertices);
	IndexBuffer = CBuffer(numIndex, m_indices);
	
	m_numIndex = numIndex;
	ChangeEveryFrame = CBuffer(sizeof(CBChangesEveryFrame));
	
}


CMesh::~CMesh()
{
	
}

void CMesh::loadMeshText(ID3D11Device* m_pd3dDevice)
{
	wchar_t wText[50];
	char *token;
	char *nextToken;
	token = (char *)textName.c_str();
	mbstowcs(wText, token, strlen(token) + 1);
	dirText = (LPCWSTR)wText;

	D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, dirText, NULL, NULL, &TextureMesh.m_pTextureRV, NULL);
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, TM.SciHelmetTex, NULL, NULL, &TextureMesh_2.m_pTextureRV, NULL);
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, L"CoC_iOS_HER_PPA_SEN_B_baseColor.png", NULL, NULL, &TextureMesh.m_pTextureRV, NULL);
}


void CMesh::loadMeshText2(ID3D11Device* m_pd3dDevice)
{
	wchar_t wText[50];
	char *token;
	char *nextToken;
	token = (char *)textName.c_str();// Restar caracteres para agregar .jpg
	mbstowcs(wText, token, strlen(token) + 1);
	dirText = (LPCWSTR)wText;
	D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, TM.SceneTex, NULL, NULL, &TextureMesh.m_pTextureRV, NULL);
}

void CMesh::loadMeshText3(ID3D11Device* m_pd3dDevice)
{
	D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, TM.SMGTex, NULL, NULL, &TextureMesh.m_pTextureRV, NULL);
}

void CMesh::createMeshMatrix(float Time, float x, float y, float z, float ScaleX, float ScaleY, float ScaleZ)
{
	m_MeshMatrix.mWorld = XMMatrixIdentity();
	m_MeshMatrix.mWorld = XMMATRIX(
		ScaleX, 0.0f, 0.0f, x,
		0.0f, ScaleY, 0.0f, y,
		0.0f, 0.0f, ScaleZ, z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	m_MeshMatrix.mWorld *= XMMatrixRotationY(Time);
	
	//XMMatrixTranspose(m_MeshMatrix.mWorld);
}

void CMesh::setMaterial(XMFLOAT4 LD, XMFLOAT4 SP, XMFLOAT4 SC, XMFLOAT4 DC, XMFLOAT4 Eye, XMFLOAT4 kDiffuse, XMFLOAT4 kAmbient, XMFLOAT4 kSpecular)
{
	m_MeshMatrix.ligthDir = LD;
	m_MeshMatrix.SpecularPower = SP;
	m_MeshMatrix.SpecularColor = SC;
	m_MeshMatrix.difuseColor = DC;
	m_MeshMatrix.vViewPosition = Eye;
	//m_MeshMatrix.kDiffuse = kDiffuse;
	//m_MeshMatrix.kAmbient = kAmbient;
	//m_MeshMatrix.kSpecular = kSpecular;
}

XMMATRIX CMesh::setMatrix()
{

	return m_MeshMatrix.mWorld;
}

#elif defined(OPENGL)
CMesh::CMesh()
{
}


CMesh::~CMesh()
{
}

void CMesh::LoadMeshV2(int numVertex, int numIndex, aiVector3D* VertexArray, aiVector3D* textCoord, std::vector<std::uint32_t>& IndexArray, aiVector3D* Normals)
{
	m_tris = numVertex;
	m_indices = numIndex;
	buffersito = new simpleVertex[m_tris * 3];

	for (size_t i = 0; i < m_tris; i++)
	{
		Vertices.x = VertexArray[i].x;
		Vertices.z = VertexArray[i].z;
		Vertices.y = VertexArray[i].y;

		temp_Vertex.push_back(Vertices);

		Normal.x = Normals[i].x;
		Normal.z = Normals[i].z;
		Normal.y = Normals[i].y;

		temp_Normals.push_back(Normal);

		uv.x = textCoord[i].x;
		uv.y = -textCoord[i].y;
		tem_uvs.push_back(uv);
	}
	for (size_t i = 0; i < m_indices; i++)
	{
		VertexIndex.push_back(IndexArray[i]);

	}
	for (int i = 0; i < VertexIndex.size(); i++)
	{
		unsigned int vIndex = VertexIndex[i];
		glmVertices.push_back(temp_Vertex[vIndex]);
		buffersito[i].position = temp_Vertex[vIndex];
		buffersito[i].texCoord = glm::vec2(tem_uvs[vIndex].x, tem_uvs[vIndex].y);

		glmNormals.push_back(temp_Normals[vIndex]);
		buffersito[i].normals = temp_Normals[vIndex];
		//buffersito[i].normals = glm::vec3(temp_Normals[vIndex].x, temp_Normals[vIndex].y, temp_Normals[vIndex].z);
		//buffersito[i].normals = temp_Normals[vIndex];
	}

	glGenBuffers(1, &buuferID);

	glBindBuffer(GL_ARRAY_BUFFER, buuferID);

	glBufferData(GL_ARRAY_BUFFER, VertexIndex.size() * sizeof(simpleVertex), &buffersito[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, buuferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), 0);

	glBindBuffer(GL_ARRAY_BUFFER, buuferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, buuferID);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), (unsigned char*)NULL + (5 * sizeof(float)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	temp_Vertex.clear();
	tem_uvs.clear();
	VertexIndex.clear();
	glmVertices.clear();
}

void CMesh::loadTexture(GLuint TextureID)
{
	char* token = NULL;
	char* nextToken = NULL;
	token = strtok_s((char*)texname.c_str(), ".", &nextToken);
	string pngextencion = token;
	pngextencion += ".png";

	int m_widht, m_height;
	unsigned char * data = SOIL_load_image(pngextencion.c_str(), &m_widht, &m_height, 0, SOIL_LOAD_RGBA);

	glGenTextures(1, &textID);
	glBindTexture(GL_TEXTURE_2D, textID);

	//up settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Maximisize
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Minimisize
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Not extend the area
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Not Tiling the area

	 //to OpenGL the data of local buffer
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_widht, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	//glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(data);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind or setting to default
}


void CMesh::createMeshMatrix(float Timer, float x, float y, float z, float ScaleX, float ScaleY, float ScaleZ)
{
	MeshMatrix = glm::mat4(1.0f); // Load Identitytrans = glm::rotate(
	translation = glm::vec3(x, y, z);
	MeshMatrix = glm::translate(glm::mat4(1.0f), translation);
	newScale = glm::vec3(ScaleX, ScaleY, ScaleY);
	//glm::vec3 scale = glm::vec3(0.1f, 0.1f, 0.1f);
	MeshMatrix = glm::scale(MeshMatrix, newScale);
	MeshMatrix = glm::rotate(MeshMatrix, Timer * glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 CMesh::setMatrix()
{
	officialMatrix = MeshMatrix;
	return officialMatrix;
}

#endif