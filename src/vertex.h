#pragma once

#include <array>
#include <glm/matrix.hpp>
#include <vector>

#include "glad.h"

typedef glm::vec<3, GLubyte, glm::packed_highp> v_position;
typedef glm::vec<2, GLubyte, glm::packed_highp> v_textureCoordinates;

class Vertex {
   public:
    v_position position;
    v_textureCoordinates textureCoordinates;
    GLubyte facing;
    GLubyte textureIndex;

    Vertex(const v_position &position, const v_textureCoordinates &textureCoordinates,
           const GLubyte &facing, const GLubyte &textureIndex) : position(position), textureCoordinates(textureCoordinates), facing(facing), textureIndex(textureIndex){};

    std::array<GLuint, 2> convertVertex() const;

    static std::vector<std::array<GLuint, 2>> interlaceVertexData(const std::vector<Vertex> &vertices);
};