//
//  FlyByCamera.hpp
//  OpenGL_Corner
//
//  Created by Chanelle Mosquera on 7/12/23.
//

#ifndef FlyByCamera_hpp
#define FlyByCamera_hpp

#include <stdio.h>
#include <glad/glad.h> // Include OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACK,
    LEFT,
    RIGHT
};

// Constant defaults
const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class FlyByCamera {
    
public:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    
    float m_yaw;
    float m_pitch;
    
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;
    
    FlyByCamera(glm::vec3 position = glm::vec3(0.0, 0.0, 0.0),
        glm::vec3 up = glm::vec3(0.0, 1.0, 0.0),
        float yaw = YAW,
        float pitch = PITCH)
        : m_front(glm::vec3(0.0, 0.0, -1.0)),
        m_movementSpeed(SPEED),
        m_mouseSensitivity(SENSITIVITY),
        m_zoom(ZOOM)
    {
        m_position = position;
        m_up = up;
        m_worldUp = up;
        m_yaw = yaw;
        m_pitch = pitch;
    };
    
    ~FlyByCamera(){};
    
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }
    
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    
    void ProcessMouseInput(float deltaX, float deltaY, float deltaTime);
    
    
private:
    bool m_firstMouse = true;
    void updateCameraVectors();
};



#endif /* FlyByCamera_hpp */
