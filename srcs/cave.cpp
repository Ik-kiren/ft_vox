#include "../includes/cave.hpp"

cave::cave() {}

cave::cave(int x, int y, int size, int lenght) {
	this->_seed = randSeed(x, y);
	this->_sizeIni = size;
	this->_lenght = lenght;
	this->_nbrGP = 8;
	this->_sizeModif = 5;

	for (int i = 0; i < this->_lenght; i++) {
		caveGP	tmp;
		tmp.GP = 0;
		tmp.sizeF = 0;
		tmp.sizeI = 0;
		tmp.dirXF = 0;
		tmp.dirXI = 0;
		tmp.dirYF = 0;
		tmp.dirYI = 0;
		tmp.dirZF = 0;
		tmp.dirZI = 0;

		this->_caveX.push_back(tmp);
	}
	this->_caveX[0].GP = 1;
	this->_caveX[0].sizeF = 0;
	this->_caveX[0].dirXF = randFloatBetween(1);
	this->_caveX[0].dirYF = randFloatBetween(1);
	this->_caveX[0].dirZF = 0;

	this->_caveX[this->_lenght - 1].GP = 1;
	this->_caveX[this->_lenght - 1].sizeF = randFloatBetween(1);
	this->_caveX[this->_lenght - 1].dirXF = randFloatBetween(1);
	this->_caveX[this->_lenght - 1].dirYF = randFloatBetween(1);
	this->_caveX[this->_lenght - 1].dirZF = randFloatBetween(1);
}

cave::~cave() {}

void	cave::doIniCave() {
	std::srand(this->_seed);
	float	H1 = 1;
	float	H2 = 1;
	int		L = 0;

	for (int t = 0; t < this->_nbrGP; t++) {
		L = 0;
		for (int j = 1; j < this->_lenght; j++) {
			if (this->_caveX[j].GP == 1) {
				this->_caveX[(j + L) / 2].GP = 1;
				this->_caveX[(j + L) / 2].sizeF = (this->_caveX[L].sizeF + this->_caveX[j].sizeF) / 2 + randFloatBetween(H1);
				this->_caveX[(j + L) / 2].dirXF = (this->_caveX[L].dirXF + this->_caveX[j].dirXF) / 2 + randFloatBetween(H2);
				this->_caveX[(j + L) / 2].dirYF = (this->_caveX[L].dirYF + this->_caveX[j].dirYF) / 2 + randFloatBetween(H2);
				this->_caveX[(j + L) / 2].dirZF = (this->_caveX[L].dirZF + this->_caveX[j].dirZF) / 2 + randFloatBetween(H2);
				L = j;
			}
		}
		H1 /= 2;
		H2 /= 2;
	}
	afterGPcave();
}

void	cave::afterGPcave() {
	for (int i = 0; i < this->_lenght; i++) {
		this->_caveX[i].sizeI = std::max(this->_sizeIni + (int)(this->_caveX[i].sizeF * this->_sizeModif), 1);
		this->_caveX[i].dirXI = (int)(this->_caveX[i].dirXF * 2.0f);
		this->_caveX[i].dirYI = (int)(this->_caveX[i].dirYF * 2.0f);
		this->_caveX[i].dirZI = (int)(this->_caveX[i].dirZF + 1);
	}
}

int		cave::getLenght() {
	return this->_lenght;
}

caveGP	cave::getCaveX(int i) {
	return this->_caveX[i];
}