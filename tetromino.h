#ifndef TETROMINO_H
#define TETROMINO_H

#include <QVector>

// 方块类型枚举（七种标准方块）
enum BlockType {
    I,   // 长条形（青色）
    O,   // 方形（黄色）
    T,   // T形（紫色）
    S,   // S形（绿色）
    Z,   // Z形（红色）
    J,   // J形（蓝色）
    L,   // L形（橙色）
    COUNT  // 用于计数，值为7
};

class Tetromino
{
public:
    // 构造函数
    Tetromino();
    explicit Tetromino(BlockType type);

    // ---------- 核心操作 ----------
    // 顺时针旋转 90 度（4x4 矩阵转置 + 每行反转）
    void rotateClockwise();

    // ---------- 移动（只改变坐标，合法性由外部 GameModel 检查）----------
    void moveLeft()  { m_x--; }
    void moveRight() { m_x++; }
    void moveDown()  { m_y++; }
    void moveUp()    { m_y--; }

    // ---------- Getter 方法 ----------
    BlockType getType() const { return m_type; }
    const QVector<QVector<int>>& getShape() const { return m_shape; }
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    int getColor() const { return m_color; }

    // ---------- Setter 方法 ----------
    void setPosition(int x, int y) { m_x = x; m_y = y; }
    void setShape(const QVector<QVector<int>>& shape) { m_shape = shape; }

private:
    BlockType m_type;                    // 方块类型
    QVector<QVector<int>> m_shape;       // 4x4 形状矩阵（0=空，1=有方块）
    int m_x;                             // 在棋盘中的列索引 (0~9)
    int m_y;                             // 在棋盘中的行索引 (0~19)
    int m_color;                         // 颜色编号 (1~7)
};

#endif // TETROMINO_H
