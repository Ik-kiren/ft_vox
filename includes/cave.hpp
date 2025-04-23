#ifndef cave_H
# define cave_H

# include <iostream>
# include "utils.hpp"

struct caveGP {
	int		GP;
	float	sizeF;
	int		sizeI;
	float	dirXF;
	int		dirXI;
	float	dirYF;
	int		dirYI;
	float	dirZF;
	int		dirZI;
};

struct caveDig {
	bool					impact;
	std::vector<coord3d>	toDel;
};

class cave {
	private:
		int	_seed;
		int	_sizeIni;
		int	_lenght;
		int	_nbrGP;
		int	_sizeModif;
		int	_sizeBiome;
		std::vector<caveGP>	_caveX;

		void	afterGPcave();
		void	dig(int x, int y, int h, int size);
		void	doIniCave();
		void	doDig(int x, int y, int h);
	public:
		cave();
		cave(int sizeBiome, int lenghtMax);
		~cave();
		
		caveDig			**_tab;

		int		getLenght();
		caveGP	getCaveX(int i);
		void	doNewCave(int xrand, int yrand, int x, int y, int sizeHoleIni, int lenght, int H);
};



#endif