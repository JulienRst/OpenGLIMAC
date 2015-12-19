#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

out vec3 fFragColor;


vec3 blinnPhong(){
    vec3 Li = uLightIntensity;
    vec3 Wi = normalize(uLightDir_vs);
    vec3 Wo = normalize(- vPosition_vs);
    vec3 halfVector = (Wo + Wi) / 2;
    vec3 N = normalize(vNormal_vs);

    return uLightIntensity * (uKd * max(0,dot(Wi,N))) + uKs * pow(dot(halfVector,N),uShininess);
}

void main(){
    fFragColor = blinnPhong();
}
