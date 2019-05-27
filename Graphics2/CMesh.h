#pragma once
#include "Header.h"
#if defined(DIRECTX)
#include "CBuffer.h"
#include "CTexture2D.h"
class CMesh
{
public:
	CBuffer						VertexBuffer;
	CBuffer						IndexBuffer;
	CBuffer						ChangeEveryFrame;
	SimpleVertex*				m_vertices;
	WORD*						m_indices;
	int							m_numIndex;

	CTexture2D					TextureMesh;
	CTexture2D					TextureMesh_2;
	string						textName;
	LPCWSTR						dirText;

	TextAndMesh					TM;
	CBChangesEveryFrame			m_object;
	CBChangesEveryFrame			m_object2;
	CBChangesEveryFrame			m_MeshMatrix;
public:
	CMesh() = default;
	// Complete Mesh properties
	CMesh(D3D11_USAGE Usage, SimpleVertex* VertexArray, WORD* IndexArray, int numRealVertex, int numIndex, D3D11_BIND_FLAG BindFlags, int CPUAccesFlags, int Offset);
	// Simple Mesh Properties (Useful for setting the complete data)
	CMesh(int numRealVertex, int numIndex, SimpleVertex* VertexArray, WORD* IndexArray);
	// Assimp Constructor
	
	~CMesh();
public:


	void loadMeshText(ID3D11Device* m_pd3dDevice);
	void loadMeshText2(ID3D11Device* m_pd3dDevice);
	void loadMeshText3(ID3D11Device* m_pd3dDevice);
	void createMeshMatrix(float , float , float , float , float , float , float );
	void setMaterial(XMFLOAT4 LD, XMFLOAT4 SP, XMFLOAT4 SC, XMFLOAT4 DC, XMFLOAT4 Eye, XMFLOAT4 kDiffuse, XMFLOAT4 kAmbient, XMFLOAT4 kSpecular);
	XMMATRIX setMatrix();
};

#elif defined(OPENGL)
#pragma once
//#include "Config.h"
//#include "CVertexBuffer.h"
class CMesh
{
public:
	CMesh();
	~CMesh();
public:
	struct simpleVertex
	{
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normals;
	};
	simpleVertex *				buffersito;

	GLuint						buuferID;
	GLuint						textID;
	GLuint						vertexArrayID;

	glm::vec3					Vertices;
	vector <glm::vec3>			temp_Vertex;
	vector<unsigned int>		VertexIndex;
	glm::vec2					uv;
	vector<glm::vec2>			tem_uvs;
	vector<glm::vec3>			glmVertices;
	
	glm::vec3					Normal;
	vector <glm::vec3>			temp_Normals;
	vector<glm::vec3>			glmNormals;
	
	glm::mat4					MeshMatrix;
	glm::vec3					newScale;
	glm::vec3					translation;
	glm::mat4					officialMatrix;
	
	int							m_tris;
	int							m_indices;
	string						texname;
	// Scale Model
	float						m_Scalex = 0.01f;
	float						m_Scaley = 0.01f;
	float						m_Scalez = 0.01f;
	// Trasform
	float						m_x = 2.0f;
	float						m_y = 0;
	float						m_z = 0;
	float						m_timer;

public:
	void LoadMeshV2(int numVertex, int numIndex, aiVector3D* VertexArray, aiVector3D* textCoord, std::vector<std::uint32_t>& IndexArray, aiVector3D* Normals);
	void loadTexture(GLuint TextureID);
	void createMeshMatrix(float Iden, float x, float y, float z, float ScaleX, float ScaleY, float ScaleZ);
	glm::mat4 setMatrix();
};


#endif