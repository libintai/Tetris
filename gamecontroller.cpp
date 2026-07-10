#include "gamecontroller.h"
#include <QDebug>

// ---------- 构造函数 ----------
GameController::GameController(GameModel *model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_state(Idle)
{
    // 创建定时器
    m_timer = new QTimer(this);
    m_timer->setSingleShot(false);  // 循环触发

    // 连接信号槽
    connect(m_timer, &QTimer::timeout, this, &GameController::onTimerTimeout);
    connect(m_model, &GameModel::gameOverSignal, this, &GameController::onGameOver);

    qDebug() << "✅ GameController 创建完成，初始状态: Idle";
}

// ---------- 开始游戏 ----------
void GameController::startGame()
{
    qDebug() << "🎮 开始游戏";

    // 重置游戏数据
    m_model->initGame();

    // 更新状态
    m_state = Playing;

    // 启动定时器
    updateTimerSpeed();
    m_timer->start();

    // 通知界面
    emit gameStateChanged(m_state);
}

// ---------- 暂停游戏 ----------
void GameController::pauseGame()
{
    if (m_state == Playing) {
        m_state = Paused;
        m_timer->stop();
        emit gameStateChanged(m_state);
        qDebug() << "⏸️ 游戏暂停";
    }
}

// ---------- 继续游戏 ----------
void GameController::resumeGame()
{
    if (m_state == Paused) {
        m_state = Playing;
        updateTimerSpeed();
        m_timer->start();
        emit gameStateChanged(m_state);
        qDebug() << "▶️ 游戏继续";
    }
}

// ---------- 停止游戏 ----------
void GameController::stopGame()
{
    m_timer->stop();
    m_state = Idle;
    emit gameStateChanged(m_state);
    qDebug() << "🛑 游戏停止";
}

// ---------- 硬降 ----------
void GameController::hardDrop()
{
    if (m_state == Playing) {
        m_model->hardDropCurrentBlock();
        // 硬降后可能需要立即更新速度（因为可能消除了行，等级变化）
        updateTimerSpeed();
    }
}

// ---------- 定时器超时：触发下落 ----------
void GameController::onTimerTimeout()
{
    if (m_state != Playing) return;

    // 尝试下落一格
    if (!m_model->moveCurrentBlock(0, 1)) {
        // 无法下落 → 固定方块
        m_model->lockBlock();
        // 更新定时器速度（可能等级变了）
        updateTimerSpeed();
    }
}

// ---------- 响应游戏结束 ----------
void GameController::onGameOver(int finalScore)
{
    m_timer->stop();
    m_state = GameOver;
    emit gameStateChanged(m_state);
    emit gameOver(finalScore);
    qDebug() << "💀 游戏结束，最终得分:" << finalScore;
}

// ---------- 更新定时器速度 ----------
void GameController::updateTimerSpeed()
{
    if (m_model) {
        int speed = m_model->getSpeed();
        m_timer->setInterval(speed);
        qDebug() << "⏱️ 速度更新:" << speed << "ms";
    }
}
