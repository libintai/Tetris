#include "gamemodel.h"
#include "tetrominofactory.h"
#include "databasehelper.h"
#include <QDebug>
#include <QtGlobal>
#include <algorithm>

// ---------- 构造函数 ----------
GameModel::GameModel(QObject *parent)
    : QObject(parent)
{
    initGame();  // 初始化游戏
}

// ---------- 初始化游戏 ----------
void GameModel::initGame()
{
    // 1. 清空棋盘
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            m_board[r][c] = 0;
            m_boardSnapshot[r][c] = 0;
        }
    }

    // 2. 重置数据
    m_score = 0;
    m_lines = 0;
    m_level = 1;
    m_speed = 500;      // 初始下落间隔 500ms
    m_gameOver = false;
    m_pendingClearRows.clear();

    // 3. 生成初始方块
    m_nextBlock = TetrominoFactory::createRandomBlock();
    generateNewBlock();

    // 4. 发射信号通知界面更新
    emit boardChanged();
    emit scoreChanged(m_score);
    emit linesChanged(m_lines);
    emit levelChanged(m_level);

    qDebug() << "✅ 游戏初始化完成";
}

// ---------- 碰撞检测 ----------
bool GameModel::checkCollision(const Tetromino &block) const
{
    const auto &shape = block.getShape();
    int x = block.getX();
    int y = block.getY();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] != 0) {  // 该格子有方块
                int boardRow = y + i;
                int boardCol = x + j;

                // 检查是否超出左右边界
                if (boardCol < 0 || boardCol >= COLS) {
                    return true;
                }

                // 检查是否超出底部边界
                if (boardRow >= ROWS) {
                    return true;
                }

                // 检查是否与已固定方块重叠（只在棋盘范围内检查）
                if (boardRow >= 0 && m_board[boardRow][boardCol] != 0) {
                    return true;
                }
            }
        }
    }
    return false;  // 无碰撞
}

// ---------- 移动当前方块 ----------
bool GameModel::moveCurrentBlock(int dx, int dy)
{
    if (m_gameOver) return false;

    Tetromino temp = m_currentBlock;
    temp.setPosition(temp.getX() + dx, temp.getY() + dy);

    if (!checkCollision(temp)) {
        m_currentBlock.setPosition(temp.getX(), temp.getY());
        emit boardChanged();  // 通知界面重绘
        return true;
    }
    return false;
}

// ---------- 旋转当前方块（含踢墙处理）----------
bool GameModel::rotateCurrentBlock()
{
    if (m_gameOver) return false;

    Tetromino temp = m_currentBlock;
    temp.rotateClockwise();

    // 踢墙检测：尝试偏移位置
    // 偏移量: {不偏移, 左1, 右1, 左2, 右2}
    int kicks[5][2] = {
        {0, 0},
        {-1, 0},
        {1, 0},
        {-2, 0},
        {2, 0}
    };

    for (int i = 0; i < 5; i++) {
        temp.setPosition(m_currentBlock.getX() + kicks[i][0],
                         m_currentBlock.getY() + kicks[i][1]);
        if (!checkCollision(temp)) {
            // 找到合法位置，应用旋转
            m_currentBlock = temp;
            emit boardChanged();
            return true;
        }
    }

    // 所有尝试都失败，放弃旋转
    return false;
}

// ---------- 硬降（直接落底）----------
bool GameModel::hardDropCurrentBlock()
{
    if (m_gameOver) return false;

    // 一直向下移动，直到碰撞
    while (!checkCollision(m_currentBlock)) {
        m_currentBlock.moveDown();
    }
    // 回退一格（因为最后一步移动后触发了碰撞，实际上方块在碰撞位置的上方）
    m_currentBlock.moveUp();

    // 固定方块
    lockBlock();
    return true;
}

