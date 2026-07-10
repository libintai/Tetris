#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include "gamemodel.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    // 构造函数：传入 GameModel 指针，用于获取数据
    explicit GameWidget(GameModel *model, QWidget *parent = nullptr);

    // 重写 sizeHint()，告诉布局管理器我们想要的大小
    QSize sizeHint() const override { return QSize(300, 600); }

protected:
    // 重写绘图事件
    void paintEvent(QPaintEvent *event) override;

private:
    GameModel *m_model;  // 指向游戏模型（只读，不修改）

    // 辅助函数：绘制一个方块（格子）
    void drawBlock(QPainter &painter, int row, int col, int color);
};

#endif // GAMEWIDGET_H
