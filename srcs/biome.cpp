#include "../includes/biome.hpp"

biome::biome() {}

biome::biome(int size, square sq) {
	this->_size = size;
	this->_sizeZ = 256;
	this->_heightMin = 64;
	this->_heightMax = 192;	
	this->_heightDiff = this->_heightMax - this->_heightMin;
	this->_sq = sq;
	this->_nbrGP = 8;
	this->_Hb = 6;

	for (int i = 0; i < this->_size; i++) {
		std::vector<heightGP>	tmp;
		for (int j = 0; j < this->_size; j++) {
			heightGP*	tmpS = new heightGP();	//new to force heap, maybe not necessary ?
			// heightGP*	tmpS;
			tmpS->GP = 0;
			tmpS->heightF1 = 0;
			tmpS->heightF2 = 0;
			tmpS->heightF3 = 0;
			tmpS->tempF = 0;
			tmpS->heightI = 0;
			tmpS->texture = '0';
			tmp.push_back(*tmpS);
			delete tmpS;
		}
		this->_tab.push_back(tmp);
	}
}

biome::~biome() {}

void	biome::printTab(int mode) {
	if (mode == 1) {
		for (int i = 0; i < this->_size; i++) {
			for (int j = 0; j < this->_size; j++) {
				if (this->_tab[i][j].heightF > 1.2)
					printf("\033[1;31m");
				else if (this->_tab[i][j].heightF > 1)
					printf("\033[0;31m");
				else if (this->_tab[i][j].heightF > 0.75)
					printf("\033[1;34m");
				else if (this->_tab[i][j].heightF > 0.5)
					printf("\033[0;34m");
				else if (this->_tab[i][j].heightF > 0.25)
					printf("\033[1;30m");
				else if (this->_tab[i][j].heightF > 0)
					printf("\033[0;30m");
				else if (this->_tab[i][j].heightF < -1.25)
					printf("\033[1;33m");
				else if (this->_tab[i][j].heightF < -1)
					printf("\033[0;33m");
				else if (this->_tab[i][j].heightF < -0.75)
					printf("\033[1;35m");
				else if (this->_tab[i][j].heightF < -0.5)
					printf("\033[0;35m");
				else if (this->_tab[i][j].heightF < -0.25)
					printf("\033[1;32m");
				else if (this->_tab[i][j].heightF <= 0)
					printf("\033[0;32m");
				std::cout << this->_tab[i][j].texture;
				printf("\033[0m");
			}
			std::cout << '\n';
		}
	} else if (mode == 0) {
		for (int i = 0; i < this->_size; i++) {
			for (int j = 0; j < this->_size; j++) {
				std::cout << this->_tab[i][j].heightI;
				std::cout << ' ';
			}
			std::cout << '\n';
		} 
	} else if (mode == 2) {
		for (int i = 0; i < this->_size; i++) {
			for (int j = 0; j < this->_size; j++) {
				if (this->_tab[i][j].texture == 1)
					printf("\033[0;31m");
				else if (this->_tab[i][j].texture == 2)
					printf("\033[1;31m");
				else if (this->_tab[i][j].texture == 3)
					printf("\033[0;33m");
				else if (this->_tab[i][j].texture == 4)
					printf("\033[1;33m");
				else if (this->_tab[i][j].texture == 5)
					printf("\033[0;35m");
				else if (this->_tab[i][j].texture == 6)
					printf("\033[1;35m");
				std::cout << this->_tab[i][j].texture;
				printf("\033[0m");
			}
			std::cout << '\n';
		}
	} else if (mode == 3) {
		int	x = 128;
		int	y = 128;
		for (int i = 0; i < 256; i++) {
			std::cout << static_cast<int>(this->_tab[x][y].arrayH[i]);
		}
		std::cout << '\n' << this->_tab[x][y].heightI;
		std::cout << '\n' << this->_tab[x][y].heightF;
	}
	std::cout << '\n';
}

void	biome::printCave(int x, int y) {
	for (int i = 0; i < 256; i++) {
		std::cout << this->_tab[x][y].arrayH[i];
	}
	std::cout << '\n';
}

