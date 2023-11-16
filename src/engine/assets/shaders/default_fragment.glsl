#version 330 core

out vec4 FragColor;
in vec4 ourColor;

uniform float time;

void main()
{
    float color_r = ourColor.r / 255.0f;
    float color_g = ourColor.g / 255.0f;
    float color_b = ourColor.b / 255.0f;
    float color_a = ourColor.a / 255.0f;

    FragColor = vec4(color_r, color_g, color_b, color_a);
}
