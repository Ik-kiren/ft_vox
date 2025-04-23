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
		void	doDig(int x, int y, int h);
	public:
		cave();
		cave(int x, int y, int size, int lenght, int H, int sizeBiome);
		~cave();
		
		caveDig			**_tab;

		void	doIniCave();
		int		getLenght();
		caveGP	getCaveX(int i);
};



#endif