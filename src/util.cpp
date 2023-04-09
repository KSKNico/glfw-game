#include "util.h"

std::size_t IntegerVec3Hasher::operator()(const glm::ivec3& k) const {
    return (
        ((std::hash<int>()(k[0])
        ^ (std::hash<int>()(k[1]) << 1 )) >> 1)
        ^ (std::hash<int>()(k[2]) << 1)
    );
}