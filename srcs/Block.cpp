#include "../includes/Block.hpp"

Block::Block() {
    this->active = true;
}

Block::~Block() {

}

bool Block::IsActive() {
    return this->active;
}

void Block::SetActive(bool active) {
    this->active = active;
}