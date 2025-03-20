#version 330

uniform mat4 MVP;
uniform vec3 cameraVector;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in uint aFacing;
layout (location = 3) in uint aTextureIndex;

out vec2 TexCoord;
out float Brightness;
flat out uint TextureIndex;

mat3 normalVectors = mat3(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
);

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Brightness = clamp(abs(dot(normalVectors[aFacing], cameraVector)), 0.6f, 1.0f);
    TextureIndex = aTextureIndex;
};