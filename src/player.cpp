#include "player.h"

Player::Player(glm::vec3 position, Camera& camera) : position(position), camera(camera) {
    speed = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Player::movePlayer(glm::vec3& direction) {
    this->position += direction;
}