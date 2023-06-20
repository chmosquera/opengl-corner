//
//  shader-loader.hpp
//  OpenGL_Corner
//
//  Created by Chanelle Mosquera on 6/15/23.
//

#ifndef shader_loader_hpp
#define shader_loader_hpp

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION  // Must be before OpenGL includes
#endif

#include <glad/glad.h> // Include OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Core {

    class Shader {
        
    public:
        
        
        Shader(const char* vertPath, const char* fragPath);
        ~Shader();
        
        void Use();
        
        unsigned int GetID(){return m_id;}
        
        void SetUniformBool(const std::string &name, bool value);
        void SetUniform1i(const std::string &name, int value);
        void SetUniform1f(const std::string &name, float value);
        void SetUniform4f(const std::string &name, float x, float y, float z, float w);
        
    private:
        unsigned int m_id;
    };

}
#endif /* shader_loader_hpp */


