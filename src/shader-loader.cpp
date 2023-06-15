//
//  shader-loader.cpp
//  OpenGL_Corner
//
//  Created by Chanelle Mosquera on 6/15/23.
//

#include "shader-loader.hpp"

using namespace Core;

Shader::Shader(const char* vertPath, const char* fragPath) {
    
    // -----------------
    // Read shader files
    // -----------------
    std::string vertCode;
    std::string fragCode;
    std::ifstream vertFile;
    std::ifstream fragFile;
    
    // ensure ifstream objects can throw exceptions:
    vertFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        
        vertFile.open(vertPath);
        fragFile.open(fragPath);
        
        std::stringstream vertStream, fragStream;
        
        vertStream << vertFile.rdbuf();
        fragStream << fragFile.rdbuf();
        
        vertFile.close();
        fragFile.close();
        
        vertCode = vertStream.str();
        fragCode = fragStream.str();
        
        std::cout << vertCode << '\n';
        std::cout << fragCode << '\n';
        
    }
    catch (std::ifstream::failure e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    const char* cvertCode = vertCode.c_str();
    const char* cfragCode = fragCode.c_str();
    
    // --------------
    // Create shaders
    // --------------
    unsigned int vShader_id, fShader_id;

    // Create a vertex shader object
    vShader_id = glCreateShader(GL_VERTEX_SHADER);

    // Set the shader code to the vertex shader object and compile
    glShaderSource(vShader_id, 1, &cvertCode, nullptr);
    glCompileShader(vShader_id);

    // Check if shader compile is successful
    int success;
    GLchar infoLog[512];
    glGetShaderiv(vShader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vShader_id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create a fragment shader
    fShader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader_id, 1, &cfragCode, nullptr);
    glCompileShader(fShader_id);
    
    glGetShaderiv(fShader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vShader_id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        printf("GLError: %d\n", glGetError());
    }
    
    // -------------------
    // Link shader program
    // -------------------
    m_id = glCreateProgram();
    glAttachShader(m_id, vShader_id);
    glAttachShader(m_id, fShader_id);
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    
    // ------------------------------
    // Clean up shaders after linking
    // ------------------------------
    glDeleteShader(vShader_id);
    glDeleteShader(fShader_id);
}

Shader::~Shader(){
    glDeleteProgram(m_id);
}

void Shader::Use() {
    glUseProgram(m_id);
}

void Shader::SetUniformBool(const std::string &name, bool value) {
    auto loc = glGetUniformLocation(m_id, name.c_str());   // Uniform name is defined in shader
    if (loc == -1) {
        printf("ERROR: Could not find uniform location in shader program '%d' (GL error code: %d)\n", m_id, glGetError());
    }
    glUniform1i(loc, value);
}

void Shader::SetUniform1i(const std::string &name, int value) {
    auto loc = glGetUniformLocation(m_id, name.c_str());   // Uniform name is defined in shader
    if (loc == -1) {
        printf("ERROR: Could not find uniform location in shader program '%d' (GL error code: %d)\n", m_id, glGetError());
    }
    glUniform1i(loc, value);
}

void Shader::SetUniform1f(const std::string &name, float value) {
    auto loc = glGetUniformLocation(m_id, name.c_str());   // Uniform name is defined in shader
    if (loc == -1) {
        printf("ERROR: Could not find uniform location in shader program '%d' (GL error code: %d)\n", m_id, glGetError());
    }
    glUniform1f(loc, value);
}

void Shader::SetUniform4f(const std::string &name, float x, float y, float z, float w) {
    auto loc = glGetUniformLocation(m_id, name.c_str());   // Uniform name is defined in shader
    if (loc == -1) {
        printf("ERROR: Could not find uniform location in shader program '%d' (GL error code: %d)\n", m_id, glGetError());
    }
    glUniform4f(loc, x, y, z, w);
}
