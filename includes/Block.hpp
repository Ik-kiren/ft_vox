#pragma once

enum BlockType {
    DEFAULT = 0,
    GRASS = 1,
    STONE = 2
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