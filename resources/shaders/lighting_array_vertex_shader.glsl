#version 330

uniform mat4 MVP;
uniform vec3 cameraVector;

layout (location = 0) in vec3 aPos;
layout (location = 1) in uint aFacing;
layout (location = 2) in uint aTextureIndex;

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

    if (aFacing == 0)
        TexCoord = vec2(aPos.y, aPos.z);
    else if (aFacing == 1)
        TexCoord = vec2(aPos.x, aPos.z);
    else
        TexCoord = vec2(aPos.x, aPos.y);
    Brightness = clamp(abs(dot(normalVectors[aFacing], cameraVector)), 0.6f, 1.0f);
    TextureIndex = aTextureIndex;
};