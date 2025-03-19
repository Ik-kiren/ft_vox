#pragma once

enum BlockType {
    GRASS = 0
};

class Block
{
private:
    bool active;
public:
    constexpr static float BLOCK_SIZE = 0.5f;
    Block(/* args */);
    ~Block();
    bool IsActive();
    void SetActive(bool active);
};