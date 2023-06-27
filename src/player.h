#pragma once

#include <glm/matrix.hpp>

#include "camera.h"

class Player {
   public:
    Player(glm::vec3 position, Camera &camera);
    glm::vec3 position;
    glm::vec3 speed;
    Camera &camera;

    void movePlayer(glm::vec3 &direction);
    void isLookingAtBlock();
};