// Tessilation final project
// Name: Wesley Strong
// NetID: wfs51
#include "../stdafx.h"
#include "shader.h"

// Default constructer
Shader::Shader() {}

// For creating all shaders including tesselation shaders
Shader::Shader(string vertFile, string fragFile, string tessContrlFile, string tessEvalFile, string geomFile)
{
	// Create the vertex shader
	CreateShader(vertFile, GL_VERTEX_SHADER);
	// Create the fragment shader
	CreateShader(fragFile, GL_FRAGMENT_SHADER);
	// Create the geometry shader
	//CreateShader(geomFile, GL_GEOMETRY_SHADER);
	// Create the tessilation controller shader
	CreateShader(tessContrlFile, GL_TESS_CONTROL_SHADER);
	// Create the tessilation evaluation shader 
	CreateShader(tessEvalFile, GL_TESS_EVALUATION_SHADER);
	// Create the program and store its ID
	programID = glCreateProgram();
	// Attach the shaders to the program
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragShaderID);
	//glAttachShader(programID, geometyShaderID);
	glAttachShader(programID, tessControlShaderID);
	glAttachShader(programID, tessEvalShaderID);
	// Link the program
	glLinkProgram(programID);
	glUseProgram(programID);
	
}

// Load shader from file location and set as type
void Shader::CreateShader(string file, GLenum type)
{
	GLint shaderID;
	ifstream shaderFile(file);
	shaderFile.seekg(0, ios_base::beg);
	ifstream::pos_type begin = shaderFile.tellg();
	shaderFile.seekg(0, ios_base::end);
	int size = 1 + static_cast<int>(shaderFile.tellg() - begin);
	GLchar *ShaderTmp = new GLchar[size];
	shaderFile.seekg(0, ios_base::beg);
	shaderFile.read(ShaderTmp, size - 1);
	ShaderTmp[size - 1] = '\0';
	shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, (const GLchar**)&ShaderTmp, NULL);
	glCompileShader(vertexShaderID);
	shaderFile.close();
	delete[] ShaderTmp;

	// Store shader ID of matching type
	switch(type) {
		case GL_VERTEX_SHADER:
			vertexShaderID = shaderID;
			break;
		case GL_FRAGMENT_SHADER:
			fragShaderID = shaderID;
			break;
		case GL_TESS_CONTROL_SHADER:
			tessControlShaderID = shaderID;
			break;
		case GL_TESS_EVALUATION_SHADER:
			tessEvalShaderID = shaderID;
			break;
		case GL_GEOMETRY_SHADER:
			geometyShaderID = shaderID;
			break;
	}
}

void Shader::DeleteShaders()
{
	glUseProgram(0);

	// Detach the shader's
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragShaderID);
	glDetachShader(programID, geometyShaderID);

	// Destroy the shader's
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragShaderID);
	glDeleteShader(geometyShaderID);

	// Destroy the program
	glDeleteProgram(programID);
}

// Operator overloater for printing debugging data from compiling shaders
ostream& operator<<(ostream& stream, Shader& shader)
{
	GLsizei length;
	char log[1024];
	// Vertex log
	glGetShaderInfoLog(shader.vertexShaderID, 1023, &length, log);
	stream << "Vertex Log: " << log << endl;
	memset(log, 0, sizeof(log));
	// Fragment log
	glGetShaderInfoLog(shader.fragShaderID, 1023, &length, log);
	stream << "Fragment Log: " << log << endl;
	memset(log, 0, sizeof(log));
	// Gemoetry log
	glGetShaderInfoLog(shader.geometyShaderID, 1023, &length, log);
	stream << "Geometry Log: " << log << endl;
	memset(log, 0, sizeof(log));
	// Tessilation controller log
	glGetShaderInfoLog(shader.tessControlShaderID, 1023, &length, log);
	stream << "TessCtrl Log: " << log << endl;
	memset(log, 0, sizeof(log));
	// Tessilations evaluation log
	glGetShaderInfoLog(shader.tessEvalShaderID, 1023, &length, log);
	stream << "TessEval Log: " << log << endl;
	memset(log, 0, sizeof(log));

	return stream;
}