// ---------- 固定当前方块到棋盘（核心修改）----------
void GameModel::lockBlock()
{
    const auto &shape = m_currentBlock.getShape();
    int x = m_currentBlock.getX();
    int y = m_currentBlock.getY();

    // 将当前方块的颜色写入棋盘
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] != 0) {
                int boardRow = y + i;
                int boardCol = x + j;
                if (boardRow >= 0 && boardRow < ROWS &&
                    boardCol >= 0 && boardCol < COLS) {
                    m_board[boardRow][boardCol] = m_currentBlock.getColor();
                }
            }
        }
    }

    // ========== 特效核心逻辑 ==========
    // 检测满行，但不再立即消除，而是记录到 m_pendingClearRows
    m_pendingClearRows.clear();
    for (int row = ROWS - 1; row >= 0; row--) {
        bool full = true;
        for (int col = 0; col < COLS; col++) {
            if (m_board[row][col] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            m_pendingClearRows.append(row);
        }
    }

    if (!m_pendingClearRows.isEmpty()) {
        // 有满行 → 保存快照，发射信号通知界面播放特效
        saveBoardSnapshot();
        emit rowsPendingClear(m_pendingClearRows);
        qDebug() << "🎨 检测到" << m_pendingClearRows.size() << "行待消除，等待特效播放";
    } else {
        // 没有满行 → 正常生成下一个方块
        generateNewBlock();
        if (checkCollision(m_currentBlock)) {
            m_gameOver = true;
            emit gameOverSignal(m_score);
        }
        emit boardChanged();
    }
}

// ---------- 特效：真正执行行消除 ----------
int GameModel::performClearRows()
{
    if (m_pendingClearRows.isEmpty()) return 0;

    int cleared = m_pendingClearRows.size();

    // 从下往上排序（确保从底部开始消除）
    std::sort(m_pendingClearRows.begin(), m_pendingClearRows.end(), std::greater<int>());

    for (int row : m_pendingClearRows) {
        // 将该行上面的所有行下移
        for (int r = row; r > 0; r--) {
            for (int c = 0; c < COLS; c++) {
                m_board[r][c] = m_board[r - 1][c];
            }
        }
        // 顶部行置空
        for (int c = 0; c < COLS; c++) {
            m_board[0][c] = 0;
        }
    }

    // 清空待消除列表
    m_pendingClearRows.clear();

    // 计分
    int scoreTable[] = {0, 100, 300, 500, 800};
    int addScore = (cleared <= 4) ? scoreTable[cleared] : 800;
    m_score += addScore;
    m_lines += cleared;
    m_level = m_lines / 10 + 1;
    updateSpeed();

    // 发射信号更新界面
    emit scoreChanged(m_score);
    emit linesChanged(m_lines);
    emit levelChanged(m_level);

    qDebug() << "🎯 消除" << cleared << "行！得分 +" << addScore
             << "，总得分：" << m_score << "，等级：" << m_level;

    // 生成下一个方块
    generateNewBlock();
    if (checkCollision(m_currentBlock)) {
        m_gameOver = true;
        emit gameOverSignal(m_score);
    }
    emit boardChanged();

    return cleared;
}

// ---------- 生成新方块 ----------
void GameModel::generateNewBlock()
{
    m_currentBlock = m_nextBlock;
    m_currentBlock.setPosition(3, 0);  // 从顶部中间出现
    m_nextBlock = TetrominoFactory::createRandomBlock();
}

// ---------- 更新下落速度 ----------
void GameModel::updateSpeed()
{
    // 等级越高，下落越快
    // 等级1: 500ms, 等级2: 440ms, ... 等级10: 140ms
    int baseSpeed = 500 - (m_level - 1) * 40;
    m_speed = qMax(80, baseSpeed);  // 最快不低于 80ms
}

// ---------- 获取棋盘格子 ----------
int GameModel::getBoardCell(int row, int col) const
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return 0;
    }
    return m_board[row][col];
}

// ---------- 特效：设置格子颜色 ----------
void GameModel::setBoardCell(int row, int col, int color)
{
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        m_board[row][col] = color;
    }
}

// ---------- 特效：保存棋盘快照 ----------
void GameModel::saveBoardSnapshot()
{
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            m_boardSnapshot[r][c] = m_board[r][c];
        }
    }
}

// ---------- 特效：恢复棋盘快照 ----------
void GameModel::restoreBoardSnapshot()
{
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            m_board[r][c] = m_boardSnapshot[r][c];
        }
    }
}

// ---------- 旧方法：保留但不再使用（避免编译警告） ----------
int GameModel::clearFullRows()
{
    // 这个方法已被 performClearRows 替代，但保留以防其他地方调用
    // 实际上 lockBlock() 中已经不再调用此方法
    return 0;
}

// ---------- 保存最高分 ----------
void GameModel::saveHighScore(int userId)
{
    if (userId > 0) {
        DatabaseHelper db;
        if (db.initDatabase()) {
            db.saveScore(userId, m_score, m_lines);
            qDebug() << "💾 最高分已保存，user_id:" << userId
                     << ", score:" << m_score;
        }
    }
}
