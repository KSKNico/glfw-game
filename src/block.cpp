#include "block.h"



Block::Block(glm::vec<3, GLubyte, glm::highp_bvec3> position, Type type) : position(position), type(type) {
}

bool Block::isSolid() const {
    return bool(this->type);
}

void Block::render() {
    
}

