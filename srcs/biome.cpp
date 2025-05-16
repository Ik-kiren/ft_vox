#include "../includes/biome.hpp"

biome::biome() {}

biome::biome(int size, square sq) {
	this->_size = size;
	this->_nbr = size;
	this->_sizeZ = 256;
	this->_sq = sq;
	this->_heightMin = 32;
	this->_heightMax = 224;
	this->_heightDiff = this->_heightMax - this->_heightMin;
	this->_nbrGP = 8;
	this->_Hb1 = 6;
	this->_Hb2 = this->_Hb1 / 2;
	this->_Hb3 = this->_Hb1 / 4;
	this->_level = 2;
	this->_cave = NULL;
	this->_deleted = NULL;

	for (int i = 0; i < this->_size; i++) {
		std::vector<heightGP>	tmp;
		for (int j = 0; j < this->_size; j++) {
			tmp.emplace_back(heightGP());
		}
		this->_tab.emplace_back(tmp);
	}
}

biome::biome(biome &biSup, int x, int y) {
	this->_size = 16;
	this->_nbr = biSup._nbr - 1;
	this->_sizeZ = 256;
	this->_heightMin = biSup._heightMin;
	this->_heightMax = biSup._heightMax;
	this->_heightDiff = this->_heightMax - this->_heightMin;
	this->_sq = squarelvl1(x, y);
	this->_nbrGP = 8;
	this->_Hb1 = biSup._Hb1 * 6;
	this->_Hb2 = this->_Hb1 / 2;
	this->_Hb3 = this->_Hb1 / 4;
	this->_level = 1;
	this->_posTab = gene2DposTab(x, y, this->_nbr);
	this->_cave = NULL;
	this->_deleted = NULL;

	for (int i = 0; i < this->_size; i++) {
		std::vector<heightGP>	tmp;
		for (int j = 0; j < this->_size; j++) {
			tmp.emplace_back(heightGP());
		}
		this->_tab.emplace_back(tmp);
	}
	iniBiome1(biSup);
}

biome::~biome() {
	if (this->_cave) {
		delete this->_cave;
		this->_cave = NULL;
	}
	if (this->_deleted) {
		for (int i = 0; i < this->_nbr - 1; i++)
			delete [] this->_deleted[i];
		delete [] this->_deleted;
		this->_deleted = NULL;
	}
}

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
		std::cout << (int)this->_tab[x][y].arrayH[i];
	}
	std::cout << '\n';
}

void	biome::iniBiome() {
	this->_tab[0][0].GP = 2;
	this->_tab[0][this->_size - 1].GP = 2;
	this->_tab[this->_size - 1][0].GP = 2;
	this->_tab[this->_size - 1][this->_size - 1].GP = 2;

	float biome = 1.2;
	newSeed(this->_sq.NE.x, this->_sq.NE.y);
	// float	biome = 0.5 - randFloatBetween(0.5);
	this->_tab[0][0].heightF1 = randFloatBetween(biome);
	this->_tab[0][0].heightF2 = randFloatBetween(biome);
	this->_tab[0][0].heightF3 = randFloatBetween(biome);
	this->_tab[0][0].tempF = randFloatBetween(biome);
	this->_tab[0][0].waterF = randFloatBetween(biome);

	newSeed(this->_sq.NO.x, this->_sq.NO.y);
	// biome = 0.5 - randFloatBetween(0.5);
	this->_tab[0][this->_size - 1].heightF1 = randFloatBetween(biome);
	this->_tab[0][this->_size - 1].heightF2 = randFloatBetween(biome);
	this->_tab[0][this->_size - 1].heightF3 = randFloatBetween(biome);
	this->_tab[0][this->_size - 1].tempF = randFloatBetween(biome);
	this->_tab[0][this->_size - 1].waterF = randFloatBetween(biome);

	newSeed(this->_sq.SE.x, this->_sq.SE.y);
	// biome = 0.5 - randFloatBetween(0.5);
	this->_tab[this->_size - 1][0].heightF1 = randFloatBetween(biome);
	this->_tab[this->_size - 1][0].heightF2 = randFloatBetween(biome);
	this->_tab[this->_size - 1][0].heightF3 = randFloatBetween(biome);
	this->_tab[this->_size - 1][0].tempF = randFloatBetween(biome);
	this->_tab[this->_size - 1][0].waterF = randFloatBetween(biome);

	newSeed(this->_sq.SO.x, this->_sq.SO.y);
	// biome = 0.5 - randFloatBetween(0.5);
	this->_tab[this->_size - 1][this->_size - 1].heightF1 = randFloatBetween(biome);
	this->_tab[this->_size - 1][this->_size - 1].heightF2 = randFloatBetween(biome);
	this->_tab[this->_size - 1][this->_size - 1].heightF3 = randFloatBetween(biome);
	this->_tab[this->_size - 1][this->_size - 1].tempF = randFloatBetween(biome);
	this->_tab[this->_size - 1][this->_size - 1].waterF = randFloatBetween(biome);
}
 
