// #include "gamewidget.h"
// #include <QPainter>
// #include <QPen>
// #include <QBrush>

// // ---------- 构造函数 ----------
// GameWidget::GameWidget(GameModel *model, QWidget *parent)
//     : QWidget(parent)
//     , m_model(model)
// {
//     // 设置固定大小（由 sizeHint 决定）
//     setFixedSize(sizeHint());

//     // 连接 Model 的 boardChanged 信号，当棋盘变化时自动重绘
//     connect(m_model, &GameModel::boardChanged, this, QOverload<>::of(&QWidget::update));
// }

// // ---------- 绘图事件 ----------
// void GameWidget::paintEvent(QPaintEvent *event)
// {
//     Q_UNUSED(event);

//     // 创建 QPainter 对象
//     QPainter painter(this);

//     // 抗锯齿：让方块边缘更平滑（可选）
//     painter.setRenderHint(QPainter::Antialiasing, false);

//     // ---------- 1. 绘制背景 ----------
//     painter.fillRect(rect(), QColor(30, 30, 46));  // 深色背景

//     // 计算每个格子的大小（30×30 像素）
//     const int blockSize = 30;

//     // ---------- 2. 绘制已固定的方块 ----------
//     for (int row = 0; row < 20; row++) {
//         for (int col = 0; col < 10; col++) {
//             int color = m_model->getBoardCell(row, col);
//             if (color != 0) {
//                 drawBlock(painter, row, col, color);
//             }
//         }
//     }

//     // ---------- 3. 绘制当前活动方块 ----------
//     const Tetromino &current = m_model->getCurrentBlock();
//     const auto &shape = current.getShape();
//     int x = current.getX();
//     int y = current.getY();

//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             if (shape[i][j] != 0) {
//                 // 只绘制在棋盘范围内的格子
//                 int boardRow = y + i;
//                 int boardCol = x + j;
//                 if (boardRow >= 0 && boardRow < 20 && boardCol >= 0 && boardCol < 10) {
//                     drawBlock(painter, boardRow, boardCol, current.getColor());
//                 }
//             }
//         }
//     }

//     // ---------- 4. 绘制网格线（增强视觉效果）----------
//     painter.setPen(QPen(QColor(60, 60, 80), 1));
//     for (int row = 0; row <= 20; row++) {
//         painter.drawLine(0, row * blockSize, 300, row * blockSize);
//     }
//     for (int col = 0; col <= 10; col++) {
//         painter.drawLine(col * blockSize, 0, col * blockSize, 600);
//     }
// }

// // ---------- 绘制一个方块 ----------
// void GameWidget::drawBlock(QPainter &painter, int row, int col, int color)
// {
//     // 边界检查
//     if (row < 0 || row >= 20 || col < 0 || col >= 10) return;

//     const int blockSize = 30;
//     int x = col * blockSize;
//     int y = row * blockSize;

//     // 颜色映射表（七种标准色 + 一个默认）
//     // 索引 0 不用，1~7 对应七种方块
//     QColor colors[] = {
//         QColor(0, 0, 0),           // 0: 空（不用）
//         QColor(0, 255, 255),       // 1: I - 青色
//         QColor(255, 255, 0),       // 2: O - 黄色
//         QColor(160, 32, 240),      // 3: T - 紫色
//         QColor(0, 255, 0),         // 4: S - 绿色
//         QColor(255, 0, 0),         // 5: Z - 红色
//         QColor(0, 0, 255),         // 6: J - 蓝色
//         QColor(255, 165, 0)        // 7: L - 橙色
//     };

//     // 确保颜色索引在有效范围内
//     if (color < 1 || color > 7) {
//         color = 1;  // 默认用青色
//     }

//     // 填充方块主体
//     painter.fillRect(x, y, blockSize, blockSize, colors[color]);

//     // 添加高光边框效果（让方块看起来更立体）
//     // 左上边框（亮色）
//     painter.setPen(QPen(QColor(255, 255, 255, 180), 2));
//     painter.drawLine(x, y, x + blockSize, y);
//     painter.drawLine(x, y, x, y + blockSize);

//     // 右下边框（暗色）
//     painter.setPen(QPen(QColor(0, 0, 0, 150), 2));
//     painter.drawLine(x + blockSize, y, x + blockSize, y + blockSize);
//     painter.drawLine(x, y + blockSize, x + blockSize, y + blockSize);

