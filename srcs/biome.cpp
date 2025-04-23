#include "../includes/biome.hpp"

biome::biome() {}

biome::biome(int size, square sq) {
	this->_size = size;
	this->_nbr = size;
	this->_sizeZ = 256;
	this->_heightMin = 32;
	this->_heightMax = 224;
	// this->_heightMin = 120;
	// this->_heightMax = 130;
	this->_heightDiff = this->_heightMax - this->_heightMin;
	this->_sq = sq;
	this->_nbrGP = 8;
	this->_Hb = 6;
	this->_level = 2;
	this->_cave = NULL;

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

biome::biome(biome &biSup, int x, int y) {
	this->_size = 16;
	this->_nbr = biSup._nbr - 1;
	this->_sizeZ = 256;
	this->_heightMin = biSup._heightMin;
	this->_heightMax = biSup._heightMax;
	this->_heightDiff = this->_heightMax - this->_heightMin;
	this->_sq = squarelvl1(biSup._sq, x, y);
	this->_nbrGP = 8;
	this->_Hb = biSup._Hb * 6;
	this->_level = 1;
	this->_cave = NULL;

	for (int i = 0; i < this->_size; i++) {
		std::vector<heightGP>	tmp;
		for (int j = 0; j < this->_size; j++) {
			heightGP*	tmpS = new heightGP();
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
	iniBiome1(biSup, x, y);
}

biome::~biome() {
	if (!this->_cave)
		return ;
	delete this->_cave;
	this->_cave = NULL;
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

	newSeed(this->_sq.NE.x, this->_sq.NE.y);
	float	biome = 0.5 - randFloatBetween(0.5);
	this->_tab[0][0].heightF1 = randFloatBetween(biome);
	this->_tab[0][0].heightF2 = randFloatBetween(biome);
	this->_tab[0][0].heightF3 = randFloatBetween(biome);
	this->_tab[0][0].tempF = randFloatBetween(biome);

	newSeed(this->_sq.NO.x, this->_sq.NO.y);
	biome = 0.5 - randFloatBetween(0.5);
	this->_tab[0][this->_size - 1].heightF1 = randFloatBetween(biome);
	this->_tab[0][this->_size - 1].heightF2 = randFloatBetween(biome);
	this->_tab[0][this->_size - 1].heightF3 = randFloatBetween(biome);
	this->_tab[0][this->_size - 1].tempF = randFloatBetween(biome);

	newSeed(this->_sq.SE.x, this->_sq.SE.y);
	biome = 0.5 - randFloatBetween(0.5);
	this->_tab[this->_size - 1][0].heightF1 = randFloatBetween(biome);
	this->_tab[this->_size - 1][0].heightF2 = randFloatBetween(biome);
	this->_tab[this->_size - 1][0].heightF3 = randFloatBetween(biome);
	this->_tab[this->_size - 1][0].tempF = randFloatBetween(biome);

	newSeed(this->_sq.SO.x, this->_sq.SO.y);
	biome = 0.5 - randFloatBetween(0.5);
	this->_tab[this->_size - 1][this->_size - 1].heightF1 = randFloatBetween(biome);
	this->_tab[this->_size - 1][this->_size - 1].heightF2 = randFloatBetween(biome);
	this->_tab[this->_size - 1][this->_size - 1].heightF3 = randFloatBetween(biome);
	this->_tab[this->_size - 1][this->_size - 1].tempF = randFloatBetween(biome);
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
	newSeed(cd.x, cd.y);
	float	H1 = 2 / this->_Hb;
	float	H2 = 2 / (this->_Hb / 2);
	float	H3 = 2 / (this->_Hb / 4);
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
		H1 /= this->_Hb;
		H2 /= (this->_Hb / 2);
		H3 /= (this->_Hb / 4);
	}
}

void	biome::doGPColumn(int j, coord2d cd) {
	newSeed(cd.x, cd.y);
	float	H1 = 2 / this->_Hb;
	float	H2 = 2 / (this->_Hb / 2);
	float	H3 = 2 / (this->_Hb / 4);
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

	for (int i = 0; i < this->_size; i++) {
		for (int j = 0; j < this->_size; j++) {
			this->_tab[i][j].heightF = (this->_tab[i][j].heightF1 + this->_tab[i][j].heightF2 / 2 + this->_tab[i][j].heightF3 / 4) / 1.75;
		}
	}
}

int	biome::whatTexture(int x, int y) {
	if (this->_tab[x][y].heightI < 64)
		return 8;
	if (this->_tab[x][y].tempF < -0.5) {
		if (this->_tab[x][y].heightF < 0)
			return 6;
		return 5;
	} else if (this->_tab[x][y].tempF > 0.5) {
		if (this->_tab[x][y].heightF < 0)
			return 3;
		return 4;
	} else if (this->_tab[x][y].tempF < 0) {
		if (this->_tab[x][y].heightF < 0)
			return 1;
		return 9;
	} else {
		if (this->_tab[x][y].heightF < 0)
			return 9;
		return 7;
	}
}

std::array<unsigned char, sizeH>	biome::fillArray(int h, int texture) {
	std::array<unsigned char, sizeH>	ret;

	for (int i = 0; i < sizeH; i++) {
		if (i < h)
			ret[i] = 2;
		else if (i > h)
			ret[i] = 0;
		else
			ret[i] = texture;
	}
	return ret;
}

int		biome::heightFtoI(float heightF) {
	return ((heightF * std::abs(heightF)) + 1) / 2 * this->_heightDiff + this->_heightMin;
}

void	biome::afterGP() {
	for (int i = 0; i < this->_size; i++) {
		for (int j = 0; j < this->_size; j++) {
			this->_tab[i][j].heightF = (this->_tab[i][j].heightF1 + this->_tab[i][j].heightF2 / 2 + this->_tab[i][j].heightF3 / 4) / 1.75;
			this->_tab[i][j].heightI = this->heightFtoI(this->_tab[i][j].heightF);
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

void	biome::iniBiome1(biome &biSup, int a, int b) {
	int	x = (a + signeN(a)) % this->_nbr;
	int	y = (b + signeN(b)) % this->_nbr;

	if (a < 0)
		x = this->_nbr - 1 + x;
	if (b < 0)
		y = this->_nbr - 1 + y;

	this->_tab[0][0].GP = 2;
	this->_tab[0][this->_size - 1].GP = 2;
	this->_tab[this->_size - 1][0].GP = 2;
	this->_tab[this->_size - 1][this->_size - 1].GP = 2;

	this->_tab[0][0].heightF1 = biSup._tab[x][y].heightF1;
	this->_tab[0][0].heightF2 = biSup._tab[x][y].heightF2;
	this->_tab[0][0].heightF3 = biSup._tab[x][y].heightF3;
	this->_tab[0][0].tempF = biSup._tab[x][y].tempF;

	this->_tab[0][this->_size - 1].heightF1 = biSup._tab[x][y + 1].heightF1;
	this->_tab[0][this->_size - 1].heightF2 = biSup._tab[x][y + 1].heightF2;
	this->_tab[0][this->_size - 1].heightF3 = biSup._tab[x][y + 1].heightF3;
	this->_tab[0][this->_size - 1].tempF = biSup._tab[x][y + 1].tempF;

	this->_tab[this->_size - 1][0].heightF1 = biSup._tab[x + 1][y].heightF1;
	this->_tab[this->_size - 1][0].heightF2 = biSup._tab[x + 1][y].heightF2;
	this->_tab[this->_size - 1][0].heightF3 = biSup._tab[x + 1][y].heightF3;
	this->_tab[this->_size - 1][0].tempF = biSup._tab[x + 1][y].tempF;

	this->_tab[this->_size - 1][this->_size - 1].heightF1 = biSup._tab[x + 1][y + 1].heightF1;
	this->_tab[this->_size - 1][this->_size - 1].heightF2 = biSup._tab[x + 1][y + 1].heightF2;
	this->_tab[this->_size - 1][this->_size - 1].heightF3 = biSup._tab[x + 1][y + 1].heightF3;
	this->_tab[this->_size - 1][this->_size - 1].tempF = biSup._tab[x + 1][y + 1].tempF;
}
 

void	biome::doGPlvl1() {
	int		l;
	int 	L;
	float	H1 = 2 / this->_Hb;
	float	H2 = 2 / (this->_Hb / 2);
	float	H3 = 2 / (this->_Hb / 4);

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
	// for (int i = 0; i < this->_size; i++) {
	// 	this->_tab[i][0].GP = 0;
	// 	doGPCalculLine(i, 0, 0, 1, H1, H2, H3);
	// 	this->_tab[i][0].GP = 2;
	// }
	// for (int i = 0; i < this->_size; i++) {
	// 	this->_tab[0][i].GP = 0;
	// 	doGPCalculColumn(0, i, 0, 1, H1, H2, H3);
	// 	this->_tab[0][i].GP = 2;
	// }
	afterGP();
}

square	biome::getSquare() {
	return this->_sq;	
}

cave	*biome::getCave() {
	return this->_cave;
}

void	biome::setCave(int x, int y, int size, int lenght, float H) {
	int Hint = this->heightFtoI(H);
	this->_cave = new cave(x, y, size, lenght, Hint, this->_nbr);
}

float	biome::getHeightF(int x, int y) {
	return this->_tab[x][y].heightF;
}

void	biome::dig(biome &biSup, int a, int b) {
	int	x = (a + signeN(a)) % this->_nbr;
	int	y = (b + signeN(b)) % this->_nbr;

	if (a < 0)
		x = this->_nbr - 1 + x;
	if (b < 0)
		y = this->_nbr - 1 + y;

		if (biSup._cave->_tab[x][y].impact == 0)
		return ;
	int i;
	int j;
	int	k;
	for (int l = 0; l < biSup._cave->_tab[x][y].toDel.size(); l++) {
		i = biSup._cave->_tab[x][y].toDel[l].x;
		j = biSup._cave->_tab[x][y].toDel[l].y;
		k = biSup._cave->_tab[x][y].toDel[l].z;
		this->_tab[i][j].arrayH[k] = 0;
	}
}