void	biome::doGPCalculLine(int i, int j, int mid, int L, float H1, float H2, float H3) {
	if (this->_tab[i][mid].GP == 0) {
		this->_tab[i][mid].heightF1 = (this->_tab[i][L].heightF1 + this->_tab[i][j].heightF1) / 2 + randFloatBetween(H1);
		this->_tab[i][mid].heightF2 = (this->_tab[i][L].heightF2 + this->_tab[i][j].heightF2) / 2 + randFloatBetween(H2);
		this->_tab[i][mid].heightF3 = (this->_tab[i][L].heightF3 + this->_tab[i][j].heightF3) / 2 + randFloatBetween(H3);
		this->_tab[i][mid].tempF = (this->_tab[i][L].tempF + this->_tab[i][j].tempF) / 2 + randFloatBetween(H1);
		this->_tab[i][mid].waterF = (this->_tab[i][L].waterF + this->_tab[i][j].waterF) / 2 + randFloatBetween(H1);
	}
}

void	biome::doGPCalculColumn(int i, int j, int mid, int l, float H1, float H2, float H3) {
	if (this->_tab[mid][j].GP == 0) {
		this->_tab[mid][j].heightF1 = (this->_tab[l][j].heightF1 + this->_tab[i][j].heightF1) / 2 + randFloatBetween(H1);
		this->_tab[mid][j].heightF2 = (this->_tab[l][j].heightF2 + this->_tab[i][j].heightF2) / 2 + randFloatBetween(H2);
		this->_tab[mid][j].heightF3 = (this->_tab[l][j].heightF3 + this->_tab[i][j].heightF3) / 2 + randFloatBetween(H3);
		this->_tab[mid][j].tempF = (this->_tab[l][j].tempF + this->_tab[i][j].tempF) / 2 + randFloatBetween(H1);
		this->_tab[mid][j].waterF = (this->_tab[l][j].waterF + this->_tab[i][j].waterF) / 2 + randFloatBetween(H1);
	}
}

void	biome::doGPLine(int i, coord2d cd) {
	newSeed(cd.x, cd.y);
	float	H1 = 2 / this->_Hb1;
	float	H2 = 2 / this->_Hb2;
	float	H3 = 2 / this->_Hb3;
	int		L;

	for (int t = 0; t < this->_nbrGP; t++) {
		L = 0;
		for (int j = 1; j < this->_size; j++) {
			if (this->_tab[i][j].GP > 0) {
				doGPCalculLine(i, j, (j + L) / 2, L, H1, H2, H3);
				this->_tab[i][(j + L) / 2].GP = 1;
				L = j;
			}
		}
		H1 /= this->_Hb1;
		H2 /= this->_Hb2;
		H3 /= this->_Hb3;
	}
}

