#version 330

uniform sampler2DArray ourTextures;

flat in uvec2 TexCoord;
in float Brightness;
flat in uint TextureIndex;

out vec4 FragColor;

void main()
{
    FragColor = texture(ourTextures, vec3(TexCoord, TextureIndex)) * Brightness;
};