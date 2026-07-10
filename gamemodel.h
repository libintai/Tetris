#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QVector>
#include "tetromino.h"

class GameModel : public QObject
{
    Q_OBJECT
public:
    explicit GameModel(QObject *parent = nullptr);

    // ---------- 游戏控制 ----------
    void initGame();                          // 重置游戏
    bool moveCurrentBlock(int dx, int dy);    // 移动当前方块（dx, dy 为偏移量）
    bool rotateCurrentBlock();                // 旋转当前方块
    bool hardDropCurrentBlock();              // 硬降（直接落底）
    void lockBlock();                         // 固定当前方块到棋盘
    bool isGameOver() const { return m_gameOver; }

    // ---------- 数据查询（供界面调用）----------
    int getBoardCell(int row, int col) const;              // 获取棋盘某个格子
    const Tetromino& getCurrentBlock() const { return m_currentBlock; }
    const Tetromino& getNextBlock() const { return m_nextBlock; }
    int getScore() const { return m_score; }
    int getLines() const { return m_lines; }
    int getLevel() const { return m_level; }
    int getSpeed() const { return m_speed; }

    // ---------- 特效相关 ----------
    QVector<int> getPendingClearRows() const { return m_pendingClearRows; }
    void setBoardCell(int row, int col, int color);       // 临时设置格子颜色（特效用）
    void saveBoardSnapshot();                             // 保存棋盘快照
    void restoreBoardSnapshot();                          // 恢复棋盘快照
    int performClearRows();                               // 真正执行行消除（特效结束后调用）

    // ---------- 数据持久化 ----------
    void saveHighScore(int userId);  // 保存最高分到数据库

signals:
    // 这些信号在数据变化时发射，界面收到后更新显示
    void boardChanged();        // 棋盘变化（需要重绘）
    void scoreChanged(int newScore);
    void linesChanged(int newLines);
    void levelChanged(int newLevel);
    void gameOverSignal(int finalScore);
    void rowsPendingClear(QVector<int> rows);  // 通知界面有行待消除（特效触发）

private:
    // 常量
    static const int ROWS = 20;   // 行数
    static const int COLS = 10;   // 列数

    // 数据成员
    int m_board[ROWS][COLS];      // 棋盘，0表示空，非0表示已固定方块的颜色编号
    int m_boardSnapshot[ROWS][COLS]; // 棋盘快照（用于特效恢复）
    Tetromino m_currentBlock;     // 当前活动方块
    Tetromino m_nextBlock;        // 下一个方块预览
    int m_score;                  // 当前得分
    int m_lines;                  // 已消除行数
    int m_level;                  // 当前等级（1开始）
    int m_speed;                  // 下落速度（毫秒）
    bool m_gameOver;              // 游戏是否结束

    QVector<int> m_pendingClearRows;  // 待消除的行号列表

    // 内部辅助函数
    bool checkCollision(const Tetromino &block) const;  // 碰撞检测
    void generateNewBlock();       // 生成下一个方块（把 m_nextBlock 变成当前，再生成新的 m_nextBlock）
    int clearFullRows();           // 消除满行，返回消除行数（内部使用，已废弃，改用 performClearRows）
    void updateSpeed();            // 根据等级更新下落速度
};

#endif // GAMEMODEL_H
