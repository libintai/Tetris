#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QList>

// 排行榜条目结构体（用于存储一条排行榜数据）
struct RankItem {
    QString username;   // 玩家名
    int score;          // 分数
    int lines;          // 消除行数
    QString date;       // 游戏日期
};

class DatabaseHelper : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseHelper(QObject *parent = nullptr);
    ~DatabaseHelper();

    // ---------- 初始化 ----------
    // 打开数据库连接，如果表不存在则创建
    bool initDatabase(const QString &dbPath = "tetris.db");

    // ---------- 用户管理 ----------
    // 注册新用户（密码会自动加密存储）
    bool registerUser(const QString &username, const QString &password);
    // 登录验证，成功返回 user_id，失败返回 -1
    int loginUser(const QString &username, const QString &password);
    // 检查用户名是否已被占用
    bool userExists(const QString &username);

    // ---------- 成绩管理 ----------
    // 保存游戏成绩
    bool saveScore(int userId, int score, int lines);
    // 获取某个用户的最高分
    int getHighScore(int userId);
    // 获取排行榜（默认取前10名）
    QList<RankItem> getRankList(int limit = 10);

private:
    QSqlDatabase m_db;  // 数据库连接对象

    // 内部辅助函数
    QString encryptPassword(const QString &password);  // SHA-256 加密
    bool createTables();  // 创建表
};

#endif // DATABASEHELPER_H
