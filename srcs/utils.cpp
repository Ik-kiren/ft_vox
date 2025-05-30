#include "../includes/utils.hpp"

square	squarelvl1(int x, int y) {
	square	ret;

	ret.NE = gene2D(x, y);
	ret.NO = gene2D(x, y + 1);
	ret.SE = gene2D(x + 1, y);
	ret.SO = gene2D(x + 1, y + 1);
	return ret;	
}

coord2d	gene2D(int x, int y) {
	coord2d	ret;

	ret.x = x;
	ret.y = y;
	return ret;
}

coord2d	gene2DposTab(int x, int y, int nbr) {
	coord2d	ret;

	ret.x = (x + signeN(x)) % nbr;
	ret.y = (y + signeN(y)) % nbr;

	if (x < 0)
		ret.x = nbr - 1 + ret.x;
	if (y < 0)
		ret.y = nbr - 1 + ret.y;
	return ret;
}

coord3d	gene3D(int x, int y, int z) {
	coord3d	ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}

float	randFloatBetween(float h) {
	return (((double)std::rand()) / RAND_MAX) * 2 * h - h;
}

int		randIntBetween(int h) {
	h += 1;
	return (((double)std::rand()) / RAND_MAX) * 2 * h - h;
}

float	roundPreci(float x, int preci) {
	return (float)round((x * pow(10, preci))) / pow(10, preci);
}

void	newSeed(int x, int y) {
	std::srand(abs(seed + (1453 ^ x) * 12 - (8469 ^ y) * 31));
}

int		randSeed(int x, int y) {
	newSeed(x, y);
	return std::rand();
}

static void	freeVoxels(chunk toFree) {
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			delete [] toFree.voxel[i][j];
		}
		delete [] toFree.voxel[i];
	}
	delete [] toFree.voxel;
}

void	freeChunks(chunk *toFree) {
	for (int k = 0; k < 16; k++) {
		freeVoxels(toFree[k]);
	}
	delete [] toFree;
}

void	freeChunksAll(unsigned char ****ch) {
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < 16; k++)
				delete [] ch[i][j][k];
			delete [] ch[i][j];
		}
		delete [] ch[i];
	}
	delete [] ch;
}

int	signe(float x) {
	if (x < 0)
		return -1;
	return 1;
}

int	signeP(int x) {
	if (x < 0)
		return 0;
	return 1;
}

int	signeN(int x) {
	if (x >= 0)
		return 0;
	return 1;
}

int	signeN(float x) {
	if (x >= 0)
		return 0;
	return 1;
}
