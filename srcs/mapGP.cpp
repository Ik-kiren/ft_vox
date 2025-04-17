#include "../includes/mapGP.hpp"
#include <sys/time.h>

mapGP::mapGP() {}

mapGP::mapGP(int size, int sizeBiome) {
	this->_sizeIni = size;
	this->_sizeH = this->_sizeIni + 2;
	this->_sizeL = this->_sizeIni + 2;
	this->_sizeZ = 256;
	this->_sizeBiome = sizeBiome;

	for (int i = -this->_sizeH / 2; i < this->_sizeH / 2; i++) {
		std::vector<biomeGP>	tmp;
		for (int j = -this->_sizeL / 2; j < this->_sizeL / 2; j++) {
	// for (int i = 0; i < this->_sizeH; i++) {
	// 	std::vector<biomeGP>	tmp;
	// 	for (int j = 0; j < this->_sizeL; j++) {
			biomeGP	tmpS;	
			tmpS.sq.NE = gene2D(i, j);
			tmpS.sq.NO = gene2D(i, j + 1);
			tmpS.sq.SE = gene2D(i + 1, j);
			tmpS.sq.SO = gene2D(i + 1, j + 1);
			tmpS.bio = biome(this->_sizeBiome, tmpS.sq);
			tmpS.GP = 0;
			tmp.push_back(tmpS);
		}
		this->_tab.push_back(tmp);
	}
	doGPIniBiome();
}

mapGP::~mapGP() {}

void	mapGP::printGP() {
	for (int i = 0; i < this->_sizeH; i++) {
		for (int j = 0; j < this->_sizeL; j++) {
			if (this->_tab[i][j].GP == 0)
				printf("\033[1;31m");
			else
				printf("\033[1;32m");
			std::cout << "(" << this->_tab[i][j].sq.NE.x << ",";
			std::cout << this->_tab[i][j].sq.NE.y << ") ";
		}
		std::cout << std::endl;
		printf("\033[0m");
	}
}

void	mapGP::printSquareGP(int x, int y, int mode) {
	this->_tab[x][y].bio.printTab(mode);
}

void	mapGP::printCave(int x, int y, int a, int b) {
	this->_tab[x][y].bio.printCave(a, b);
}

void	mapGP::doGPIniBiome() {
	for (int i = 1; i < this->_sizeH - 1; i++) {
		for (int j = 1; j < this->_sizeL - 1; j++) {
			this->_tab[i][j].bio.doGP();
			this->_tab[i][j].GP = 1;
		}
	}
}

coord2d	mapGP::findCoord(int x, int y) {
	for (int i = 0; i < this->_sizeH; i++) {
		for (int j = 0; j < this->_sizeL; j++) {
			if (this->_tab[i][j].sq.NE.x == x && this->_tab[i][j].sq.NE.y == y)
				return gene2D(i, j);
		}
	}
	return gene2D(x, y);
}

biomeGP	mapGP::createEmpty(int x, int y) {
	biomeGP	ret;

	ret.sq.NE = gene2D(x, y);
	ret.sq.NO = gene2D(x, y + 1);
	ret.sq.SE = gene2D(x + 1, y);
	ret.sq.SO = gene2D(x + 1, y + 1);
	ret.GP = 0;
	return ret;
}

void	mapGP::checkAround(int x, int y) {
	coord2d tmp = findCoord(x, y);
	x = tmp.x;
	y = tmp.y;

	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (this->_tab[i][j].GP == 0) {
				this->_tab[i][j].bio = biome(this->_sizeBiome, this->_tab[i][j].sq);
				this->_tab[i][j].bio.doGP();
				this->_tab[i][j].GP = 1;

				if (j == this->_sizeL - 1) {
					for (int k = 0; k < this->_sizeH; k++) {
						this->_tab[k].push_back(createEmpty(this->_tab[k][j].sq.NE.x, this->_tab[k][j].sq.NE.y + 1));
					}
					this->_sizeL += 1;
				}

				if (j == 0) {
					for (int k = 0; k < this->_sizeH; k++) {
						this->_tab[k].insert(this->_tab[k].begin(), createEmpty(this->_tab[k][j].sq.NE.x, this->_tab[k][j].sq.NE.y - 1));
					}
					this->_sizeL += 1;
					y += 1;
				}

				if (i == this->_sizeH - 1) {
					std::vector<biomeGP>	tmp;
					for (int k = 0; k < this->_sizeL; k++) {
						tmp.push_back(createEmpty(this->_tab[i][k].sq.NE.x + 1, this->_tab[i][k].sq.NE.y));
					}
					this->_tab.push_back(tmp);
					this->_sizeH += 1;
				}

				if (i == 0) {
					std::vector<biomeGP>	tmp;
					for (int k = 0; k < this->_sizeL; k++) {
						tmp.push_back(createEmpty(this->_tab[i][k].sq.NE.x - 1, this->_tab[i][k].sq.NE.y));
					}
					this->_tab.insert(this->_tab.begin(), tmp);
					this->_sizeH += 1;
					i += 1;
				}
			}
		}
	}
}

chunk	***mapGP::chunkToRet(int x, int y) {
	int nbr = 1;
	chunk	***ret = new chunk**[nbr];
	int	a = (x + signeN(x)) / 15 + 33 - signeN(x);
	int	b = (y + signeN(y)) / 15 + 33 - signeN(y);

	for (int i = 0; i < nbr; i++) {
		ret[i] = new chunk*[nbr];
		for (int j = 0; j < nbr; j++) {
			ret[i][j] = new chunk[16];
			biome newB(this->_tab[a][b].bio, x, y);
			newB.doGPlvl1();
			for (int k = 0; k < 16; k++) {
				ret[i][j][k] = newB.voxelToChunk(i, j, k);
			}
		}
	}
	return ret;
}
