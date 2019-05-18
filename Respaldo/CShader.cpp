#include "CShader.h"

#if defined(DIRECTX)

#elif defined(OPENGL)

CShader::CShader(const string & file) : m_path(file), m_handle_ID(0)
{
	ShaderSources source = parseShader(file);
	m_handle_ID = createShader(source.VertexSource, source.PixelSource);
}

CShader::~CShader()
{
	GLCall(glDeleteProgram(m_handle_ID));
}

ShaderSources CShader::parseShader(const string & file)
{
	//ifstream Fichero(file.c_str());
	ifstream Fichero;
	string path = "Shader.fx";
	Fichero.open(file);
	enum class shaderType
	{
		NONE = -1,
		VERTEX = 0,
		PIXEL = 1
	};


	string line;
	stringstream ss[2];		// [1] for vertex shader and [2] for pixel shader
	shaderType Type = shaderType::NONE;
	if (Fichero.is_open())
	{
		while (getline(Fichero, line))
		{

			if (line.find("#shader") != string::npos)
			{	// npos returns the position but means that the shader is not located
				if (line.find("vertex") != string::npos)
				{
					// Set mode to vertex
					Type = shaderType::VERTEX;
				}
				else if (line.find("pixel") != string::npos)
				{
					// Set mode to pixel
					Type = shaderType::PIXEL;
				}
			}
			else
			{
				ss[(int)Type] << line << '\n'; // Add line on the streamstring
			}
		}
	}
	else
	{
		cout << "Error! File not found!";
		exit(1);
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int CShader::compileShader(unsigned int type, const string & source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		cout << "(!) Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Pixel") << " shader." << endl;
		cout << message << endl;
		glDeleteShader(id);
		return 0;
	}
	// Error Handling

	return id;
}

unsigned int CShader::createShader(const string & vertexShader, const string & pixelShader)
{
	unsigned int program = glCreateProgram(); // This function return an unsigned int
	unsigned int VShader = compileShader(GL_VERTEX_SHADER, vertexShader);		// Asigns an id to the Vertex shader 
	unsigned int PShader = compileShader(GL_FRAGMENT_SHADER, pixelShader);		// Asigns an id to the Pixel shader

	glAttachShader(program, VShader);	// Attach the Vertex shader to the created program
	glAttachShader(program, PShader);	// Attach the Pixel shader to the creathed program

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(VShader); // Delete the Vertex Shader
	glDeleteShader(PShader); // Delete the Pixel Shader

	return program;
}

void CShader::Bind() const
{
	glUseProgram(m_handle_ID);
}

void CShader::UnBind() const
{
	glUseProgram(0);
}

void CShader::setUniform1i(const string & name, int value)
{
	GLCall(glUniform1i(GetUniformLocatiiion(name), value));
}

void CShader::setUniform4f(const string & name, float x, float y, float z, float w)
{
	GLCall(glUniform4f(GetUniformLocatiiion(name), x, y, z, w));
}

void CShader::setUniformMat4f(const string & name, const glm::mat4 matrix4)
{
	GLCall(glUniformMatrix4fv(GetUniformLocatiiion(name), 1, GL_FALSE, &matrix4[0][0]));
}


int CShader::GetUniformLocatiiion(const string & name)
{
	// This will help when we got more uniforms
	if (m_uniformLocationCatche.find(name) != m_uniformLocationCatche.end())
		return m_uniformLocationCatche[name];

	GLCall(int location = glGetUniformLocation(m_handle_ID, name.c_str()));
	if (location == -1) // El unifrom correspone al numero del registro
		cout << "(!) Warning: uniform '" << name << "' doesn't exist." << endl;

	m_uniformLocationCatche[name] = location;
	return location;
}

#endif
