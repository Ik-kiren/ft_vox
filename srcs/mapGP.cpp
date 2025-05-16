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
			biomeGP	*tmpS = new biomeGP();
			tmpS->sq.NE = gene2D(i, j);
			tmpS->sq.NO = gene2D(i, j + 1);
			tmpS->sq.SE = gene2D(i + 1, j);
			tmpS->sq.SO = gene2D(i + 1, j + 1);
			tmpS->bio = new biome(this->_sizeBiome, tmpS->sq);
			tmpS->GP = 0;
			tmp.push_back(*tmpS);
			delete tmpS;
		}
		this->_tab.push_back(tmp);
	}
	this->_voidBiome = biome(this->_sizeBiome, squarelvl1(0, 0));
	doGPIniBiome();
	this->_myBiome = new biome(*this->_tab[0][0].bio, 0, 0);
}

mapGP::~mapGP() {
	for (int i = 0; i < this->_tab.size(); i++) {
		for (int j = 0; j < this->_tab[i].size(); j++)
			delete this->_tab[i][j].bio;
	}
	delete this->_myBiome;
}

void	mapGP::printGP() {
	for (int i = 1; i < this->_sizeH - 1; i++) {
		for (int j = 1; j < this->_sizeL - 1; j++) {
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
	this->_tab[x][y].bio->printTab(mode);
}

void	mapGP::printCave(int x, int y, int a, int b) {
	this->_tab[x][y].bio->printCave(a, b);
}

void	mapGP::doGPIniBiome() {
	for (int i = 0; i < this->_sizeH; i++) {
		for (int j = 0; j < this->_sizeL; j++) {
			if (this->_tab[i][j].GP == 0) {
				this->_tab[i][j].bio->doGP();
				this->_tab[i][j].GP = 1;
			}
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

void	mapGP::chunkToRet(int x, int y, unsigned char ****ch) {
	int	a = (x + signeN(x)) / (this->_sizeBiome - 1) + (this->_sizeIni / 2 + 1) - signeN(x);
	int	b = (y + signeN(y)) / (this->_sizeBiome - 1) + (this->_sizeIni / 2 + 1) - signeN(y);

	if (a >= this->_sizeH || b >= this->_sizeL || a < 0 || b < 0) {
		this->_myBiome->iniMyBiome(this->_voidBiome, x, y);
		this->_myBiome->doGPlvl1();
	} else {
		this->_myBiome->iniMyBiome(*this->_tab[a][b].bio, x, y);
		this->_myBiome->doGPlvl1();
		if (!this->_tab[a][b].bio->getCave()) {
			this->_tab[a][b].bio->setCaves(this->_tab[a][b].sq.NE.x, this->_tab[a][b].sq.NE.y, *this->_tab[a][b].bio);
			// this->_tab[a][b].bio->setDeleted();
		}
		this->_myBiome->dig(*this->_tab[a][b].bio);
	}

	this->_myBiome->voxelToChunk(ch);
}

void	mapGP::deleteCube(int x, int y, int z) {
	int xBiome = (x / 16 + signeN(x)) / (this->_sizeBiome - 1) + (this->_sizeIni / 2 + 1) - signeN(x);
	int yBiome = (y / 16 + signeN(y)) / (this->_sizeBiome - 1) + (this->_sizeIni / 2 + 1) - signeN(y);

	if (xBiome >= this->_sizeH || yBiome >= this->_sizeL || xBiome < 0 || yBiome < 0)
		return ;

	int	xChunk = (x / 16) % (this->_sizeBiome - 1);
	int	yChunk = (y / 16) % (this->_sizeBiome - 1);

	int xCube = (x % ((this->_sizeBiome - 1) * (this->_sizeBiome - 1))) % 16;
	int yCube = (y % ((this->_sizeBiome - 1) * (this->_sizeBiome - 1))) % 16;

	if (x < 0) {
		xChunk = (this->_sizeBiome - 2) + xChunk;
		xCube = 15 + xCube;
	}
	if (y < 0) {		
		yChunk = (this->_sizeBiome - 2) + yChunk;
		yCube = 15 + yCube;
	}
	this->_tab[xBiome][yBiome].bio->deleteCube(xChunk, yChunk, xCube, yCube, z);
}
