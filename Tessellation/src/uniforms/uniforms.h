// Tessilation final project
// Name: Wesley Strong
// NetID: wfs51
#pragma once
using namespace std;

// Unifrom class for easy managment of uniforms
// Stores uniforms names, ids and types in a map for easy lookup
class Uniforms
{
public:
	// Default constructer
	Uniforms();
	// Set program ID
	Uniforms(GLint programID);
	// Add a matrix uniform of GLenum type
	void AddUniform(string name, GLenum type, const GLfloat* value);
	// Set boolean unifrom
	void AddUniform(string name, GLboolean value);
	// Set float unifrom
	void AddUniform(string name, GLfloat value);
	// Set integer unifrom
	void AddUniform(string name, GLint value);
	// Bind attribute type
	void AddAttribute(string name, GLint pos);
	// Update matrix unifrom of GLenum type
	void UpdateUniform(string name, const GLfloat* value);
	// Update boolean unifrom
	void UpdateUniform(string name, GLboolean value);
	// Update float unifrom
	void UpdateUniform(string name, GLfloat value);
	// Update integer unifrom
	void UpdateUniform(string name, GLint value);

private:
	// Get unifrom location
	GLint GetUniformLocation(GLint programID, string name);
	// Stores the unifroms id and type
	struct UniformEntry
	{
		GLint id;
		GLenum type;
	};
	// Store program ID for convenit access
	GLint programID;
	// Map of unifrom names, id and type for easy lookup
	map<string, UniformEntry> uniforms;
};
