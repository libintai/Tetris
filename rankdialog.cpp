#include "rankdialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QBrush>

RankDialog::RankDialog(QWidget *parent)
    : QDialog(parent)
{
    initUI();
    loadRankData();

    setWindowTitle("🏆 排行榜");
    setFixedSize(500, 450);
    setModal(true);
}

void RankDialog::initUI()
{
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 标题
    QLabel *title = new QLabel("🏆 排行榜 Top 10", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #FFD700;");
    mainLayout->addWidget(title);

    // 表格
    m_table = new QTableWidget(this);
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({"排名", "玩家", "得分", "行数", "日期"});
    m_table->setEditTriggers(QTableWidget::NoEditTriggers);  // 不可编辑
    m_table->setSelectionBehavior(QTableWidget::SelectRows);  // 选择整行
    m_table->setAlternatingRowColors(true);

    // 设置列宽
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    m_table->setStyleSheet(
        "QTableWidget { background-color: #1a1a2e; color: white; gridline-color: #333; }"
        "QTableWidget::item { padding: 5px; }"
        "QHeaderView::section { background-color: #16213e; color: #FFD700; padding: 5px; }"
        "QTableWidget::item:selected { background-color: #4CAF50; }"
        );

    mainLayout->addWidget(m_table);

    // 按钮
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_refreshBtn = new QPushButton("🔄 刷新", this);
    m_refreshBtn->setFixedHeight(35);
    m_refreshBtn->setStyleSheet("background-color: #2196F3; color: white; font-weight: bold; border-radius: 4px;");
    btnLayout->addWidget(m_refreshBtn);

    m_closeBtn = new QPushButton("✕ 关闭", this);
    m_closeBtn->setFixedHeight(35);
    m_closeBtn->setStyleSheet("background-color: #f44336; color: white; font-weight: bold; border-radius: 4px;");
    btnLayout->addWidget(m_closeBtn);

    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    // 连接信号
    connect(m_refreshBtn, &QPushButton::clicked, this, &RankDialog::onRefreshClicked);
    connect(m_closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    setStyleSheet("QDialog { background-color: #1a1a2e; }");
}

void RankDialog::loadRankData()
{
    DatabaseHelper db;
    if (!db.initDatabase()) {
        QMessageBox::warning(this, "错误", "数据库连接失败！");
        return;
    }

    QList<RankItem> rankList = db.getRankList(10);

    m_table->setRowCount(rankList.size());

    if (rankList.isEmpty()) {
        m_table->setRowCount(1);
        m_table->setItem(0, 0, new QTableWidgetItem("暂无数据"));
        m_table->setSpan(0, 0, 1, 5);
        return;
    }

    for (int i = 0; i < rankList.size(); i++) {
        // 排名
        QString rankStr;
        if (i == 0) rankStr = "🥇";
        else if (i == 1) rankStr = "🥈";
        else if (i == 2) rankStr = "🥉";
        else rankStr = QString::number(i + 1);

        m_table->setItem(i, 0, new QTableWidgetItem(rankStr));
        m_table->setItem(i, 1, new QTableWidgetItem(rankList[i].username));
        m_table->setItem(i, 2, new QTableWidgetItem(QString::number(rankList[i].score)));
        m_table->setItem(i, 3, new QTableWidgetItem(QString::number(rankList[i].lines)));
        m_table->setItem(i, 4, new QTableWidgetItem(rankList[i].date));

        // 对齐方式
        for (int col = 0; col < 5; col++) {
            m_table->item(i, col)->setTextAlignment(Qt::AlignCenter);
        }

        // 高亮前三名
        if (i == 0) {
            m_table->item(i, 1)->setForeground(QBrush(QColor("#FFD700")));
        } else if (i == 1) {
            m_table->item(i, 1)->setForeground(QBrush(QColor("#C0C0C0")));
        } else if (i == 2) {
            m_table->item(i, 1)->setForeground(QBrush(QColor("#CD7F32")));
        }
    }

    qDebug() << "✅ 排行榜加载完成，共" << rankList.size() << "条记录";
}

void RankDialog::onRefreshClicked()
{
    loadRankData();
}
