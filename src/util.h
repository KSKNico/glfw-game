#pragma once
#include <functional>
#include <glm/vec3.hpp>

class IntegerVec3Hasher {
    public:
        std::size_t operator() (const glm::ivec3& k) const;
};