#ifndef Player_H
# define Player_H

# include "Vector3.hpp"
# include "utils.hpp"

class Player {
	private:
		Vector3	_pos;
		chunk	*_ch;
		int		_timeFall;
	public:
		Player();
		Player(float x, float y, float z);
		~Player();

		Vector3	getPos();
		void 	setPos(Vector3 const &vec);
		void	setChunk(chunk *ch);
		void	setYfromChunk(chunk *ch);
		void	setYfromOwnChunk(Vector3 const &camera);
};

#endif
