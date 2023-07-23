#include "vertex.h"

std::array<GLuint, 2> Vertex::convertVertex() const {
    std::array<GLuint, 2> data;
    data[0] = position.x | (position.y << 8) | (position.z << 16) | (facing << 24);
    data[1] = textureCoordinates.x | (textureCoordinates.y << 8) | (textureIndex << 16);
    return data;
}

std::vector<std::array<GLuint, 2>> Vertex::interlaceVertexData(const std::vector<Vertex> &vertices) {
    std::vector<std::array<GLuint, 2>> data;
    data.reserve(vertices.size());
    for (const Vertex &vertex : vertices) {
        data.push_back(vertex.convertVertex());
    }
    return data;
}