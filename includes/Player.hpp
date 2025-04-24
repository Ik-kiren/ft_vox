#ifndef Player_H
# define Player_H

# include "Vector3.hpp"
# include "utils.hpp"

class Player {
	private:
		Vector3	_pos;
	public:
		Player();
		Player(float x, float y, float z);
		~Player();

		Vector3	getPos();
		void 	setPos(Vector3 vec);
		void	setYfromChunk(chunk *ch);
};

#endif
