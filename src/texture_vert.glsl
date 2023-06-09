//
//  texture_vert.glsl
//  OpenGL_Corner
//
//  Created by Chanelle Mosquera on 6/15/23.
//

#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform sampler2D uTexture;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec4 vPos;
out vec2 vTexCoords;
out vec3 vColor;

void main()
{
    vec4 pos = uProj * uView * uModel * vec4(aPos, 1.0);
    
    // Set vertex position
    gl_Position = pos;
    
    // Send to frag shader
    vPos = pos;
    vTexCoords = aTexCoords;
}
