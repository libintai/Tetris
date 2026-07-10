#ifndef RANKDIALOG_H
#define RANKDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include "databasehelper.h"

class RankDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RankDialog(QWidget *parent = nullptr);

private slots:
    void onRefreshClicked();

private:
    QTableWidget *m_table;
    QPushButton *m_refreshBtn;
    QPushButton *m_closeBtn;

    void initUI();
    void loadRankData();
};

#endif // RANKDIALOG_H
