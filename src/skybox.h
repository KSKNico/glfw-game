#pragma once

#include <glm/matrix.hpp>
#include <vector>

#include "glad.h"

class Skybox {
   public:
    GLuint vao;
    GLuint vertexBuffer;
    GLuint indexBuffer;

    std::vector<glm::vec3> vertexPositions;
    std::vector<GLubyte> indices;

    Skybox();

    void createVAO();
};