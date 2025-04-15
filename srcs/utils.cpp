#include "../includes/utils.hpp"

square	squarelvl1(square sq, int x, int y, int size) {
	square ret;

	// ret.NE = gene2D(sq.NE.x * size + x, sq.NE.y * size + y);
	// ret.NO = gene2D(sq.NO.x * size + x, sq.NO.y * size + y);
	// ret.SE = gene2D(sq.SE.x * size + x, sq.SE.y * size + y);
	// ret.SO = gene2D(sq.SO.x * size + x, sq.SO.y * size + y);
	ret.NE = gene2D(sq.NE.x + x, sq.NE.y + y);
	ret.NO = gene2D(sq.NO.x + x, sq.NO.y + y);
	ret.SE = gene2D(sq.SE.x + x, sq.SE.y + y);
	ret.SO = gene2D(sq.SO.x + x, sq.SO.y + y);
	(void)size;
	return ret;	
}

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
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 1; j++) {
			for (int k = 0; k < 16; k++) {
				freeVoxels(toFree[i][j][k]);
			}
			delete [] toFree[i][j];
		}
		delete [] toFree[i];
	}
	delete [] toFree;
}