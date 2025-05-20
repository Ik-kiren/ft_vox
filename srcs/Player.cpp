#include "../includes/Player.hpp"

Player::Player() {
	this->_pos = Vector3(0, 128, 0);
	this->_ch = NULL;
}

Player::Player(float x, float y, float z) {
	this->_pos = Vector3(x, y, z);
	this->_ch = NULL;
	this->_timeFall = 0;
}

Player::~Player() {}

Vector3	Player::getGlobalPos() {
    return this->_pos;
}

Vector3 Player::getChunkPos() {
	Vector3 tmp(0.0);
    tmp = this->_pos;
    if (tmp.x < 0)
        tmp.x -= 16;
    if (tmp.z < 0)
        tmp.z -= 16;
    return (tmp / 16).Trunc();
}

void	Player::setPos(Vector3 const &vec) {
	this->_pos = vec;
}

void	Player::setChunk(unsigned char ****ch) {
	this->_ch = ch;
}

void	Player::setYfromChunk(unsigned char ****ch) {
	for (int k = 255; k >= 0; k--) {
		if (ch[k / 16][std::abs((int)this->getGlobalPos().x % 16)][std::abs((int)this->getGlobalPos().z % 16)][k % 16] != 0) {
			this->setPos(Vector3(this->getGlobalPos().x, k + 3, this->getGlobalPos().z));
			return ;
		}
	}
}

void	Player::setYfromOwnChunk(Vector3 const &camera) {
	int	x = (int)(camera.x) % 16;
	int	y = (int)(this->getGlobalPos().y);
	int	z = (int)(camera.z) % 16;

	if (camera.x < 0)
		x = 15 + x;
	if (camera.z < 0)
		z = 15 + z;

	if (y < 1)
		return ;
	else if (y > 255) {
		this->setPos(Vector3(camera.x, this->getGlobalPos().y - 0.04f * this->_timeFall, camera.z));
		this->_timeFall++;
	} else if (this->_ch[(y - 1) / 16][x][z][(y - 1) % 16] == 0) {
		this->setPos(Vector3(camera.x, this->getGlobalPos().y - 0.04f * this->_timeFall, camera.z));
		this->_timeFall++;
	} else if (this->_ch[y / 16][x][z][y % 16] == 0) {
		this->setPos(Vector3(camera.x, this->getGlobalPos().y, camera.z));
		this->_timeFall = 0;
	} else if (this->_ch[(y + 1) / 16][x][z][(y + 1) % 16] == 0) {
		this->setPos(Vector3(camera.x, this->getGlobalPos().y + 1, camera.z));
		this->_timeFall = 0;
	} else if (this->_ch[(y + 2) / 16][x][z][(y + 2) % 16] == 0) {
		this->setPos(Vector3(camera.x, this->getGlobalPos().y + 2, camera.z));
		this->_timeFall = 0;
	} else if (this->_ch[(y + 3) / 16][x][z][(y + 3) % 16] == 0) {
		this->setPos(Vector3(camera.x, this->getGlobalPos().y + 3, camera.z));
		this->_timeFall = 0;
	}
}