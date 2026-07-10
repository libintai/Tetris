// #include "mainwindow.h"
// #include "ui_mainwindow.h"
// #include "rankdialog.h"
// #include <QMenuBar>
// #include <QToolBar>
// #include <QStatusBar>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QGroupBox>
// #include <QMessageBox>
// #include <QKeyEvent>
// #include <QDebug>

// MainWindow::MainWindow(int userId, QWidget *parent)
//     : QMainWindow(parent)
//     , ui(new Ui::MainWindow)
//     , m_userId(userId)
// {
//     // 创建模型和控制器
//     m_model = new GameModel(this);
//     m_controller = new GameController(m_model, this);

//     // 创建 UI
//     initUI();
//     createMenuBar();
//     connectSignals();

//     // 初始状态
//     updateUI(Idle);

//     setWindowTitle("俄罗斯方块 Tetris");
//     setFixedSize(650, 700);

//     qDebug() << "✅ MainWindow 创建完成，用户ID:" << m_userId;
// }

// MainWindow::~MainWindow()
// {
//     delete ui;
// }

// // ---------- 初始化界面 ----------
// void MainWindow::initUI()
// {
//     // 创建中央部件
//     QWidget *central = new QWidget(this);
//     setCentralWidget(central);

//     // 主水平布局
//     QHBoxLayout *mainLayout = new QHBoxLayout(central);

//     // ===== 左侧：游戏画布 =====
//     m_gameWidget = new GameWidget(m_model, this);
//     mainLayout->addWidget(m_gameWidget);

//     // ===== 右侧：信息面板 =====
//     QWidget *rightPanel = new QWidget(this);
//     QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
//     rightLayout->setSpacing(10);

//     // 标题
//     QLabel *titleLabel = new QLabel("🎮 俄罗斯方块", this);
//     titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #FFD700;");
//     rightLayout->addWidget(titleLabel);
//     rightLayout->addSpacing(10);

//     // 分数显示
//     m_scoreLabel = new QLabel("得分: 0", this);
//     m_scoreLabel->setStyleSheet("font-size: 14px; color: white;");
//     rightLayout->addWidget(m_scoreLabel);

//     m_linesLabel = new QLabel("行数: 0", this);
//     m_linesLabel->setStyleSheet("font-size: 14px; color: white;");
//     rightLayout->addWidget(m_linesLabel);

//     m_levelLabel = new QLabel("等级: 1", this);
//     m_levelLabel->setStyleSheet("font-size: 14px; color: white;");
//     rightLayout->addWidget(m_levelLabel);

//     m_statusLabel = new QLabel("状态: 待开始", this);
//     m_statusLabel->setStyleSheet("font-size: 14px; color: #FFD700;");
//     rightLayout->addWidget(m_statusLabel);

//     rightLayout->addSpacing(10);

//     // 下一个方块预览
//     QLabel *previewTitle = new QLabel("⬇️ 下一个", this);
//     previewTitle->setStyleSheet("font-size: 14px; color: white;");
//     rightLayout->addWidget(previewTitle);

//     m_previewWidget = new PreviewWidget(m_model, this);
//     rightLayout->addWidget(m_previewWidget);

//     rightLayout->addSpacing(15);

//     // 速度控制
//     QLabel *speedLabel = new QLabel("速度", this);
//     speedLabel->setStyleSheet("color: white;");
//     rightLayout->addWidget(speedLabel);

//     m_speedSlider = new QSlider(Qt::Horizontal, this);
//     m_speedSlider->setRange(1, 10);
//     m_speedSlider->setValue(3);
//     m_speedSlider->setTickPosition(QSlider::TicksBelow);
//     m_speedSlider->setTickInterval(1);
//     rightLayout->addWidget(m_speedSlider);

//     rightLayout->addSpacing(10);

//     // 音效控制
//     m_soundCheck = new QCheckBox("🔊 音效", this);
//     m_soundCheck->setStyleSheet("color: white;");
//     m_soundCheck->setChecked(true);
//     rightLayout->addWidget(m_soundCheck);

//     rightLayout->addSpacing(15);

//     // 控制按钮
//     m_startBtn = new QPushButton("▶ 开始", this);
//     m_startBtn->setFixedHeight(35);
//     m_startBtn->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
//     rightLayout->addWidget(m_startBtn);

