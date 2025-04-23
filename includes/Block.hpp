#pragma once

enum BlockType {
    DEFAULT = 0,
    DIRT = 1,
    STONE = 2,
	SAND = 3,
	RSAND = 4,
	SNOW = 5,
	ICE = 6,
	GRAVEL = 7,
	GRASS = 8
};

enum FacesType {
    YPOSITIF,
    YNEGATIF,
    XPOSITIF,
    XNEGATIF,
    ZPOSITIF,
    ZNEGATIF
};

class Block
{
private:
    bool active;
public:
    bool visited[6] = {false, false, false, false, false, false};
    BlockType type = DEFAULT;
    constexpr static float BLOCK_SIZE = 1.0f;
    Block(/* args */);
    ~Block();
    
    bool IsActive();
    void SetActive(bool active);
};