#ifndef utils_H
# define utils_H

# include <iostream>
# include <bits/stdc++.h>

extern int seed;

struct coord2d {
	int	x;
	int	y;
};

struct coord3d {
	int	x;
	int	y;
	int	z;
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

square	squarelvl1(int x, int y);
coord2d	gene2D(int x, int y);
coord2d	gene2DposTab(int x, int y, int nbr);
coord3d	gene3D(int x, int y, int k);
float	randFloatBetween(float h);
int		randIntBetween(int h);
float	roundPreci(float x, int preci);
void	newSeed(int x, int y);
int		randSeed(int x, int y);
void	freeChunks(chunk *toFree);
void	freeChunksAll(unsigned char ****ch);
int		signe(float x);
int		signeP(int x);
int		signeN(int x);
int		signeN(float x);

#endif