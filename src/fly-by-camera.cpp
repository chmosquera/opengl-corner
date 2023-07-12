//
//  FlyByCamera.cpp
//  OpenGL_Corner
//
//  Created by Chanelle Mosquera on 7/12/23.
//

#include "fly-by-camera.hpp"

void FlyByCamera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
    
    float deltaSpeed = m_movementSpeed * deltaTime;
    switch (direction) {
        case FORWARD:
            m_position += m_front * deltaSpeed;
            break;
        case BACK:
            m_position -= m_front * deltaSpeed;
            break;
        case LEFT:
            m_position -= glm::normalize(glm::cross(m_front, m_up)) * deltaSpeed;
            break;
        case RIGHT:
            m_position += glm::normalize(glm::cross(m_front, m_up)) * deltaSpeed;
            break;
        default:
            break;
    }
}
    
    
void FlyByCamera::ProcessMouseInput(float deltaX, float deltaY, float deltaTime) {
    
    m_yaw += deltaX * m_mouseSensitivity;
    m_pitch += deltaY * m_mouseSensitivity;
    
    // Fly-by-cameras have the following limitations
    if (m_pitch > 89.0)
        m_pitch = 89.0;
    if (m_pitch < -89.0)
        m_pitch = -89.0;
    
    updateCameraVectors();
}
    

void FlyByCamera::updateCameraVectors() {
    
    // Calculate direction vector using trig
    glm::vec3 direction = glm::vec3(1.0f);
    direction.x = cos(glm::radians(m_pitch)) + cos(glm::radians(m_yaw));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = cos(glm::radians(m_pitch)) + sin(glm::radians(m_yaw));
    
    m_front = glm::normalize(direction);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
    
}
