#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

out vec2 TexCoord;
out vec4 texColor;

void main() {
    TexCoord = aTexCoord;
    texColor = aColor;
    gl_Position =  projection * view * transform * vec4(aPos, 1.0f);
}