//     m_pauseBtn = new QPushButton("⏸ 暂停", this);
//     m_pauseBtn->setFixedHeight(35);
//     m_pauseBtn->setStyleSheet("background-color: #FF9800; color: white; font-weight: bold;");
//     m_pauseBtn->setEnabled(false);
//     rightLayout->addWidget(m_pauseBtn);

//     m_resetBtn = new QPushButton("🔄 重置", this);
//     m_resetBtn->setFixedHeight(35);
//     m_resetBtn->setStyleSheet("background-color: #f44336; color: white; font-weight: bold;");
//     rightLayout->addWidget(m_resetBtn);

//     rightLayout->addStretch();

//     // 把右侧面板加入主布局
//     mainLayout->addWidget(rightPanel);

//     // 设置中央部件
//     setCentralWidget(central);

//     // 状态栏
//     statusBar()->showMessage("按方向键控制方块，按 P 暂停/继续");
// }

// // ---------- 创建菜单栏 ----------
// void MainWindow::createMenuBar()
// {
//     // 游戏菜单
//     QMenu *gameMenu = menuBar()->addMenu("游戏(&G)");

//     QAction *startAction = new QAction("开始", this);
//     startAction->setShortcut(QKeySequence("Ctrl+N"));
//     connect(startAction, &QAction::triggered, this, &MainWindow::onStartClicked);
//     gameMenu->addAction(startAction);

//     QAction *pauseAction = new QAction("暂停/继续", this);
//     pauseAction->setShortcut(QKeySequence("Ctrl+P"));
//     connect(pauseAction, &QAction::triggered, this, &MainWindow::onPauseClicked);
//     gameMenu->addAction(pauseAction);

//     gameMenu->addSeparator();

//     QAction *exitAction = new QAction("退出", this);
//     exitAction->setShortcut(QKeySequence("Ctrl+Q"));
//     connect(exitAction, &QAction::triggered, this, &QWidget::close);
//     gameMenu->addAction(exitAction);

//     // 视图菜单
//     QMenu *viewMenu = menuBar()->addMenu("视图(&V)");
//     QAction *rankAction = new QAction("排行榜", this);
//     connect(rankAction, &QAction::triggered, this, &MainWindow::onShowRank);
//     viewMenu->addAction(rankAction);

//     // 帮助菜单
//     QMenu *helpMenu = menuBar()->addMenu("帮助(&H)");
//     QAction *aboutAction = new QAction("关于", this);
//     connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
//     helpMenu->addAction(aboutAction);
// }

// // ---------- 连接信号槽 ----------
// void MainWindow::connectSignals()
// {
//     // 按钮信号
//     connect(m_startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);
//     connect(m_pauseBtn, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
//     connect(m_resetBtn, &QPushButton::clicked, this, &MainWindow::onResetClicked);

//     // 速度滑块
//     connect(m_speedSlider, &QSlider::valueChanged, [this](int value) {
//         // 速度值存储到 QSettings 中（可选），这里我们实时调整 Model 的速度
//         // 但实际速度由等级决定，这个滑块可以控制"难度系数"
//         // 简单实现：让速度 = 基础速度 / (1 + (value-1) * 0.2)
//         // 这里先不做复杂处理，留作扩展
//         qDebug() << "速度滑块:" << value;
//     });

//     // Controller 信号
//     connect(m_controller, &GameController::gameStateChanged,
//             this, &MainWindow::updateUI);
//     connect(m_controller, &GameController::gameOver,
//             this, &MainWindow::onGameOver);

//     // Model 信号
//     connect(m_model, &GameModel::scoreChanged,
//             this, &MainWindow::updateScore);
//     connect(m_model, &GameModel::linesChanged,
//             this, &MainWindow::updateLines);
//     connect(m_model, &GameModel::levelChanged,
//             this, &MainWindow::updateLevel);
// }

// // ---------- 按钮槽函数 ----------
// void MainWindow::onStartClicked()
// {
//     qDebug() << "🎮 点击「开始」";
//     m_controller->startGame();
// }

// void MainWindow::onPauseClicked()
// {
//     if (m_controller->getState() == Playing) {
//         m_controller->pauseGame();
//     } else if (m_controller->getState() == Paused) {
//         m_controller->resumeGame();
//     }
// }

