// Tessilation final project
// Name: Wesley Strong
// NetID: wfs51
#include "../stdafx.h"
#include "uniforms.h"

Uniforms::Uniforms() {}

Uniforms::Uniforms(GLint programID) { this->programID = programID; }

// Set a matrix uniform of type GLenum
// Currentley only works for GL_FLOAT_MAT4, GL_FLOAT_MAT3 and GL_FLOAT_VEC3.
// Insersts the new uniform into the map for easy lookup later
void  Uniforms::AddUniform(string name, GLenum type, const GLfloat* value) {
	UniformEntry id_type;
	id_type.id = GetUniformLocation(programID, name.c_str());

	switch (type){
		case GL_FLOAT_MAT4:
		{
			glUniformMatrix4fv(id_type.id, 1, GL_FALSE, value);
			id_type.type = GL_FLOAT_MAT4;
			break;
		}
		case GL_FLOAT_MAT3:
		{
			glUniformMatrix3fv(id_type.id, 1, GL_FALSE, value);
			id_type.type = GL_FLOAT_MAT3;
			break;
		}
		case GL_FLOAT_VEC3:
		{
			glUniform3fv(id_type.id, 1, value);
			id_type.type = GL_FLOAT_VEC3;
			break;
		}
	}
	uniforms.insert(pair<string, UniformEntry>(name, id_type));
}

// Set and bind a GLboolean unifrom
// Insersts the new uniform into the map for easy lookup later
void  Uniforms::AddUniform(string name, GLboolean value) {
	UniformEntry id_type;
	id_type.id = GetUniformLocation(programID, name.c_str());
	glUniform1f(id_type.id, value);
	uniforms.insert(pair<string, UniformEntry>(name, id_type));
}

// Set and bind a GLfloat unifrom
// Insersts the new uniform into the map for easy lookup later
void  Uniforms::AddUniform(string name, GLfloat value) {
	UniformEntry id_type;
	id_type.id = GetUniformLocation(programID, name.c_str());
	glUniform1f(id_type.id, value);
	uniforms.insert(pair<string, UniformEntry>(name, id_type));
}

// Set and bind a GLint unifrom
// Insersts the new uniform into the map for easy lookup later
void  Uniforms::AddUniform(string name, GLint value) {
	UniformEntry id_type;
	id_type.id = GetUniformLocation(programID, name.c_str());
	glUniform1i(id_type.id, value);
	uniforms.insert(pair<string, UniformEntry>(name, id_type));
}

// Set an attribute by name and set position at pos
void  Uniforms::AddAttribute(string name, GLint pos) {
	glBindAttribLocation(programID, pos, name.c_str());
}

// Looks up matrix unifrom of type GL_FLOAT_MAT4 or GL_FLOAT_MAT3
// and updates the unifrom
void  Uniforms::UpdateUniform(string name, const GLfloat* value) {
	UniformEntry id_type = uniforms.find(name)->second;
	switch (id_type.type) {
		case GL_FLOAT_MAT4:
		{
			glUniformMatrix4fv(id_type.id, 1, GL_FALSE, value);
			id_type.type = GL_FLOAT_MAT4;
			break;
		}
		case GL_FLOAT_MAT3:
		{
			glUniformMatrix3fv(id_type.id, 1, GL_FALSE, value);
			id_type.type = GL_FLOAT_MAT3;
			break;
		}
	}
}

// Update the name unifrom of type boolean
void  Uniforms::UpdateUniform(string name, GLboolean value) {
	UniformEntry id_type = uniforms.find(name)->second;
	glUniform1f(id_type.id, value);
}

// Update the name unifrom of type GLfloat
void  Uniforms::UpdateUniform(string name, GLfloat value) {
	UniformEntry id_type = uniforms.find(name)->second;
	glUniform1f(id_type.id, value);
}

// Update the name unifrom of type GLint
void  Uniforms::UpdateUniform(string name, GLint value) {
	UniformEntry id_type = uniforms.find(name)->second;
	glUniform1i(id_type.id, value);
}

// Returns the unifrom location id
GLint Uniforms::GetUniformLocation(GLint programID, string name) {
	return glGetUniformLocation(programID, name.c_str());
}