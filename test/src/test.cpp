#include <gtest/gtest.h>

#include "../../src/vertex.h"

TEST(Vertex, VertexInterlacing) {
  Vertex v1(v_position(2, 3, 4), v_textureCoordinates(5, 6), 7, 8);
  auto data = v1.convertVertex();
  ASSERT_EQ(data[0], 0b00000111000001000000001100000010);
  ASSERT_EQ(data[1], 0b00000000000010000000011000000101);
}