// void MainWindow::onResetClicked()
// {
//     qDebug() << "🔄 点击「重置」";
//     m_controller->stopGame();
//     m_controller->startGame();
// }

// // ---------- 更新 UI ----------
// void MainWindow::updateUI(GameState state)
// {
//     QString statusText;
//     switch (state) {
//     case Idle:
//         statusText = "待开始";
//         m_startBtn->setEnabled(true);
//         m_pauseBtn->setEnabled(false);
//         m_pauseBtn->setText("⏸ 暂停");
//         break;
//     case Playing:
//         statusText = "🟢 游戏中";
//         m_startBtn->setEnabled(false);
//         m_pauseBtn->setEnabled(true);
//         m_pauseBtn->setText("⏸ 暂停");
//         break;
//     case Paused:
//         statusText = "⏸ 已暂停";
//         m_startBtn->setEnabled(false);
//         m_pauseBtn->setEnabled(true);
//         m_pauseBtn->setText("▶ 继续");
//         break;
//     case GameOver:
//         statusText = "💀 游戏结束";
//         m_startBtn->setEnabled(true);
//         m_pauseBtn->setEnabled(false);
//         m_pauseBtn->setText("⏸ 暂停");
//         break;
//     }
//     m_statusLabel->setText(statusText);
// }

// void MainWindow::updateScore(int score)
// {
//     m_scoreLabel->setText(QString("得分: %1").arg(score));
// }

// void MainWindow::updateLines(int lines)
// {
//     m_linesLabel->setText(QString("行数: %1").arg(lines));
// }

// void MainWindow::updateLevel(int level)
// {
//     m_levelLabel->setText(QString("等级: %1").arg(level));
// }

// void MainWindow::onGameOver(int finalScore)
// {
//     QMessageBox::information(this, "游戏结束",
//                              QString("🎮 游戏结束！\n\n最终得分: %1\n消除行数: %2\n等级: %3")
//                                  .arg(finalScore)
//                                  .arg(m_model->getLines())
//                                  .arg(m_model->getLevel()));

//     // 保存最高分
//     if (m_userId > 0) {
//         m_model->saveHighScore(m_userId);
//     }
// }

// // ---------- 键盘事件 ----------
// void MainWindow::keyPressEvent(QKeyEvent *event)
// {
//     switch (event->key()) {
//     case Qt::Key_Left:
//     case Qt::Key_A:
//         m_controller->moveLeft();
//         break;
//     case Qt::Key_Right:
//     case Qt::Key_D:
//         m_controller->moveRight();
//         break;
//     case Qt::Key_Down:
//     case Qt::Key_S:
//         m_controller->moveDown();
//         break;
//     case Qt::Key_Up:
//     case Qt::Key_W:
//         m_controller->rotateBlock();
//         break;
//     case Qt::Key_Space:
//         m_controller->hardDrop();
//         break;
//     case Qt::Key_P:
//     case Qt::Key_Escape:
//         if (m_controller->getState() == Playing) {
//             m_controller->pauseGame();
//         } else if (m_controller->getState() == Paused) {
//             m_controller->resumeGame();
//         }
//         break;
//     default:
//         QMainWindow::keyPressEvent(event);
//     }
// }

// void MainWindow::keyReleaseEvent(QKeyEvent *event)
// {
//     // 不处理 KeyRelease，避免重复触发
//     QMainWindow::keyReleaseEvent(event);
// }

// // ---------- 菜单槽函数 ----------
// void MainWindow::onShowRank()
// {
//     RankDialog rankDialog(this);
//     rankDialog.exec();  // 模态显示排行榜对话框
// }

