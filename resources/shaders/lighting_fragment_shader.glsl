#version 330

uniform sampler2D ourTexture;

in vec2 TexCoord;
in float Brightness;

out vec4 FragColor;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * Brightness;
};