void	biome::iniBiome() {
	this->_tab[0][0].GP = 2;
	this->_tab[0][this->_size - 1].GP = 2;
	this->_tab[this->_size - 1][0].GP = 2;
	this->_tab[this->_size - 1][this->_size - 1].GP = 2;

	newSeed(this->_sq.NE.x, this->_sq.NE.y);
	this->_tab[0][0].heightF1 = randFloatBetween(1);
	this->_tab[0][0].heightF2 = randFloatBetween(1);
	this->_tab[0][0].heightF3 = randFloatBetween(1);
	this->_tab[0][0].tempF = randFloatBetween(1);
	newSeed(this->_sq.NO.x, this->_sq.NO.y);
	this->_tab[0][this->_size - 1].heightF1 = randFloatBetween(1);
	this->_tab[0][this->_size - 1].heightF2 = randFloatBetween(1);
	this->_tab[0][this->_size - 1].heightF3 = randFloatBetween(1);
	this->_tab[0][this->_size - 1].tempF = randFloatBetween(1);
	newSeed(this->_sq.SE.x, this->_sq.SE.y);
	this->_tab[this->_size - 1][0].heightF1 = randFloatBetween(1);
	this->_tab[this->_size - 1][0].heightF2 = randFloatBetween(1);
	this->_tab[this->_size - 1][0].heightF3 = randFloatBetween(1);
	this->_tab[this->_size - 1][0].tempF = randFloatBetween(1);
	newSeed(this->_sq.SO.x, this->_sq.SO.y);
	this->_tab[this->_size - 1][this->_size - 1].heightF1 = randFloatBetween(1);
	this->_tab[this->_size - 1][this->_size - 1].heightF2 = randFloatBetween(1);
	this->_tab[this->_size - 1][this->_size - 1].heightF3 = randFloatBetween(1);
	this->_tab[this->_size - 1][this->_size - 1].tempF = randFloatBetween(1);
}
 
void	biome::doGPCalculLine(int i, int j, int mid, int L, float H1, float H2, float H3) {
	if (this->_tab[i][mid].GP == 0) {
		this->_tab[i][mid].heightF1 = (this->_tab[i][L].heightF1 + this->_tab[i][j].heightF1) / 2 + randFloatBetween(H1);
		this->_tab[i][mid].heightF2 = (this->_tab[i][L].heightF2 + this->_tab[i][j].heightF2) / 2 + randFloatBetween(H2);
		this->_tab[i][mid].heightF3 = (this->_tab[i][L].heightF3 + this->_tab[i][j].heightF3) / 2 + randFloatBetween(H3);
		this->_tab[i][mid].tempF = (this->_tab[i][L].tempF + this->_tab[i][j].tempF) / 2 + randFloatBetween(H1);;

	}
}

void	biome::doGPCalculColumn(int i, int j, int mid, int l, float H1, float H2, float H3) {
	if (this->_tab[mid][j].GP == 0) {
		this->_tab[mid][j].heightF1 = (this->_tab[l][j].heightF1 + this->_tab[i][j].heightF1) / 2 + randFloatBetween(H1);
		this->_tab[mid][j].heightF2 = (this->_tab[l][j].heightF2 + this->_tab[i][j].heightF2) / 2 + randFloatBetween(H2);
		this->_tab[mid][j].heightF3 = (this->_tab[l][j].heightF3 + this->_tab[i][j].heightF3) / 2 + randFloatBetween(H3);
		this->_tab[mid][j].tempF = (this->_tab[l][j].tempF + this->_tab[i][j].tempF) / 2 + randFloatBetween(H1);
	}
}

void	biome::doGPLine(int i, coord2d cd) {
	float	H1 = 2 / this->_Hb;
	float	H2 = 2 / (this->_Hb / 2);
	float	H3 = 2 / (this->_Hb / 4);
	int		L;

	for (int t = 0; t < this->_nbrGP; t++) {
		L = 0;
		newSeed(cd.x, cd.y);
		for (int j = 1; j < this->_size; j++) {
			if (this->_tab[i][j].GP > 0) {
				doGPCalculLine(i, j, (j + L) / 2, L, H1, H2, H3);
				this->_tab[i][(j + L) / 2].GP = 1;
				L = j;
			}
		}
		H1 /= this->_Hb;
		H2 /= (this->_Hb / 2);
		H3 /= (this->_Hb / 4);
	}
}

void	biome::doGPColumn(int j, coord2d cd) {
	float	H1 = 2 / this->_Hb;
	float	H2 = 2 / (this->_Hb / 2);
	float	H3 = 2 / (this->_Hb / 4);
	int		l;

	for (int t = 0; t < this->_nbrGP; t++) {
		l = 0;
		newSeed(cd.x, cd.y);
		for (int i = 1; i < this->_size; i++) {
			if (this->_tab[i][j].GP > 0) {
				doGPCalculColumn(i, j, (i + l) / 2, l, H1, H2, H3);
				this->_tab[(i + l) / 2][j].GP = 1;
				l = i;
			}
		}
		H1 /= this->_Hb;
		H2 /= (this->_Hb / 2);
		H3 /= (this->_Hb / 4);
	}
}

