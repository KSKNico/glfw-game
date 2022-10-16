#pragma once

#include "camera.h"

#include <glm/matrix.hpp>

class Player {
    public:
        Player(glm::vec3 position, Camera &camera);
        glm::vec3 position;
        Camera &camera;
};