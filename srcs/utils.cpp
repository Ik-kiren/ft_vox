#include "../includes/utils.hpp"

square	squarelvl1(square sq, int x, int y, int size) {
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
