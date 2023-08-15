#version 330

uniform mat4 MVP;
uniform vec3 cameraVector;

// data.x:
// facing | z | y | x
// data.y:
// unused | index | v | u
layout (location = 0) in uvec2 data;;

out vec2 TexCoord;
out float Brightness;
flat out uint TextureIndex;

mat3 normalVectors = mat3(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
);

const uint xMask =          0x000000FFu;
const uint yMask =          0x0000FF00u;
const uint zMask =          0x00FF0000u;
const uint facingMask =     0xFF000000u;

const uint uMask =          0x000000FFu;
const uint vMask =          0x0000FF00u;
const uint indexMask =      0x00FF0000u;

void main()
{
    gl_Position = MVP * vec4(float(data.x & xMask), float((data.x & yMask) >> 8u), float((data.x & zMask) >> 16u), 1.0);
    TexCoord.x = float(data.y & uMask);
    TexCoord.y = float((data.y & vMask) >> 8u);
    Brightness = float(abs(dot(normalVectors[int((data.x & facingMask) >> 24)], cameraVector)));
    TextureIndex = uint(((data.y & indexMask) >> 16u));
};

