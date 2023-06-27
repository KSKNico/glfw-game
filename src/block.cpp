#include "block.h"

Block::Block(glm::ivec3 position, Type type) : position(position), type(type) {
}

Block::Block() {
    Block(glm::ivec3(0, 0, 0), Type::AIR);
}

bool Block::isSolid() const {
    return bool(this->type);
}

void Block::render() {
}
