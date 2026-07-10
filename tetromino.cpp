#include "tetromino.h"
#include <QDebug>

// ---------- 静态数据：七种方块的形状定义 ----------
// 每个方块用 4x4 矩阵表示，1 表示有方块，0 表示空
static const QVector<QVector<int>> SHAPES[7] = {
    // I（长条形）
    {{0, 0, 0, 0},
     {1, 1, 1, 1},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},

    // O（方形）
    {{1, 1, 0, 0},
     {1, 1, 0, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},

    // T（T形）
    {{0, 1, 0, 0},
     {1, 1, 1, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},

    // S（S形）
    {{0, 1, 1, 0},
     {1, 1, 0, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},

    // Z（Z形）
    {{1, 1, 0, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},

    // J（J形）
    {{1, 0, 0, 0},
     {1, 1, 1, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},

    // L（L形）
    {{0, 0, 1, 0},
     {1, 1, 1, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}}
};

// 七种方块对应的颜色编号
// （1~7 对应不同的颜色，后续在绘制时会映射为具体 QColor）
static const int COLORS[7] = {1, 2, 3, 4, 5, 6, 7};

// ---------- 构造函数 ----------
Tetromino::Tetromino()
    : m_type(I), m_x(3), m_y(0), m_color(1)
{
    m_shape = SHAPES[0];  // 默认 I 形
}

Tetromino::Tetromino(BlockType type)
    : m_type(type), m_x(3), m_y(0)
{
    m_shape = SHAPES[type];
    m_color = COLORS[type];
}

// ---------- 顺时针旋转（核心算法）----------
void Tetromino::rotateClockwise()
{
    // 4x4 矩阵顺时针旋转 90 度
    // 公式：newShape[j][3 - i] = oldShape[i][j]
    QVector<QVector<int>> newShape(4, QVector<int>(4, 0));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            newShape[j][3 - i] = m_shape[i][j];
        }
    }

    m_shape = newShape;
}
