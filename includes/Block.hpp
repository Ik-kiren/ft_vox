#pragma once

enum BlockType {
    GRASS = 0,
    STONE = 1
};

class Block
{
private:
    bool active;
public:
    BlockType type = GRASS;
    constexpr static float BLOCK_SIZE = 0.5f;
    Block(/* args */);
    ~Block();
    bool IsActive();
    void SetActive(bool active);
};