void	biome::doGPColumn(int j, coord2d cd) {
	newSeed(cd.x, cd.y);
	float	H1 = 2 / this->_Hb1;
	float	H2 = 2 / this->_Hb2;
	float	H3 = 2 / this->_Hb3;
	int		l;

	for (int t = 0; t < this->_nbrGP; t++) {
		l = 0;
		for (int i = 1; i < this->_size; i++) {
			if (this->_tab[i][j].GP > 0) {
				doGPCalculColumn(i, j, (i + l) / 2, l, H1, H2, H3);
				this->_tab[(i + l) / 2][j].GP = 1;
				l = i;
			}
		}
		H1 /= this->_Hb1;
		H2 /= this->_Hb2;
		H3 /= this->_Hb3;
	}
}
 

void	biome::doGP() {
	int		l;
	int 	L;
	float	H1 = 2 / this->_Hb1;
	float	H2 = 2 / this->_Hb2;
	float	H3 = 2 / this->_Hb3;

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
		H1 /= this->_Hb1;
		H2 /= this->_Hb2;
		H3 /= this->_Hb3;
	}

	for (int i = 0; i < this->_size; i++) {
		for (int j = 0; j < this->_size; j++) {
			this->_tab[i][j].heightF = (this->_tab[i][j].heightF1 + this->_tab[i][j].heightF2 / 2 + this->_tab[i][j].heightF3 / 4) / 1.75;
		}
	}
}

int	biome::whatTexture(int x, int y) {
	if (this->_tab[x][y].heightI < 120 + ((int)(this->_tab[x][y].waterF * 100) % 3)) {
		if (this->_tab[x][y].tempF < -0.15)
			return 6;
		else if (this->_tab[x][y].tempF > 0.15)
			return 3;
	} else if (this->_tab[x][y].heightI > 200 - ((int)(this->_tab[x][y].waterF * 100) % 4)) {
		if (this->_tab[x][y].tempF < -0.15)
			return 5;
		else if (this->_tab[x][y].tempF > 0.15)
			return 4;
	}

	if (this->_tab[x][y].tempF < -0.6)
		return 5;
	else if (this->_tab[x][y].tempF > 0.6) {
		return 4;
	} else {
		if (this->_tab[x][y].waterF > 0.5)
			return 7;
		else if (this->_tab[x][y].waterF < -0.5)
			return 1;
		return 8;
	}
}

std::array<unsigned char, sizeH>	biome::fillArray(int h, int texture) {
	std::array<unsigned char, sizeH>	ret;

	for (int i = 0; i < sizeH; i++) {
		if (i < h - 4)
			ret[i] = 2;
		else if (i > h) {
			if (i < 110)
				ret[i] = 9;
			else
				ret[i] = 0;
		} else {
			if (i == 109 && h == 109)
				ret[i] = 9;
			else
				ret[i] = texture;
		}
	}
	return ret;
}

int		biome::heightFtoI(float heightF, float denivele) {
	return ((heightF * std::abs(heightF)) + 1) / 2 * this->_heightDiff * (1 + denivele / 5) + this->_heightMin;
}

void	biome::afterGP() {
	for (int i = 0; i < this->_size; i++) {
		for (int j = 0; j < this->_size; j++) {
			this->_tab[i][j].heightF = (this->_tab[i][j].heightF1 + this->_tab[i][j].heightF2 / 2 + this->_tab[i][j].heightF3 / 4) / 1.75;
			this->_tab[i][j].heightI = this->heightFtoI(this->_tab[i][j].heightF, this->_tab[i][j].waterF);
			this->_tab[i][j].texture = whatTexture(i, j);
			this->_tab[i][j].arrayH = fillArray(this->_tab[i][j].heightI, this->_tab[i][j].texture);	
		}
	}
}

std::array<unsigned char, sizeH>	&biome::getArray(int x, int y) {
	return this->_tab[x][y].arrayH;
}

chunk	biome::voxelToChunk(int c) {
	chunk toRet;
	unsigned char	***ret = new unsigned char**[this->_size];

	for (int i = 0; i < this->_size; i++) {
		ret[i] = new unsigned char*[this->_size];
		for (int j = 0; j < this->_size; j++) {
			ret[i][j] = new unsigned char[this->_size];
			for (int k = 0; k < this->_size; k++) {
				ret[i][j][k] = this->_tab[i][j].arrayH[c * this->_size + k];
			}
		}
	}
	toRet.voxel = ret;
	return toRet;
}

