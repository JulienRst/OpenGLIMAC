#version 330 core

uniform mat3 uModelMatrix;

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexturePosition;

out vec2 vFragTextCoord;

void main() {
	vFragTextCoord = aVertexTexturePosition;
	gl_Position = vec4(uModelMatrix * vec3(aVertexPosition,1), 1);
};