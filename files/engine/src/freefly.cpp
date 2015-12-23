#include "engine/freefly.hpp"


using namespace std;
using namespace glm;


//Calcul les vecteurs Left, Front et Up à partir des angles Phi et Teta
void FreeFlyCamera::computeDirectionVectors(){
    //Front Vector : F =(cos(θ)sin(ϕ), sin(θ), cos(θ)cos(ϕ))
    m_FrontVector = vec3(cos(m_fTheta) * sin(m_fPhi),sin(m_fTheta), cos(m_fTheta)* cos(m_fPhi));
    //Left Vector : L =(sin(ϕ+π/2), 0, cos(ϕ+π2))
    m_LeftVector = vec3(sin(m_fPhi + pi<float>()/2),0,cos(m_fPhi + pi<float>()/2));
    //Up Vector : U = F x L (utiliser la fonction cross)
    m_UpVector = cross(m_FrontVector,m_LeftVector);
}

//Initialisation de la Caméra
FreeFlyCamera::FreeFlyCamera(){
    m_Position = vec3(0,0,0);
    m_fPhi = pi<float>();
    m_fTheta = 0.0f;
    computeDirectionVectors();
}

void FreeFlyCamera::moveLeft(float t){
    m_Position += t * m_LeftVector;
}

void FreeFlyCamera::moveFront(float t){
    m_Position += t * m_FrontVector;
}

void FreeFlyCamera::rotateLeft(float degrees){
    m_fPhi += degrees * pi<float>() / 180;
}

void FreeFlyCamera::rotateUp(float degrees){
    m_fTheta += degrees * pi<float>() / 180;
}

mat4 FreeFlyCamera::getViewMatrix(){
    computeDirectionVectors();
    return lookAt(m_Position,m_Position + m_FrontVector,m_UpVector);
}