void	biome::voxelToChunk(unsigned char ****ch) {
	for (int i = 0; i < this->_size; i++) {
		unsigned char	***tmp0 = ch[i];
		for (int j = 0; j < this->_size; j++) {
			unsigned char	**tmp1 = tmp0[j];
			for (int k = 0; k < this->_size; k++) {
				unsigned char	*tmp2 = tmp1[k];
				for (int l = 0; l < this->_size; l++)
					tmp2[l] = this->_tab[j][k].arrayH[i * this->_size + l];
			}
		}
	}
}

void	biome::iniMyBiome(biome &biSup, int x, int y) {
	for (int i = 0; i < this->_size; i++) {
		for (int j = 0; j < this->_size; j++) {
			this->_tab[i][j].GP = 0;
		}
	}
	this->_sq = squarelvl1(x, y);
	this->_posTab = gene2DposTab(x, y, this->_nbr);
	iniBiome1(biSup);
}

void	biome::iniBiome1(biome &biSup) {
	this->_tab[0][0].GP = 2;
	this->_tab[0][this->_size - 1].GP = 2;
	this->_tab[this->_size - 1][0].GP = 2;
	this->_tab[this->_size - 1][this->_size - 1].GP = 2;

	this->_tab[0][0].heightF1 = biSup._tab[this->_posTab.x][this->_posTab.y].heightF1;
	this->_tab[0][0].heightF2 = biSup._tab[this->_posTab.x][this->_posTab.y].heightF2;
	this->_tab[0][0].heightF3 = biSup._tab[this->_posTab.x][this->_posTab.y].heightF3;
	this->_tab[0][0].tempF = biSup._tab[this->_posTab.x][this->_posTab.y].tempF;
	this->_tab[0][0].waterF = biSup._tab[this->_posTab.x][this->_posTab.y].waterF;

	this->_tab[0][this->_size - 1].heightF1 = biSup._tab[this->_posTab.x][this->_posTab.y + 1].heightF1;
	this->_tab[0][this->_size - 1].heightF2 = biSup._tab[this->_posTab.x][this->_posTab.y + 1].heightF2;
	this->_tab[0][this->_size - 1].heightF3 = biSup._tab[this->_posTab.x][this->_posTab.y + 1].heightF3;
	this->_tab[0][this->_size - 1].tempF = biSup._tab[this->_posTab.x][this->_posTab.y + 1].tempF;
	this->_tab[0][this->_size - 1].waterF = biSup._tab[this->_posTab.x][this->_posTab.y + 1].waterF;

	this->_tab[this->_size - 1][0].heightF1 = biSup._tab[this->_posTab.x + 1][this->_posTab.y].heightF1;
	this->_tab[this->_size - 1][0].heightF2 = biSup._tab[this->_posTab.x + 1][this->_posTab.y].heightF2;
	this->_tab[this->_size - 1][0].heightF3 = biSup._tab[this->_posTab.x + 1][this->_posTab.y].heightF3;
	this->_tab[this->_size - 1][0].tempF = biSup._tab[this->_posTab.x + 1][this->_posTab.y].tempF;
	this->_tab[this->_size - 1][0].waterF = biSup._tab[this->_posTab.x + 1][this->_posTab.y].waterF;

	this->_tab[this->_size - 1][this->_size - 1].heightF1 = biSup._tab[this->_posTab.x + 1][this->_posTab.y + 1].heightF1;
	this->_tab[this->_size - 1][this->_size - 1].heightF2 = biSup._tab[this->_posTab.x + 1][this->_posTab.y + 1].heightF2;
	this->_tab[this->_size - 1][this->_size - 1].heightF3 = biSup._tab[this->_posTab.x + 1][this->_posTab.y + 1].heightF3;
	this->_tab[this->_size - 1][this->_size - 1].tempF = biSup._tab[this->_posTab.x + 1][this->_posTab.y + 1].tempF;
	this->_tab[this->_size - 1][this->_size - 1].waterF = biSup._tab[this->_posTab.x + 1][this->_posTab.y + 1].waterF;
}
 

