#pragma once

#include <glm/matrix.hpp>

// the position of a block in world coordinates
using worldCoordinates = glm::ivec3;

// chunk local coordinates with all components in [0, CHUNK_SIZE-1]
using localCoordinates = glm::u8vec3;

// the position of the chunk in the world
// all chunks have their own internal coordinate system defined by the localCoordinates
using chunkCoordinates = glm::ivec3;
