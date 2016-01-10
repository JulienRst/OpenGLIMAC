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
  	// Use the program
  	void Use();
};
