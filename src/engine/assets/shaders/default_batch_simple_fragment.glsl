#version 330 core

out vec4 FragColor;
in vec4 ourColor;

uniform float time;

void main()
{
    FragColor = ourColor;
}
