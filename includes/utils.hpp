#ifndef utils_H
# define utils_H

# include <iostream>
# include <bits/stdc++.h>

extern int seed;

struct coord2d {
	int	x;
	int	y;
};

struct square {
	coord2d	NE;
	coord2d	NO;
	coord2d	SE;
	coord2d	SO;
};

struct chunk {
	unsigned char	***voxel;
};

square	squarelvl1(square sq, int x, int y, int size);
coord2d	gene2D(int x, int y);
float	randFloatBetween(float h);
float	roundPreci(float x, int preci);
void	newSeed(int x, int y);
int		randSeed(int x, int y);
void	freeChunks(chunk ***toFree);
int		signe(float x);
int		signeP(int x);
int		signeN(int x);

#endif