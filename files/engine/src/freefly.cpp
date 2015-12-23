#include "engine/freefly.hpp"

glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

// void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime){
//     GLfloat velocity = this->MovementSpeed * deltaTime;
//     if (direction == FORWARD)
//         this->Position += this->Front * velocity;
//     if (direction == BACKWARD)
//         this->Position -= this->Front * velocity;
//     if (direction == LEFT)
//         this->Position -= this->Right * velocity;
//     if (direction == RIGHT)
//         this->Position += this->Right * velocity;
// }

void Camera::MoveFront(float t){
    this->Position += t * this->Front;
}

void Camera::MoveRight(float t){
    this->Position += t * this->Right;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch){
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw   += xoffset;
    this->Pitch -= yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

void Camera::updateCameraVectors(){
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
}
