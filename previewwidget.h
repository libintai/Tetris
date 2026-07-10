#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include "gamemodel.h"

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(GameModel *model, QWidget *parent = nullptr);

    QSize sizeHint() const override { return QSize(120, 120); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    GameModel *m_model;
    void drawBlock(QPainter &painter, int row, int col, int color);
};

#endif // PREVIEWWIDGET_H
