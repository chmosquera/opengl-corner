#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION  // Must be before OpenGL includes
#endif

#include <iostream>
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <cmath>
#include "shader-loader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Core;

unsigned int vao_id, vbo_id, tex_id;

std::string root_folder = "/Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera properties
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Temporary up vector - a trick used to get right.
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
float yaw, pitch = 0.0f;
glm::vec3 direction = glm::vec3(0.0);
bool firstMouse = true;
float lastMouseX = SCR_WIDTH/2.0;
float lastMouseY = SCR_HEIGHT/2.0;
float deltaTime, lastTime = 0.0f;

void InitGeom(void) {
    
    // Cube
    float vertices[] = {
        // pos                  // tex
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f
        };


    // Generate array and buffers
    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vbo_id);               // vertex buffer, to store vertex data

    // Bind and configure the vertex array object for later use
    glBindVertexArray(vao_id);
    
    
    // Copy data to the buffer and configures GPU to manage data
    // Params: 1) Type of buffer, 2) Size of data, 3) Data, 4) How GPU should manage data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void InitTexture(const char* path){
    
    stbi_set_flip_vertically_on_load(true); // Set before stbi_load

    
    // Load image
    int width, height, channel_cnt;
    unsigned char *data = stbi_load(path, &width, &height, &channel_cnt, 0);
    if (data == NULL) {
        printf("Failed to load texture '%s'", data);
        return;
    }
    
    // Generate texture
    glGenTextures(1, &tex_id); // Generate 1 texture and store in uint array
    glBindTexture(GL_TEXTURE_2D, tex_id);
    
    // Texture coordinates S,T
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D); // optional if you use the base mipmap via glTexImage2D
    
    
    // Clean up
    stbi_image_free(data);    
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


int main(int argc, char *argv[]) {
    
    // glfw - initialize and configure
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
       glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

   // Create window (GLFW library)
   GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Corner", NULL, NULL);
   if (window == NULL)
   {
       std::cout << "Failed to create GLFW window" << std::endl;
       glfwTerminate();
       return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set up mouse callback (GLFW library)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    double mouseX, mouseY = 0.0f;
    glfwSetCursorPosCallback(window, mouse_callback);

   // Glad - load all OpenGL function pointers
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
       std::cout << "Failed to initialize GLAD" << std::endl;
       return -1;
   }

    // Call after loading OpenGL context
    printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));


    InitGeom();
    
    InitTexture("/Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/Assets/zebra.png");

    Shader mainShader = Shader("/Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/src/texture_vert.glsl", "/Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/src/texture_frag.glsl");

    // Link the vertex attributes to the vertex shader
    // index: attr location, size: value, stride: length of vertex data, pointer: offset.
    GLint aPosAttr = glGetAttribLocation(mainShader.GetID(), "aPos");
    glVertexAttribPointer(aPosAttr, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPosAttr);

    GLint aTexCoords = glGetAttribLocation(mainShader.GetID(), "aTexCoords");
    glVertexAttribPointer(aTexCoords, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(aTexCoords);
    
    glEnable(GL_DEPTH_TEST);
    
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 0.5f,  0.0f,  0.0f),
        glm::vec3( 0.5f,  0.5f,  0.0f),
        glm::vec3( 0.0f,  0.5f,  0.0f),

    };
    
    
   // Render loop
   while (!glfwWindowShouldClose(window))
   {
       // Calculate delta time
       deltaTime = glfwGetTime() - lastTime;
       lastTime = glfwGetTime();
       
       // input
       process_input(window);

       // render
       glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       mainShader.Use();

       // Bind VAO
       glBindVertexArray(vao_id);

       // Bind texture
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, tex_id);
       glEnable(GL_TEXTURE_2D);

       
       glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
       mainShader.SetUniformMatrix4v("uProj", proj);
       
       // ------------------------
       // Update camera/view space
       // ------------------------
       // rotating camera in circle (trig math)
       const float radius = 10.0f;
       float camX = sin(glfwGetTime() * radius);
       float camZ = cos(glfwGetTime() * radius);
       
       
       glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
       mainShader.SetUniformMatrix4v("uView", view);
       
       for (int x = 0; x < 5; x++) {
           for (int y = 0; y < 5; y++) {
               for (int z = 0; z < 5; z++) {
                   // Transform geom
                   glm::vec3 offset = glm::vec3(-2.5, -2.5, 2.5);
                   glm::vec3 objPos = glm::vec3(x, y, z) + offset;
                   objPos *= 2.0f;
                   glm::mat4 model = glm::mat4(1.0f);
                   model = glm::scale(model, glm::vec3(0.25));
                   model = glm::translate(model, objPos);
                   mainShader.SetUniformMatrix4v("uModel", model);
                   
                   // Draw triangle
                   glDrawArrays(GL_TRIANGLES, 0, 36);
               }
           }
       } 

       // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
       // -------------------------------------------------------------------------------
       glfwSwapBuffers(window);
       glfwPollEvents();
   }

   // glfw: terminate, clearing all previously allocated GLFW resources.
   glfwTerminate();
   return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    //const float cameraSpeed = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    
    if (firstMouse)
    {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos; // y is reversed
    
    lastMouseX = xpos;
    lastMouseY = ypos;
    
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
    // Constrain camera rotation to avoid strange camera effects
    // e.g. when direction vector is parallel to world up vector, the camera flips.
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    
    // Update direction
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    
    printf("Camera front: (%f, %f, %f)\n", cameraFront.x, cameraFront.y, cameraFront.z);
}