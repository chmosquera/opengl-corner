//
//  texture_frag.glsl
//  OpenGL_Corner
//
//  Created by Chanelle Mosquera on 6/15/23.
//

#version 410 core

in vec4 vPos;
in vec2 vTexCoords;
in vec3 vColor;

out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    
    // Set pixel color
    FragColor = texture(uTexture, vTexCoords);
//    FragColor = vec4(0.0f);

}