//     // 最外边框（细线，让方块之间更清晰）
//     painter.setPen(QPen(QColor(0, 0, 0, 80), 1));
//     painter.drawRect(x, y, blockSize, blockSize);
// }
#include "gamewidget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>

// ---------- 构造函数 ----------
GameWidget::GameWidget(GameModel *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
{
    // 设置固定大小（由 sizeHint 决定）
    setFixedSize(sizeHint());

    // 连接 Model 的 boardChanged 信号，当棋盘变化时自动重绘
    connect(m_model, &GameModel::boardChanged, this, QOverload<>::of(&QWidget::update));
}

// ---------- 绘图事件 ----------
void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // 创建 QPainter 对象
    QPainter painter(this);

    // 抗锯齿：让方块边缘更平滑（可选）
    painter.setRenderHint(QPainter::Antialiasing, false);

    // ---------- 1. 绘制背景 ----------
    painter.fillRect(rect(), QColor(30, 30, 46));  // 深色背景

    // 计算每个格子的大小（30×30 像素）
    const int blockSize = 30;

    // ---------- 2. 绘制已固定的方块 ----------
    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 10; col++) {
            int color = m_model->getBoardCell(row, col);
            if (color != 0) {
                drawBlock(painter, row, col, color);
            }
        }
    }

    // ---------- 3. 绘制当前活动方块 ----------
    const Tetromino &current = m_model->getCurrentBlock();
    const auto &shape = current.getShape();
    int x = current.getX();
    int y = current.getY();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] != 0) {
                // 只绘制在棋盘范围内的格子
                int boardRow = y + i;
                int boardCol = x + j;
                if (boardRow >= 0 && boardRow < 20 && boardCol >= 0 && boardCol < 10) {
                    drawBlock(painter, boardRow, boardCol, current.getColor());
                }
            }
        }
    }

    // ---------- 4. 绘制网格线（增强视觉效果）----------
    painter.setPen(QPen(QColor(60, 60, 80), 1));
    for (int row = 0; row <= 20; row++) {
        painter.drawLine(0, row * blockSize, 300, row * blockSize);
    }
    for (int col = 0; col <= 10; col++) {
        painter.drawLine(col * blockSize, 0, col * blockSize, 600);
    }
}

// ---------- 绘制一个方块 ----------
void GameWidget::drawBlock(QPainter &painter, int row, int col, int color)
{
    // 边界检查
    if (row < 0 || row >= 20 || col < 0 || col >= 10) return;

    const int blockSize = 30;
    int x = col * blockSize;
    int y = row * blockSize;

    // ===== 颜色映射表（增加白色用于特效） =====
    QColor colors[] = {
        QColor(0, 0, 0),           // 0: 空（不用）
        QColor(0, 255, 255),       // 1: I - 青色
        QColor(255, 255, 0),       // 2: O - 黄色
        QColor(160, 32, 240),      // 3: T - 紫色
        QColor(0, 255, 0),         // 4: S - 绿色
        QColor(255, 0, 0),         // 5: Z - 红色
        QColor(0, 0, 255),         // 6: J - 蓝色
        QColor(255, 165, 0),       // 7: L - 橙色
        QColor(255, 255, 255)      // 8: 白色（特效用）
    };

    // 确保颜色索引在有效范围内
    if (color < 1 || color > 8) {
        color = 1;  // 默认用青色
    }

    // 填充方块主体
    painter.fillRect(x, y, blockSize, blockSize, colors[color]);

    // 添加高光边框效果（让方块看起来更立体）
    // 左上边框（亮色）
    painter.setPen(QPen(QColor(255, 255, 255, 180), 2));
    painter.drawLine(x, y, x + blockSize, y);
    painter.drawLine(x, y, x, y + blockSize);

    // 右下边框（暗色）
    painter.setPen(QPen(QColor(0, 0, 0, 150), 2));
    painter.drawLine(x + blockSize, y, x + blockSize, y + blockSize);
    painter.drawLine(x, y + blockSize, x + blockSize, y + blockSize);

    // 最外边框（细线，让方块之间更清晰）
    painter.setPen(QPen(QColor(0, 0, 0, 80), 1));
    painter.drawRect(x, y, blockSize, blockSize);
}
