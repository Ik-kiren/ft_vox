#include "../includes/utils.hpp"

square	squarelvl1(square sq, int x, int y, int size) {
	square	ret;
	coord2d	angle;

	if (x >= 0 && y >= 0)
		angle = sq.NE;
	else if (x >= 0)
		angle = sq.NO;
	else if (y >= 0)
		angle = sq.SE;
	else
		angle = sq.SO;

	// ret.NE = gene2D(angle.x * size + x, angle.y * size + y);
	// ret.NO = gene2D(angle.x * size + x, angle.y * size + y + signe(y));
	// ret.SE = gene2D(angle.x * size + x + signe(x), angle.y * size + y);
	// ret.SO = gene2D(angle.x * size + x + signe(x), angle.y * size + y + signe(y));

	// ret.NE = gene2D(angle.x * size + x, angle.y * size + y);
	// ret.NO = gene2D(angle.x * size + x, angle.y * size + y + 1);
	// ret.SE = gene2D(angle.x * size + x + 1, angle.y * size + y);
	// ret.SO = gene2D(angle.x * size + x + 1, angle.y * size + y + 1);

	ret.NE = gene2D(angle.x + x, angle.y + y);
	ret.NO = gene2D(angle.x + x, angle.y + y + 1);
	ret.SE = gene2D(angle.x + x + 1, angle.y + y);
	ret.SO = gene2D(angle.x + x + 1, angle.y + y + 1);
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
