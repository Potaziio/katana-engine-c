#version 330 core

out vec4 FragColor;

uniform sampler2D ourTexture; // texture
in vec2 TexCoord; // uvs

void main()
{
    /*
    float pixels = 64.0;
    float dx = 15.0 * (1.0 / pixels);
    float dy = 10.0 * (1.0 / pixels);
    vec2 coord = vec2(dx * floor(TexCoord.x / dx), dy * floor(TexCoord.y / dy));
    */
    FragColor = texture(ourTexture, TexCoord);
}  

