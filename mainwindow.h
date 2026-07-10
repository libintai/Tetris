// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>
// #include <QLabel>
// #include <QPushButton>
// #include <QSlider>
// #include <QCheckBox>
// #include <QTimer>
// #include "gamemodel.h"
// #include "gamecontroller.h"
// #include "gamewidget.h"
// #include "previewwidget.h"

// QT_BEGIN_NAMESPACE
// namespace Ui { class MainWindow; }
// QT_END_NAMESPACE

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT

// public:
//     MainWindow(int userId = -1, QWidget *parent = nullptr);
//     ~MainWindow();

// protected:
//     // 重写键盘事件，捕获按键操作
//     void keyPressEvent(QKeyEvent *event) override;
//     void keyReleaseEvent(QKeyEvent *event) override;

// private slots:
//     // 按钮槽函数
//     void onStartClicked();
//     void onPauseClicked();
//     void onResetClicked();

//     // 信号响应槽函数
//     void updateUI(GameState state);
//     void updateScore(int score);
//     void updateLines(int lines);
//     void updateLevel(int level);
//     void onGameOver(int finalScore);

//     // 菜单槽函数
//     void onShowRank();
//     void onAbout();

// private:
//     Ui::MainWindow *ui;  // Qt Designer 生成的 UI 对象

//     // 核心对象
//     GameModel *m_model;
//     GameController *m_controller;
//     GameWidget *m_gameWidget;
//     PreviewWidget *m_previewWidget;

//     // 界面控件（手动创建，不使用 .ui 文件）
//     QLabel *m_scoreLabel;
//     QLabel *m_linesLabel;
//     QLabel *m_levelLabel;
//     QLabel *m_statusLabel;
//     QPushButton *m_startBtn;
//     QPushButton *m_pauseBtn;
//     QPushButton *m_resetBtn;
//     QSlider *m_speedSlider;
//     QCheckBox *m_soundCheck;

//     // 用户 ID（从登录对话框传入）
//     int m_userId;

//     void initUI();           // 初始化界面
//     void createMenuBar();    // 创建菜单栏
//     void connectSignals();   // 连接信号槽
//     void updateButtonState(); // 更新按钮状态
// };

// #endif // MAINWINDOW_H
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QTimer>
#include "gamemodel.h"
#include "gamecontroller.h"
#include "gamewidget.h"
#include "previewwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int userId = -1, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 重写键盘事件，捕获按键操作
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    // 按钮槽函数
    void onStartClicked();
    void onPauseClicked();
    void onResetClicked();

    // 信号响应槽函数
    void updateUI(GameState state);
    void updateScore(int score);
    void updateLines(int lines);
    void updateLevel(int level);
    void onGameOver(int finalScore);

    // 菜单槽函数
    void onShowRank();
    void onAbout();

    // ===== 特效相关槽函数 =====
    void onRowsPendingClear(QVector<int> rows);  // 收到行消除请求
    void onEffectTimeout();                       // 特效动画帧更新

private:
    Ui::MainWindow *ui;  // Qt Designer 生成的 UI 对象

    // 核心对象
    GameModel *m_model;
    GameController *m_controller;
    GameWidget *m_gameWidget;
    PreviewWidget *m_previewWidget;

    // 界面控件（手动创建，不使用 .ui 文件）
    QLabel *m_scoreLabel;
    QLabel *m_linesLabel;
    QLabel *m_levelLabel;
    QLabel *m_statusLabel;
    QPushButton *m_startBtn;
    QPushButton *m_pauseBtn;
    QPushButton *m_resetBtn;
    QSlider *m_speedSlider;
    QCheckBox *m_soundCheck;

    // 用户 ID（从登录对话框传入）
    int m_userId;

    // ===== 特效相关成员 =====
    QTimer *m_effectTimer;          // 特效动画定时器
    QVector<int> m_effectRows;      // 当前正在播放特效的行
    int m_effectFrame;              // 当前特效帧数（0开始）
    bool m_isEffectPlaying;         // 是否正在播放特效

    void initUI();           // 初始化界面
    void createMenuBar();    // 创建菜单栏
    void connectSignals();   // 连接信号槽
    void updateButtonState(); // 更新按钮状态
};

#endif // MAINWINDOW_H
