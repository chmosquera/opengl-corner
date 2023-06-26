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

unsigned int vao_id, vbo_id, tex_id, ebo_id;

std::string root_folder = "/Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner";

void InitGeom(void) {
    
    float vertices[] = {
        // pos                  // tex coords   // color
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,     1.0f, 0.0f, 0.0f,   // bottom-left (red)
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f,     0.0f, 1.0f, 0.0f,   // bottom-right (green)
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f,     0.0f, 0.0f, 1.0f,   // top-right (blue)
        -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,     0.0f, 0.0f, 0.0f    // top-left (black)
    };

    unsigned int indices[] = {
        0, 1, 2,  // first Triangle
        2, 3, 0   // second Triangle
    };
        
    // Generate array and buffers
    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vbo_id);               // vertex buffer, to store vertex data
    glGenBuffers(1, &ebo_id);               // element buffer, to store indices

    // Bind and configure the vertex array object for later use
    glBindVertexArray(vao_id);
    
    
    // Copy data to the buffer and configures GPU to manage data
    // Params: 1) Type of buffer, 2) Size of data, 3) Data, 4) How GPU should manage data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Copies index data to the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
//void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char *argv[]) {
    
    // glfw - initialize and configure
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
       glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

   // glfw - window creation
   GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Corner", NULL, NULL);
   if (window == NULL)
   {
       std::cout << "Failed to create GLFW window" << std::endl;
       glfwTerminate();
       return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


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
    glVertexAttribPointer(aPosAttr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPosAttr);

    GLint aTexCoords = glGetAttribLocation(mainShader.GetID(), "aTexCoords");
    glVertexAttribPointer(aTexCoords, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(aTexCoords);


    GLint aColor = glGetAttribLocation(mainShader.GetID(), "aColor");
    glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(aColor);
    
    glEnable(GL_DEPTH_TEST);

   // Render loop
   while (!glfwWindowShouldClose(window))
   {
       // input
//           processInput(window);

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

       // Transform geom
       glm::mat4 trans = glm::mat4(1.0f);
       trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
       trans = glm::translate(trans, glm::vec3(sin(glfwGetTime()), 0.0, 0.0));
       GLint uTransform = glGetUniformLocation(mainShader.GetID(), "uTransform");
       glUniformMatrix4fv(uTransform, 1, GL_FALSE, glm::value_ptr(trans));  // If shader program is not used, 1282 error.
       
       // Draw triangle
       glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
