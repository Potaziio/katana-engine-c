#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 uvs;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 position = vec4(aPos, 1.0f);
    gl_Position = projection * view * position;
    uvs = aTex;
}

