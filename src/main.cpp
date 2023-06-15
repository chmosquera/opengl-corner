#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION  // Must be before OpenGL includes
#endif

#include <iostream>
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <cmath>
#include "shader-loader.hpp"

using namespace Core;

unsigned int vao_id, vbo_id, ebo_id;

void InitGeom(void) {
    
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
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
    
    Shader mainShader = Shader("/Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/src/main_vert.glsl", "/Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/src/main_frag.glsl");
    
    // Link the vertex attributes to the vertex shader
    GLint aPosAttr = glGetAttribLocation(mainShader.GetID(), "aPos");
    glVertexAttribPointer(aPosAttr, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPosAttr);
    
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
       
       // Update uniform in shader program
       mainShader.SetUniform1f("uTime", glfwGetTime());

       
//       int uColor_loc = glGetUniformLocation(shaderProgram_id, "uColor");
//       if (uColor_loc == -1) {
//          std::cout << "ERROR: Could not find uniform location in shader program'" << shaderProgram_id << "'\n";
//      }
//       glUniform4f(uColor_loc, 1.0, 1.0, sin(glfwGetTime()), 1.0);
       
       double  timeValue = glfwGetTime();
       float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
       int vertexColorLocation = glGetUniformLocation(mainShader.GetID(), "uColor");
       glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
       
       // Draw triangle
       glBindVertexArray(vao_id);
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
