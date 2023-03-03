#include "block.h"



Block::Block(glm::vec3 position, Type type) : position(position), type(type) {
}

bool Block::isSolid() const {
    return bool(this->type);
}

void Block::render() {
    
}

