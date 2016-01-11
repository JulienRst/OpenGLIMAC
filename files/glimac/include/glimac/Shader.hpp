#pragma once

#include <GL/glew.h>
#include <string>
#include "FilePath.hpp"

#define GLIMAC_SHADER_SRC(str) #str

namespace glimac {

class GLShader {
public:
	GLShader(GLenum type): m_nGLId(glCreateShader(type)) {
	}

	~GLShader() {
		glDeleteShader(m_nGLId);
	}

	GLShader(GLShader&& rvalue): m_nGLId(rvalue.m_nGLId) {
		rvalue.m_nGLId = 0;
	}

	GLShader& operator =(GLShader&& rvalue) {
		m_nGLId = rvalue.m_nGLId;
		rvalue.m_nGLId = 0;
		return *this;
	}

	GLuint getGLId() const {
		return m_nGLId;
	}

	void setSource(const char* src) {
		glShaderSource(m_nGLId, 1, &src, 0);
	}

	bool compile();

	const std::string getInfoLog() const;

private:
	GLShader(const GLShader&);
	GLShader& operator =(const GLShader&);

	GLuint m_nGLId;
};

// Load a GLShader (but does not compile it)
GLShader loadGLShader(GLenum type, const FilePath& filepath);

}
