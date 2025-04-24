#include "../includes/cave.hpp"

cave::cave() {
	this->_lenght = 0;
	this->_tab = NULL;
}

cave::cave(int sizeBiome, int lenghtMax) {
	this->_seed = 0;
	this->_sizeIni = 3;
	this->_lenght = lenghtMax;
	this->_nbrGP = 8;
	this->_sizeModif = 6;
	this->_sizeBiome = sizeBiome;

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

	caveDig	**ret = new caveDig*[this->_sizeBiome - 1];
	for (int i = 0; i < this->_sizeBiome - 1; i++) {
		ret[i] = new caveDig[this->_sizeBiome - 1];
		for (int j = 0; j < this->_sizeBiome - 1; j++)
			ret[i][j].impact = 0;
	}
	this->_tab = ret;
}

cave::~cave() {
	if (!this->_tab)
		return ;
	for (int i = 0; i < this->_sizeBiome - 1; i++)
		delete [] this->_tab[i];
	delete [] this->_tab;
	this->_tab = NULL;
}

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
		H1 /= 1.5;
		H2 /= 1.5;
	}
	afterGPcave();
}

void	cave::afterGPcave() {
	for (int i = 0; i < this->_lenght; i++) {
		this->_caveX[i].dirXI = (int)(this->_caveX[i].dirXF * 3.0f);
		this->_caveX[i].dirYI = (int)(this->_caveX[i].dirYF * 3.0f);	
		this->_caveX[i].dirZI = (int)(this->_caveX[i].dirZF + 1);
		this->_caveX[i].sizeI = std::max(std::max(std::max(std::max(this->_sizeIni + (int)(this->_caveX[i].sizeF * this->_sizeModif), 1), this->_caveX[i].dirXI), this->_caveX[i].dirYI), this->_caveX[i].dirZI);
	}
}

void	cave::dig(int x, int y, int h, int size) {
	// for (int i = std::max(x - size - randIntBetween(size / 2) * signeN(x), 0); i < std::min(x + size + randIntBetween(size / 2) * signeP(x), 16 * (this->_sizeBiome - 1)); i++) {
	// 	for (int j = std::max(y - size - randIntBetween(size / 2) * signeN(y), 0); j < std::min(y + size + randIntBetween(size / 2) * signeP(y), 16 * (this->_sizeBiome - 1)); j++) {
	for (int i = std::max(x - size - randIntBetween(size / 2), 0); i < std::min(x + size + randIntBetween(size / 2), 16 * (this->_sizeBiome - 1)); i++) {
		for (int j = std::max(y - size - randIntBetween(size / 2), 0); j < std::min(y + size + randIntBetween(size / 2), 16 * (this->_sizeBiome - 1)); j++) {
			for (int l = std::max(h - size - randIntBetween(size / 4), 1); l < std::min(h + size + randIntBetween(size / 2), 255); l++) {
				this->_tab[i / 16][j / 16].impact = 1;
				this->_tab[i / 16][j / 16].toDel.push_back(gene3D(i % 16, j % 16, l));
			}	
		}
	}
}

void	cave::doDig(int x, int y, int h) {
	x *= 16;
	y *= 16;
	for (int i = 0; i < this->_lenght; i++) {
		h -= this->_caveX[i].dirZI;
		x += this->_caveX[i].dirXI;
		y += this->_caveX[i].dirYI;
		this->dig(x, y, h, this->_caveX[i].sizeI);
		this->_caveX[i].GP = 0;
	}
}


int		cave::getLenght() {
	return this->_lenght;
}

caveGP	cave::getCaveX(int i) {
	return this->_caveX[i];
}


void	cave::doNewCave(int xrand, int yrand, int x, int y, int sizeHoleIni, int lenght, int H) {
	this->_seed = randSeed(xrand + x, yrand + y);
	this->_sizeIni = sizeHoleIni;
	this->_lenght = lenght;

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

	this->doIniCave();
	this->doDig(x, y, H + (1 + this->_caveX[this->_lenght - 1].dirZF) * this->_sizeIni);
}