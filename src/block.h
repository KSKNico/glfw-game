#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>


class Block {
    public:
        enum Type {
            AIR = 0,
            SOLID,
        };

        Block(glm::vec<3, GLubyte, glm::highp_bvec3> position, Type type = Type::AIR);

        // global position of block in world
        glm::vec<3, GLubyte, glm::highp_bvec3>
        glm::mat3 points;
        glm::mat3 colors;
        bool hidden;
        Type type;

        void render();
        bool isSolid() const;
    private:
        
};