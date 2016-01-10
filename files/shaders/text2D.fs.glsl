#version 330 core

in vec2 vFragTextCoord;

uniform sampler2D uTexture;

out vec3 fFragColor;

void main() {
	fFragColor = texture(uTexture,vFragTextCoord).rgb;
};
