#ifndef mapGP_H
# define mapGP_H
 
# include <iostream>
# include <limits> 
# include <bits/stdc++.h>
# include "biome.hpp"
# include "utils.hpp"

extern int seed;

struct biomeGP {
	bool	GP;
	square	sq;
	biome	*bio;
};

class mapGP {
	private:
		int			_sizeIni;
		int			_sizeH;
		int			_sizeL;
		int			_sizeZ;
		int			_sizeBiome;
		std::vector<std::vector<biomeGP>>		_tab;
		biome		_voidBiome;
		biome		*_myBiome;

		void	doGPIniBiome();
		coord2d	findCoord(int x, int y);
		biomeGP	createEmpty(int x, int y);
		void	deleteCave(int a, int b);
	public:
		mapGP();
		mapGP(int size, int sizeBiome);
		~mapGP();

		void	printGP();
		void	printSquareGP(int x, int y, int mode);
		void	printCave(int x, int y, int a, int b);
		void	chunkToRet(int x, int y, unsigned char ****ch);
		void	deleteCube(int x, int y, int z);
};

#endif