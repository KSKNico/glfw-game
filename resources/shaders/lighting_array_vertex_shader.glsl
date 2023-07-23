#version 330

uniform mat4 MVP;
uniform vec3 cameraVector;

// data[0]:
// facing | z | y | x
// data[1]:
// unused | index | v | u
layout (location = 0) in uint data[2];

out uvec2 TexCoord;
out float Brightness;
flat out uint TextureIndex;

mat3 normalVectors = mat3(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
);

const uint xMask = 0x000000FFu;
const uint yMask = 0x0000FF00u;
const uint zMask = 0x00FF0000u;
const uint facingMask = 0xFF000000u;

const uint uMask = 0x000000FFu;
const uint vMask = 0x0000FF00u;
const uint indexMask = 0x00FF0000u;

void main()
{
    gl_Position = MVP * vec4(data[0] & xMask, (data[0] & yMask) >> 8, (data[0] & zMask) >> 16, 1.0);
    TexCoord.x = data[1] & uMask;
    TexCoord.y = (data[1] & vMask) >> 8;
    Brightness = abs(dot(normalVectors[(data[0] & facingMask) >> 24], cameraVector));
    TextureIndex = ((data[1] & indexMask) >> 16);
};