void	biome::doGPlvl1() {
	int		l;
	int 	L;
	float	H1 = 2 / this->_Hb1;
	float	H2 = 2 / this->_Hb2;
	float	H3 = 2 / this->_Hb3;

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
		H1 /= this->_Hb1;
		H2 /= this->_Hb2;
		H3 /= this->_Hb3;
	}
	for (int i = 0; i < this->_size; i++) {
		this->_tab[i][0].GP = 0;
		doGPCalculLine(i, 0, 0, 1, H1, H2, H3);
		this->_tab[i][0].GP = 2;
	}
	for (int i = 0; i < this->_size; i++) {
		this->_tab[0][i].GP = 0;
		doGPCalculColumn(0, i, 0, 1, H1, H2, H3);
		this->_tab[0][i].GP = 2;
	}
	afterGP();
}

square	biome::getSquare() {
	return this->_sq;	
}

cave	*biome::getCave() {
	return this->_cave;
}

void	biome::setCaves(int xrand, int yrand, biome &biSup) {
	this->_cave = new cave(this->_nbr, 300);
	randSeed(xrand, yrand);
	int nbr = randIntBetween(3) + 7;
	for (int i = 0; i < nbr; i++) {
		int	x = randIntBetween(this->_nbr / 2 - 2) + this->_nbr / 2;
		int	y = randIntBetween(this->_nbr / 2 - 2) + this->_nbr / 2;
		int Hint = this->heightFtoI(biSup.getHeightF(x, y), 0);
		int	lenght = randIntBetween(100) + 200;
		this->_cave->doNewCave(xrand, yrand, x, y, 3, lenght, Hint);
	}
}

void	biome::setDeleted() {
	playerDig	**ret = new playerDig*[this->_nbr - 1];
	for (int i = 0; i < this->_nbr - 1; i++) {
		ret[i] = new playerDig[this->_nbr - 1];
		for (int j = 0; j < this->_nbr - 1; j++)
			ret[i][j].impact = 0;
	}
	this->_deleted = ret;
}

float	biome::getHeightF(int x, int y) {
	return this->_tab[x][y].heightF;
}

void	biome::dig(biome &biSup) {
	if (biSup._cave->_tab[this->_posTab.x][this->_posTab.y].impact == 1) {
		int i;
		int j;
		int	k;
		for (int l = 0; l < biSup._cave->_tab[this->_posTab.x][this->_posTab.y].toDel.size(); l++) {
			i = biSup._cave->_tab[this->_posTab.x][this->_posTab.y].toDel[l].x;
			j = biSup._cave->_tab[this->_posTab.x][this->_posTab.y].toDel[l].y;
			k = biSup._cave->_tab[this->_posTab.x][this->_posTab.y].toDel[l].z;
			if (this->_tab[i][j].arrayH[k] != 9)
				this->_tab[i][j].arrayH[k] = 0;
		}
	}
	// if (biSup._deleted[this->_posTab.x][this->_posTab.y].impact == 1) {
	// 	int i;
	// 	int j;
	// 	int	k;
	// 	for (int l = 0; l < biSup._deleted[this->_posTab.x][this->_posTab.y].toDel.size(); l++) {
	// 		i = biSup._deleted[this->_posTab.x][this->_posTab.y].toDel[l].x;
	// 		j = biSup._deleted[this->_posTab.x][this->_posTab.y].toDel[l].y;
	// 		k = biSup._deleted[this->_posTab.x][this->_posTab.y].toDel[l].z;
	// 		if (this->_tab[i][j].arrayH[k] != 9)
	// 			this->_tab[i][j].arrayH[k] = 0;
	// 	}
	// }
}

void	biome::deleteCube(int a, int b, int c, int d, int z) {
	this->_deleted[a][b].impact = 1;
	this->_deleted[a][b].toDel.push_back(gene3D(c, d, z - 1));
	this->_deleted[a][b].toDel.push_back(gene3D(c, d, z));
	this->_deleted[a][b].toDel.push_back(gene3D(c, d, z + 1));
}