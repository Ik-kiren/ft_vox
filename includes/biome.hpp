#ifndef biome_H
# define biome_H
 
# include <iostream>
# include "utils.hpp"
# include "cave.hpp"

extern int seed;
int const	sizeH = 256;

struct heightGP {
	int			GP;
	float		heightF1;
	float		heightF2;
	float		heightF3;
	float		heightF;
	float		tempF;
	float		waterF;
	int			heightI;
	int			texture;
	std::array<unsigned char, sizeH>	arrayH;
};

struct playerDig {
	bool					impact;
	std::vector<coord3d>	toDel;
};

class biome {
	private:
		int			_size;
		int			_nbr;
		int			_sizeZ;
		int			_heightMin;
		int			_heightMax;
		int			_heightDiff;
		square		_sq;
		int			_nbrGP;
		float		_Hb;
		int			_level;
		std::vector<std::vector<heightGP>>	_tab;
		cave		*_cave;
		playerDig	**_deleted;

		void	iniBiome();
		void	doGPCalculLine(int i, int j, int mid, int L, float H1, float H2, float H3);
		void	doGPCalculColumn(int i, int j, int mid, int l, float H1, float H2, float H3);
		void	doGPLine(int i, coord2d cd);
		void	doGPColumn(int j, coord2d cd);
		int		whatTexture(int x, int y);
		std::array<unsigned char, sizeH>	fillArray(int h, int texture);
		int		heightFtoI(float heightF, float denivele);
		void	afterGP();
		void	iniBiome1(biome &biSup, int x, int y);
	public:
		biome();
		biome(int size, square sq);
		biome(biome &biSup, int x, int y);
		~biome();

		void	printTab(int mode);
		void	printCave(int x, int y);
		void	doGP();
		std::array<unsigned char, sizeH>	&getArray(int x, int y);
		chunk	voxelToChunk(int k);
		void	doGPlvl1();
		square	getSquare();	
		cave	*getCave();
		void	setCaves(int xrand, int yrand, biome &biSup);
		void	setDeleted();
		float	getHeightF(int x, int y);
		void	dig(biome &biSup, int x, int y);
		void	deleteCube(int a, int b, int c, int d, int z);
};

#endif