#include "previewwidget.h"
#include <QPainter>

PreviewWidget::PreviewWidget(GameModel *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
{
    setFixedSize(sizeHint());
    // 当棋盘变化时也刷新预览（虽然预览本身可能没变，但为了保险）
    connect(m_model, &GameModel::boardChanged, this, QOverload<>::of(&QWidget::update));
}

void PreviewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor(20, 20, 35));

    const int blockSize = 24;
    int offsetX = (width() - 4 * blockSize) / 2;
    int offsetY = (height() - 4 * blockSize) / 2;

    const Tetromino &next = m_model->getNextBlock();
    const auto &shape = next.getShape();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] != 0) {
                int x = offsetX + j * blockSize;
                int y = offsetY + i * blockSize;
                // 使用和 GameWidget 相同的颜色映射
                QColor colors[] = {
                    QColor(0,0,0),
                    QColor(0,255,255), QColor(255,255,0),
                    QColor(160,32,240), QColor(0,255,0),
                    QColor(255,0,0), QColor(0,0,255),
                    QColor(255,165,0)
                };
                int colorIdx = next.getColor();
                if (colorIdx < 1 || colorIdx > 7) colorIdx = 1;
                painter.fillRect(x, y, blockSize, blockSize, colors[colorIdx]);
                painter.setPen(QPen(QColor(0,0,0,80), 1));
                painter.drawRect(x, y, blockSize, blockSize);
            }
        }
    }
}
