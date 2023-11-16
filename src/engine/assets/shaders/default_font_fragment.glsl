#version 330 core

out vec4 FragColor;
uniform sampler2D ourTexture;

in vec2 TexCoord;
in vec4 texColor;

void main()
{
    float color_r = texColor.r / 255.0f;
    float color_g = texColor.g / 255.0f;
    float color_b = texColor.b / 255.0f;
    float color_a = texColor.a / 255.0f;

    FragColor = texture(ourTexture, TexCoord) * vec4(color_r, color_g, color_b, color_a);
}  

