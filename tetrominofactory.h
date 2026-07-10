#ifndef TETROMINOFACTORY_H
#define TETROMINOFACTORY_H

#include "tetromino.h"
#include <QRandomGenerator>

class TetrominoFactory
{
public:
    // 随机生成一种方块（七种等概率）
    static Tetromino createRandomBlock() {
        int type = QRandomGenerator::global()->bounded(7);
        return Tetromino(static_cast<BlockType>(type));
    }

    // 根据指定类型生成方块
    static Tetromino createBlock(BlockType type) {
        return Tetromino(type);
    }
};

#endif // TETROMINOFACTORY_H
