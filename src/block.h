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

        Block(glm::ivec3 position, Type type = Type::AIR);
        Block();

        // global position of block in world
        glm::ivec3 position;
        bool hidden;
        Type type;

        void render();
        bool isSolid() const;
    private:
        
};