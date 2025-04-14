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

class cave {
	private:
		int	_seed;
		int	_sizeIni;
		int	_lenght;
		int	_nbrGP;
		int	_sizeModif;
		std::vector<caveGP>	_caveX;	

		void	afterGPcave();
	public:
		cave();
		cave(int x, int y, int size, int lenght);
		~cave();

		void	doIniCave();
		int		getLenght();
		caveGP	getCaveX(int i);
};



#endif