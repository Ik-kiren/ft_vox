#ifndef Player_H
# define Player_H

# include "Vector3.hpp"
# include "utils.hpp"

class Player {
	private:
		unsigned char	****_ch;
		int		_timeFall;
	public:
		Vector3	_pos;
		Player();
		Player(float x, float y, float z);
		~Player();

		Vector3	getGlobalPos();
		Vector3 getChunkPos();
		void 	setPos(Vector3 const &vec);
		void	setChunk(unsigned char ****ch);
		void	setYfromChunk(unsigned char ****ch);
		void	setYfromOwnChunk(Vector3 const &camera);
};

#endif
