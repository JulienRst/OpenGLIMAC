#pragma once
#define GLM_FORCE_RADIANS
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class FreeFlyCamera {
    private:
        vec3 m_Position;
        float m_fPhi;
        float m_fTheta;
        vec3 m_FrontVector;
        vec3 m_LeftVector;
        vec3 m_UpVector;

        void computeDirectionVectors();

    public:
        FreeFlyCamera();
        void moveLeft(float t);
        void moveFront(float t);
        void rotateLeft(float degrees);
        void rotateUp(float degrees);
        mat4 getViewMatrix();

};