void	biome::doGP() {
	int		l;
	int 	L;
	float	H1 = 2 / this->_Hb;
	float	H2 = 2 / (this->_Hb / 2);
	float	H3 = 2 / (this->_Hb / 4);

	iniBiome();
	doGPLine(0, this->_sq.NE);
	doGPLine(this->_size - 1, this->_sq.SE);
	doGPColumn(0, this->_sq.NE);
	doGPColumn(this->_size - 1, this->_sq.NO);

	for (int t = 0; t < this->_nbrGP; t++) {
		for (int i = 0; i < this->_size; i++) {
			L = 0;
			for (int j = 1; j < this->_size; j++) {
				if (this->_tab[i][j].GP == 2) {
					doGPCalculLine(i, j, (j + L) / 2, L, H1, H2, H3);
					this->_tab[i][(j + L) / 2].GP = 2;
					L = j;
				}
			}
		}
		for (int j = 0; j < this->_size; j++) {
			l = 0;
			for (int i = 1; i < this->_size; i++) {
				if (this->_tab[i][j].GP == 2) {
					doGPCalculColumn(i, j, (i + l) / 2, l, H1, H2, H3);
					this->_tab[(i + l) / 2][j].GP = 2;
					l = i;
				}
			}
		}
		H1 /= this->_Hb;
		H2 /= (this->_Hb / 2);
		H3 /= (this->_Hb / 4);
	}
	afterGP();
	doCave(128, 128);
}

int	biome::whatTexture(int x, int y) {
	if (this->_tab[x][y].tempF < -0.5) {
		if (this->_tab[x][y].heightF < 0)
			return 1;
		return 2;
	} else if (this->_tab[x][y].tempF > 0.5) {
		if (this->_tab[x][y].heightF < 0)
			return 7;
		return 8;
	} else if (this->_tab[x][y].tempF < 0) {
		if (this->_tab[x][y].heightF < 0)
			return 3;
		return 4;
	} else {
		if (this->_tab[x][y].heightF < 0)
			return 5;
		return 6;
	}
}

std::array<unsigned char, sizeH>	biome::fillArray(int h, int texture) {
	std::array<unsigned char, sizeH>	ret;

	for (int i = 0; i < sizeH; i++) {
		if (i < h)
			ret[i] = texture;
		else
			ret[i] = 0;
	}
	return ret;
}

void	biome::afterGP() {
	for (int i = 0; i < this->_size; i++) {
		for (int j = 0; j < this->_size; j++) {
			this->_tab[i][j].heightF = (this->_tab[i][j].heightF1 + this->_tab[i][j].heightF2 / 2 + this->_tab[i][j].heightF3 / 4) / 1.75;
			this->_tab[i][j].heightI = (this->_tab[i][j].heightF + 1.5) / 3 * this->_heightDiff + this->_heightMin;
			this->_tab[i][j].texture = whatTexture(i, j);
			this->_tab[i][j].arrayH = fillArray(this->_tab[i][j].heightI, this->_tab[i][j].texture);	
		}
	}
}

void	biome::dig(int x, int y, int h, int size) {
	for (int i = std::max(x - size, 0); i < std::min(x + size, this->_size); i++) {
		for (int j = std::max(y - size, 0); j < std::min(y + size, this->_size); j++) {
			for (int l = std::max(h - size, 0); l < std::min(h + size, this->_sizeZ); l++)
				this->_tab[i][j].arrayH[l] = 0;
		}
	}
}

void	biome::doCave(int x, int y) {
	cave	c(this->_sq.NE.x, this->_sq.NE.y, 3, 50);
	int		h = this->_tab[x][y].heightI;

	c.doIniCave();
	for (int k = 0; k < c.getLenght(); k++) {
		h -= c.getCaveX(k).dirZI;
		x += c.getCaveX(k).dirXI;
		y += c.getCaveX(k).dirYI;
		// if (this->_sq.NE.x == 1 && this->_sq.NE.y == 1)
		// 	std::cout << k << " " << c.getCaveX(k).dirZI << " " << c.getCaveX(k).dirXI << " " << c.getCaveX(k).dirYI << " " << c.getCaveX(k).sizeI << '\n';
		this->dig(x, y, h, c.getCaveX(k).sizeI);
	}
}

std::array<unsigned char, sizeH>	&biome::getArray(int x, int y) {
	return this->_tab[x][y].arrayH;
}

chunk	biome::voxelToChunk(int a, int b, int c) {
	chunk toRet;
	unsigned char	***ret = new unsigned char**[16];

	for (int i = 0; i < 16; i++) {
		ret[i] = new unsigned char*[16];
		for (int j = 0; j < 16; j++) {
			ret[i][j] = new unsigned char[16];
			for (int k = 0; k < 16; k++) {
				ret[i][j][k] = this->_tab[a * 16 + i][b * 16 + j].arrayH[c * 16 + k];
			}
		}
	}
	toRet.voxel = ret;
	return toRet;
}