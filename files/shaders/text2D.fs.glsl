#version 330 core

uniform sampler2D uTexture;

in vec2 vFragTextCoord;
out vec3 fFragColor;

void main() {
	vec4 c = texture(uTexture,vFragTextCoord);
	fFragColor = vec3(c.r,c.g,c.b);
 // fFragColor = texture(uTexture,vFragTexturePosition).rgb;
};