// void MainWindow::onAbout()
// {
//     QMessageBox::about(this, "关于俄罗斯方块",
//                        "🎮 俄罗斯方块 Tetris\n\n"
//                        "版本: 1.0\n"
//                        "开发: 李镔泰, 何文生\n"
//                        "课程: C++ 课程设计\n"
//                        "技术: Qt 6 + SQLite\n\n"
//                        "操作说明:\n"
//                        "←/A: 左移  →/D: 右移\n"
//                        "↑/W: 旋转  ↓/S: 下移\n"
//                        "空格: 硬降  P/ESC: 暂停/继续");
// }
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rankdialog.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(int userId, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_userId(userId)
    , m_effectFrame(0)
    , m_isEffectPlaying(false)
{
    // 创建模型和控制器
    m_model = new GameModel(this);
    m_controller = new GameController(m_model, this);

    // ===== 特效定时器 =====
    m_effectTimer = new QTimer(this);
    m_effectTimer->setInterval(120);  // 每 120ms 刷新一帧
    connect(m_effectTimer, &QTimer::timeout, this, &MainWindow::onEffectTimeout);

    // 创建 UI
    initUI();
    createMenuBar();
    connectSignals();

    // 初始状态
    updateUI(Idle);

    setWindowTitle("俄罗斯方块 Tetris");
    setFixedSize(650, 700);

    qDebug() << "✅ MainWindow 创建完成，用户ID:" << m_userId;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------- 初始化界面 ----------
void MainWindow::initUI()
{
    // 创建中央部件
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // 主水平布局
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // ===== 左侧：游戏画布 =====
    m_gameWidget = new GameWidget(m_model, this);
    mainLayout->addWidget(m_gameWidget);

    // ===== 右侧：信息面板 =====
    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(10);

    // 标题
    QLabel *titleLabel = new QLabel("🎮 俄罗斯方块", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #FFD700;");
    rightLayout->addWidget(titleLabel);
    rightLayout->addSpacing(10);

    // 分数显示
    m_scoreLabel = new QLabel("得分: 0", this);
    m_scoreLabel->setStyleSheet("font-size: 14px; color: white;");
    rightLayout->addWidget(m_scoreLabel);

    m_linesLabel = new QLabel("行数: 0", this);
    m_linesLabel->setStyleSheet("font-size: 14px; color: white;");
    rightLayout->addWidget(m_linesLabel);

    m_levelLabel = new QLabel("等级: 1", this);
    m_levelLabel->setStyleSheet("font-size: 14px; color: white;");
    rightLayout->addWidget(m_levelLabel);

    m_statusLabel = new QLabel("状态: 待开始", this);
    m_statusLabel->setStyleSheet("font-size: 14px; color: #FFD700;");
    rightLayout->addWidget(m_statusLabel);

    rightLayout->addSpacing(10);

    // 下一个方块预览
    QLabel *previewTitle = new QLabel("⬇️ 下一个", this);
    previewTitle->setStyleSheet("font-size: 14px; color: white;");
    rightLayout->addWidget(previewTitle);

    m_previewWidget = new PreviewWidget(m_model, this);
    rightLayout->addWidget(m_previewWidget);

    rightLayout->addSpacing(15);

    // 速度控制
    QLabel *speedLabel = new QLabel("速度", this);
    speedLabel->setStyleSheet("color: white;");
    rightLayout->addWidget(speedLabel);

    m_speedSlider = new QSlider(Qt::Horizontal, this);
    m_speedSlider->setRange(1, 10);
    m_speedSlider->setValue(3);
    m_speedSlider->setTickPosition(QSlider::TicksBelow);
    m_speedSlider->setTickInterval(1);
    rightLayout->addWidget(m_speedSlider);

    rightLayout->addSpacing(10);

    // 音效控制
    m_soundCheck = new QCheckBox("🔊 音效", this);
    m_soundCheck->setStyleSheet("color: white;");
    m_soundCheck->setChecked(true);
    rightLayout->addWidget(m_soundCheck);

    rightLayout->addSpacing(15);

    // 控制按钮
    m_startBtn = new QPushButton("▶ 开始", this);
    m_startBtn->setFixedHeight(35);
    m_startBtn->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    rightLayout->addWidget(m_startBtn);

    m_pauseBtn = new QPushButton("⏸ 暂停", this);
    m_pauseBtn->setFixedHeight(35);
    m_pauseBtn->setStyleSheet("background-color: #FF9800; color: white; font-weight: bold;");
    m_pauseBtn->setEnabled(false);
    rightLayout->addWidget(m_pauseBtn);

    m_resetBtn = new QPushButton("🔄 重置", this);
    m_resetBtn->setFixedHeight(35);
    m_resetBtn->setStyleSheet("background-color: #f44336; color: white; font-weight: bold;");
    rightLayout->addWidget(m_resetBtn);

    rightLayout->addStretch();

    // 把右侧面板加入主布局
    mainLayout->addWidget(rightPanel);

    // 设置中央部件
    setCentralWidget(central);

    // 状态栏
    statusBar()->showMessage("按方向键控制方块，按 P 暂停/继续");
}

// ---------- 创建菜单栏 ----------
void MainWindow::createMenuBar()
{
    // 游戏菜单
    QMenu *gameMenu = menuBar()->addMenu("游戏(&G)");

    QAction *startAction = new QAction("开始", this);
    startAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(startAction, &QAction::triggered, this, &MainWindow::onStartClicked);
    gameMenu->addAction(startAction);

    QAction *pauseAction = new QAction("暂停/继续", this);
    pauseAction->setShortcut(QKeySequence("Ctrl+P"));
    connect(pauseAction, &QAction::triggered, this, &MainWindow::onPauseClicked);
    gameMenu->addAction(pauseAction);

    gameMenu->addSeparator();

    QAction *exitAction = new QAction("退出", this);
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    gameMenu->addAction(exitAction);

    // 视图菜单
    QMenu *viewMenu = menuBar()->addMenu("视图(&V)");
    QAction *rankAction = new QAction("排行榜", this);
    connect(rankAction, &QAction::triggered, this, &MainWindow::onShowRank);
    viewMenu->addAction(rankAction);

    // 帮助菜单
    QMenu *helpMenu = menuBar()->addMenu("帮助(&H)");
    QAction *aboutAction = new QAction("关于", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    helpMenu->addAction(aboutAction);
}

// ---------- 连接信号槽 ----------
void MainWindow::connectSignals()
{
    // 按钮信号
    connect(m_startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(m_pauseBtn, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(m_resetBtn, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    // 速度滑块
    connect(m_speedSlider, &QSlider::valueChanged, [this](int value) {
        qDebug() << "速度滑块:" << value;
    });

    // Controller 信号
    connect(m_controller, &GameController::gameStateChanged,
            this, &MainWindow::updateUI);
    connect(m_controller, &GameController::gameOver,
            this, &MainWindow::onGameOver);

    // Model 信号
    connect(m_model, &GameModel::scoreChanged,
            this, &MainWindow::updateScore);
    connect(m_model, &GameModel::linesChanged,
            this, &MainWindow::updateLines);
    connect(m_model, &GameModel::levelChanged,
            this, &MainWindow::updateLevel);

    // ===== 特效信号 =====
    connect(m_model, &GameModel::rowsPendingClear,
            this, &MainWindow::onRowsPendingClear);
}

// ---------- 按钮槽函数 ----------
void MainWindow::onStartClicked()
{
    qDebug() << "🎮 点击「开始」";
    m_controller->startGame();
}

void MainWindow::onPauseClicked()
{
    if (m_controller->getState() == Playing) {
        m_controller->pauseGame();
    } else if (m_controller->getState() == Paused) {
        m_controller->resumeGame();
    }
}

void MainWindow::onResetClicked()
{
    qDebug() << "🔄 点击「重置」";
    // 如果正在播放特效，先停止
    if (m_isEffectPlaying) {
        m_effectTimer->stop();
        m_isEffectPlaying = false;
        m_effectRows.clear();
        m_effectFrame = 0;
    }
    m_controller->stopGame();
    m_controller->startGame();
}

// ---------- 更新 UI ----------
void MainWindow::updateUI(GameState state)
{
    QString statusText;
    switch (state) {
    case Idle:
        statusText = "待开始";
        m_startBtn->setEnabled(true);
        m_pauseBtn->setEnabled(false);
        m_pauseBtn->setText("⏸ 暂停");
        break;
    case Playing:
        statusText = "🟢 游戏中";
        m_startBtn->setEnabled(false);
        m_pauseBtn->setEnabled(true);
        m_pauseBtn->setText("⏸ 暂停");
        break;
    case Paused:
        statusText = "⏸ 已暂停";
        m_startBtn->setEnabled(false);
        m_pauseBtn->setEnabled(true);
        m_pauseBtn->setText("▶ 继续");
        break;
    case GameOver:
        statusText = "💀 游戏结束";
        m_startBtn->setEnabled(true);
        m_pauseBtn->setEnabled(false);
        m_pauseBtn->setText("⏸ 暂停");
        break;
    }
    m_statusLabel->setText(statusText);
}

void MainWindow::updateScore(int score)
{
    m_scoreLabel->setText(QString("得分: %1").arg(score));
}

void MainWindow::updateLines(int lines)
{
    m_linesLabel->setText(QString("行数: %1").arg(lines));
}

void MainWindow::updateLevel(int level)
{
    m_levelLabel->setText(QString("等级: %1").arg(level));
}

void MainWindow::onGameOver(int finalScore)
{
    // 如果正在播放特效，先停止
    if (m_isEffectPlaying) {
        m_effectTimer->stop();
        m_isEffectPlaying = false;
        m_effectRows.clear();
        m_effectFrame = 0;
    }

    QMessageBox::information(this, "游戏结束",
                             QString("🎮 游戏结束！\n\n最终得分: %1\n消除行数: %2\n等级: %3")
                                 .arg(finalScore)
                                 .arg(m_model->getLines())
                                 .arg(m_model->getLevel()));

    // 保存最高分
    if (m_userId > 0) {
        m_model->saveHighScore(m_userId);
    }
}

// ---------- 键盘事件 ----------
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 如果正在播放特效，不处理任何操作
    if (m_isEffectPlaying) {
        return;
    }

    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
        m_controller->moveLeft();
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        m_controller->moveRight();
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        m_controller->moveDown();
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        m_controller->rotateBlock();
        break;
    case Qt::Key_Space:
        m_controller->hardDrop();
        break;
    case Qt::Key_P:
    case Qt::Key_Escape:
        if (m_controller->getState() == Playing) {
            m_controller->pauseGame();
        } else if (m_controller->getState() == Paused) {
            m_controller->resumeGame();
        }
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // 不处理 KeyRelease，避免重复触发
    QMainWindow::keyReleaseEvent(event);
}

// ---------- 菜单槽函数 ----------
void MainWindow::onShowRank()
{
    RankDialog rankDialog(this);
    rankDialog.exec();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "关于俄罗斯方块",
                       "🎮 俄罗斯方块 Tetris\n\n"
                       "版本: 1.0\n"
                       "开发: 李镔泰, 何文生\n"
                       "课程: C++ 课程设计\n"
                       "技术: Qt 6 + SQLite\n\n"
                       "操作说明:\n"
                       "←/A: 左移  →/D: 右移\n"
                       "↑/W: 旋转  ↓/S: 下移\n"
                       "空格: 硬降  P/ESC: 暂停/继续");
}

// =============================================
// ===== 特效相关函数 =====
// =============================================

void MainWindow::onRowsPendingClear(QVector<int> rows)
{
    if (rows.isEmpty() || m_isEffectPlaying) return;

    qDebug() << "🎨 收到行消除请求，行号:" << rows;

    // 暂停游戏
    m_controller->pauseGame();

    // 保存特效数据
    m_effectRows = rows;
    m_effectFrame = 0;
    m_isEffectPlaying = true;

    // 保存棋盘快照（在 GameModel 中已保存）

    // 启动特效定时器
    m_effectTimer->start();
}

void MainWindow::onEffectTimeout()
{
    m_effectFrame++;

    if (m_effectFrame <= 4) {
        // 闪烁阶段：奇数帧恢复快照，偶数帧变白
        if (m_effectFrame % 2 == 1) {
            // 恢复棋盘
            m_model->restoreBoardSnapshot();
        } else {
            // 将待消除的行变为白色（颜色编号 8）
            for (int row : m_effectRows) {
                for (int col = 0; col < 10; col++) {
                    m_model->setBoardCell(row, col, 8);
                }
            }
        }
        m_gameWidget->update();
        m_previewWidget->update();

    } else if (m_effectFrame == 5) {
        // 最后再闪一次白色，然后消除
        for (int row : m_effectRows) {
            for (int col = 0; col < 10; col++) {
                m_model->setBoardCell(row, col, 8);
            }
        }
        m_gameWidget->update();

    } else if (m_effectFrame == 6) {
        // 特效结束
        m_effectTimer->stop();
        m_isEffectPlaying = false;

        // 真正执行行消除
        m_model->performClearRows();

        // 清空特效数据
        m_effectRows.clear();
        m_effectFrame = 0;

        // 恢复游戏
        m_controller->resumeGame();

        qDebug() << "✅ 特效结束，游戏继续";
    }
}
