#include "../includes/utils.hpp"

coord2d	gene2D(int x, int y) {
	coord2d	ret;

	ret.x = x;
	ret.y = y;
	return ret;
}

float	randFloatBetween(float h) {
	return (((double)std::rand()) / RAND_MAX) * 2 * h - h;
}

float	roundPreci(float x, int preci) {
	return (float)round((x * pow(10, preci))) / pow(10, preci);
}

void	newSeed(int x, int y) {
	std::srand(abs(seed + (1453 ^ x) * 12 - (8469 ^ y) * 13));
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

void	freeChunks(chunk ***toFree) {
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < 16; k++) {
				freeVoxels(toFree[i][j][k]);
			}
			delete [] toFree[i][j];
		}
		delete [] toFree[i];
	}
	delete [] toFree;
}