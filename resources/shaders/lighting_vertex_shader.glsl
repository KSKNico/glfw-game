#version 330

uniform mat4 MVP;
uniform vec3 cameraVector;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in uint aFacing;

out vec2 TexCoord;
out float Brightness;

mat3 normalVectors = mat3(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
);

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Brightness = abs(dot(normalVectors[aFacing], cameraVector));
};