#include <gtest/gtest.h>

#include "../../src/vertex.h"

TEST(Vertex, ConvertVertex) {
  Vertex v1(v_position(2, 3, 4), v_textureCoordinates(5, 6), 7, 8);
  auto data = v1.convertVertex();
  ASSERT_EQ(data[0], 0b00000111000001000000001100000010);
  ASSERT_EQ(data[1], 0b00000000000010000000011000000101);
}

TEST(Vertex, InterlaceVertices) {
  std::vector<Vertex> vertices;
  
  // populate vector with vertices
  vertices.push_back(Vertex(v_position(2, 3, 4), v_textureCoordinates(5, 6), 7, 8));
  vertices.push_back(Vertex(v_position(2, 3, 4), v_textureCoordinates(5, 6), 7, 8));
  vertices.push_back(Vertex(v_position(2, 3, 4), v_textureCoordinates(5, 6), 7, 8));

  auto data = Vertex::interlaceVertexData(vertices);

  for (int i = 0; i < vertices.size(); i++) {
    ASSERT_EQ(data[i][0], 0b00000111000001000000001100000010);
    ASSERT_EQ(data[i][1], 0b00000000000010000000011000000101);
  }  
}

TEST(Vertex, InterlaceDivereseVertices) {
  std::vector<Vertex> vertices;

  vertices.push_back(Vertex(v_position(2, 3, 4), v_textureCoordinates(5, 6), 7, 8));
  vertices.push_back(Vertex(v_position(1, 1, 1), v_textureCoordinates(1, 1), 1, 1));
  vertices.push_back(Vertex(v_position(10, 12, 14), v_textureCoordinates(16, 18), 20, 22));

  auto data = Vertex::interlaceVertexData(vertices);

  ASSERT_EQ(data[0][0], 0b00000111000001000000001100000010);
  ASSERT_EQ(data[0][1], 0b00000000000010000000011000000101);

  ASSERT_EQ(data[1][0], 0b00000001000000010000000100000001);
  ASSERT_EQ(data[1][1], 0b00000000000000010000000100000001);

}
