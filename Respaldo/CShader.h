#pragma once
#include "Header.h"
#if defined(DIRECTX)
#elif defined(OPENGL)

class CShader
{
public:
	CShader(const string &file);
	~CShader();
private:
	string m_path;
	// Catch the uniforms
	unordered_map <string, int> m_uniformLocationCatche;
public:
	unsigned int m_handle_ID;
	void Bind() const;
	void UnBind() const;

public:
	// Set Uniforms
	void setUniform1i(const string &name, int value); // Texture
	void setUniform4f(const string &name, float x, float y, float z, float w); // Albedo
	void setUniformMat4f(const string &name, const glm::mat4 matrix4); // Albedo

private:
	ShaderSources parseShader(const string & file);
	unsigned int compileShader(unsigned int type, const string & source);
	unsigned int createShader(const string &vertexShader, const string &pixelShader);

	int GetUniformLocatiiion(const string &name);

};
#endif