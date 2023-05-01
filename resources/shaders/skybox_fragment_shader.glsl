#version 330

uniform samplerCube ourTexture;

in vec3 TextureVector;

out vec4 FragColor;

void main()
{
    FragColor = texture(ourTexture, TextureVector);
};