#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glimac/FilePath.hpp>

#include <GL/glew.h> // Include glew to get all the required OpenGL headers


class Shader
{
public:
  	// The program ID
	GLuint Program;
	// Constructor reads and builds the shader
	Shader(const glimac::FilePath vertexPath, const glimac::FilePath fragmentPath);

  	Shader(GLenum type): Program(glCreateShader(type)) {
	}

	~Shader() {
		glDeleteShader(Program);
	}

	Shader(Shader&& rvalue): Program(rvalue.Program) {
		rvalue.Program = 0;
	}

	Shader& operator =(Shader&& rvalue) {
		Program = rvalue.Program;
		rvalue.Program = 0;
		return *this;
	}

	// Use the program
  	void Use();

	GLuint getGLId() const {
		return Program;
	}

	void setSource(const char* src) {
		glShaderSource(Program, 1, &src, 0);
	}

	bool compile();

	const std::string getInfoLog() const;
};

// Load a shader (but does not compile it)
Shader loadShader(GLenum type, const glimac::FilePath& filepath);
