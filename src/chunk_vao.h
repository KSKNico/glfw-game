#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <glm/vec3.hpp>
#include <vector>

class ChunkVAO {
    public:
    static constexpr unsigned int BYTES_FOR_VERTEX = 3 * sizeof(GLubyte);

    ChunkVAO(const std::vector<glm::u8vec3> &vertexPositions,
             const std::vector<GLubyte> &vertexFacing,
             const std::vector<GLubyte> &textureIndices,
             const std::vector<GLuint> &indexBuffer);
    ~ChunkVAO();

    GLuint vao;
    GLuint vertexBuffer;
    GLuint vertexFacingBuffer;
    GLuint textureIndicesBuffer;
    GLuint indexBuffer;

    private:


};