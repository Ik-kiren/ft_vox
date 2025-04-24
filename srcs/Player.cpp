#include "../includes/Player.hpp"

Player::Player() {
	this->_pos = Vector3(0, 128, 0);
}

Player::Player(float x, float y, float z) {
	this->_pos = Vector3(x, y, z);
}

Player::~Player() {}

Vector3	Player::getPos() {
	return this->_pos;
}

void	Player::setPos(Vector3 vec) {
	this->_pos = vec;
}

void	Player::setYfromChunk(chunk *ch) {
	for (int k = 255; k >= 0; k--) {
		if (ch[k / 16].voxel[(int)this->getPos().x % 16][(int)this->getPos().z % 16][k % 16] != 0) {
			this->setPos(Vector3(this->getPos().x, k + 2, this->getPos().z));
			return ;
		}
	}
}