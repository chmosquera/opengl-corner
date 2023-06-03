#include <iostream>
#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION  // Must be before OpenGL includes
#endif
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>


const char *vertexShaderSource = "#version 410 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragShaderSource = "#version 410 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

unsigned int shaderProgram_id, vao_id, vbo_id, ebo_id;

void InitShaders(void) {
    // A unique id to a shader object
    unsigned int vShader_id;

    // Create a vertex shader object
    vShader_id = glCreateShader(GL_VERTEX_SHADER);

    // Set the shader code to the vertex shader object and compile
    glShaderSource(vShader_id, 1, &vertexShaderSource, nullptr);
    glCompileShader(vShader_id);

    // Check if shader compile is successful
    int success;
    char infoLog[512];
    glGetShaderiv(vShader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vShader_id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create a fragment shader
    unsigned int fShader_id;
    fShader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader_id, 1, &fragShaderSource, nullptr);
    glCompileShader(fShader_id);

    glGetShaderiv(fShader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vShader_id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link the shaders to a shader program
    shaderProgram_id = glCreateProgram();
    glAttachShader(shaderProgram_id, vShader_id);
    glAttachShader(shaderProgram_id, fShader_id);
    glLinkProgram(shaderProgram_id);
    glGetProgramiv(shaderProgram_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_id, 512, nullptr, infoLog);
    }

    // Delete the shader objects after linking them to program
    glDeleteShader(vShader_id);
    glDeleteShader(fShader_id);

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
    
    // Link the vertex attributes to the vertex shader
    GLint aPosAttr = glGetAttribLocation(shaderProgram_id, "aPos");
    glVertexAttribPointer(aPosAttr, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPosAttr);

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

    InitShaders();
    
    glEnable(GL_DEPTH_TEST);
    
   // Render loop
   while (!glfwWindowShouldClose(window))
   {
       // input
//           processInput(window);

       // render
       glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       glUseProgram(shaderProgram_id);
       glBindVertexArray(vao_id);
//       glDrawArrays(GL_TRIANGLES, 0, 3);
       glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
       
       // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
       // -------------------------------------------------------------------------------
       glfwSwapBuffers(window);
       glfwPollEvents();
   }

   // glfw: terminate, clearing all previously allocated GLFW resources.
   glfwTerminate();
    glDeleteProgram(shaderProgram_id);
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
