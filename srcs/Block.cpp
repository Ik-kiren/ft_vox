#include "../includes/Block.hpp"

Block::Block() {
    this->active = true;
}

Block::~Block() {

}

bool Block::IsActive() {
    return this->active;
}

void Block::ClearFaces() {
    for (int i = 0; i < 6; i++) {
        visited[i] = false;
    }
    
}

void Block::SetActive(bool active) {
    this->active = active;
}