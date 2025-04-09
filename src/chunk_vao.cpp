#include "chunk_vao.h"

ChunkVAO::ChunkVAO(const std::vector<glm::u8vec3> &vertexPositions, 
                   const std::vector<GLubyte> &vertexFacing, 
                   const std::vector<GLubyte> &textureIndices, 
                   const std::vector<GLuint> &indices) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);;

    // vertex buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexFacingBuffer);
    glBufferData(GL_ARRAY_BUFFER, BYTES_FOR_VERTEX * vertexPositions.size() * sizeof(GLubyte), &vertexPositions[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, NULL);

    // vertex facing buffer
    glGenBuffers(1, &vertexFacingBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexFacingBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexFacing.size() * sizeof(GLubyte), &vertexFacing[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, 0, NULL);

    // texture indices buffer
    glGenBuffers(1, &textureIndicesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureIndicesBuffer);
    glBufferData(GL_ARRAY_BUFFER, textureIndices.size() * sizeof(GLubyte), &textureIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, 0, NULL);

    // index buffer
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

ChunkVAO::~ChunkVAO() {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &vertexFacingBuffer);
    glDeleteBuffers(1, &textureIndicesBuffer);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteVertexArrays(1, &vao);
}