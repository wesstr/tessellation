// Tessilation final project
// Name: Wesley Strong
// NetID: wfs51
#pragma once
using std::string;
using std::ostream;
using std::endl;
using std::ifstream;
using std::ios_base;

// Class for easy managment of shaders
// Currently only works for a single progrm and
// creating the following shaders: vertex, tessilation controller, tessilation evaluator
// geometry and fragment 
class Shader
{
public:
	// Default constructer
	Shader();
	// Creates the following staders from file locations: vertex, fragment, tessilation controller, 
	// tessilation evaluator and geometry.
	Shader(string vertFile, string fragFile, string tessContrlFile, string tessEvalFile, string geomFile);
	// Unlink and destroy shaders
	void DeleteShaders();
	// Store shaders IDs
	GLuint vertexShaderID,
		fragShaderID,
		tessControlShaderID,
		tessEvalShaderID,
		geometyShaderID,
		programID;

private:
	// Creats a shader from file of shader type
	void CreateShader(string file, GLenum);
};
// For printing debug statments from compiling shaders
ostream& operator<<(ostream& stream, Shader& shader);