#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include "gamemodel.h"

// 游戏状态枚举
enum GameState {
    Idle,       // 待开始（初始状态）
    Playing,    // 进行中
    Paused,     // 暂停
    GameOver    // 已结束
};

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(GameModel *model, QObject *parent = nullptr);

    // ---------- 游戏控制 ----------
    void startGame();       // 开始新游戏
    void pauseGame();       // 暂停
    void resumeGame();      // 继续
    void stopGame();        // 停止（结束当前对局）

    // ---------- 玩家操作 ----------
    void moveLeft()   { if (m_state == Playing) m_model->moveCurrentBlock(-1, 0); }
    void moveRight()  { if (m_state == Playing) m_model->moveCurrentBlock(1, 0); }
    void moveDown()   { if (m_state == Playing) m_model->moveCurrentBlock(0, 1); }
    void rotateBlock(){ if (m_state == Playing) m_model->rotateCurrentBlock(); }
    void hardDrop();  // 硬降（独立实现，因为需要处理后续逻辑）

    // ---------- 状态查询 ----------
    GameState getState() const { return m_state; }
    GameModel* getModel() const { return m_model; }

signals:
    void gameStateChanged(GameState state);  // 状态变化时通知界面
    void gameOver(int finalScore);           // 游戏结束信号

private slots:
    void onTimerTimeout();     // 定时器超时：触发下落
    void onGameOver(int finalScore);  // 响应 Model 的游戏结束信号

private:
    GameModel *m_model;        // 指向游戏模型
    QTimer *m_timer;           // 驱动下落的定时器
    GameState m_state;         // 当前状态

    void updateTimerSpeed();   // 更新定时器间隔
};

#endif // GAMECONTROLLER_H
