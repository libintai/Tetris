#include "databasehelper.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QDateTime>

// ---------- 构造函数 & 析构函数 ----------
DatabaseHelper::DatabaseHelper(QObject *parent)
    : QObject(parent)
{
    // 构造函数里什么都不做，让外部调用 initDatabase()
}

DatabaseHelper::~DatabaseHelper()
{
    // 如果数据库还开着，就关闭它
    if (m_db.isOpen()) {
        m_db.close();
    }
}

// ---------- 初始化数据库 ----------
bool DatabaseHelper::initDatabase(const QString &dbPath)
{
    // 添加 SQLite 数据库驱动
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qDebug() << "❌ 数据库打开失败：" << m_db.lastError().text();
        return false;
    }

    qDebug() << "✅ 数据库连接成功！路径：" << dbPath;

    // 创建表
    if (!createTables()) {
        qDebug() << "❌ 创建表失败！";
        return false;
    }

    return true;
}

// ---------- 创建表 ----------
bool DatabaseHelper::createTables()
{
    QSqlQuery query;

    // 创建 users 表（用户信息）
    QString createUsers = R"(
        CREATE TABLE IF NOT EXISTS users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username VARCHAR(50) NOT NULL UNIQUE,
            password_hash VARCHAR(64) NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    // 创建 scores 表（成绩记录）
    QString createScores = R"(
        CREATE TABLE IF NOT EXISTS scores (
            score_id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            score INTEGER NOT NULL,
            lines INTEGER NOT NULL,
            game_date DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE
        )
    )";

    // 创建索引（优化排行榜查询速度）
    QString createIndex = R"(
        CREATE INDEX IF NOT EXISTS idx_scores_score
        ON scores(score DESC)
    )";

    if (!query.exec(createUsers)) {
        qDebug() << "❌ 创建 users 表失败：" << query.lastError().text();
        return false;
    }

    if (!query.exec(createScores)) {
        qDebug() << "❌ 创建 scores 表失败：" << query.lastError().text();
        return false;
    }

    if (!query.exec(createIndex)) {
        qDebug() << "❌ 创建索引失败：" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ 数据表创建成功（users, scores）";
    return true;
}

// ---------- SHA-256 密码加密 ----------
QString DatabaseHelper::encryptPassword(const QString &password)
{
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
        );
    return hash.toHex();  // 转为16进制字符串
}

// ---------- 用户注册 ----------
bool DatabaseHelper::registerUser(const QString &username, const QString &password)
{
    // 先检查用户名是否已存在
    if (userExists(username)) {
        qDebug() << "⚠️ 用户名已存在：" << username;
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password_hash) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(encryptPassword(password));

    if (!query.exec()) {
        qDebug() << "❌ 注册失败：" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ 注册成功：" << username;
    return true;
}

// ---------- 用户登录 ----------
int DatabaseHelper::loginUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT user_id FROM users WHERE username = ? AND password_hash = ?");
    query.addBindValue(username);
    query.addBindValue(encryptPassword(password));

    if (query.exec() && query.next()) {
        int userId = query.value(0).toInt();
        qDebug() << "✅ 登录成功：" << username << "(user_id=" << userId << ")";
        return userId;
    }

    qDebug() << "❌ 登录失败：用户名或密码错误";
    return -1;
}

// ---------- 检查用户名是否存在 ----------
bool DatabaseHelper::userExists(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
    query.addBindValue(username);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}

// ---------- 保存成绩 ----------
bool DatabaseHelper::saveScore(int userId, int score, int lines)
{
    QSqlQuery query;
    query.prepare("INSERT INTO scores (user_id, score, lines) VALUES (?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(score);
    query.addBindValue(lines);

    if (!query.exec()) {
        qDebug() << "❌ 保存成绩失败：" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ 成绩保存成功：user_id=" << userId
             << ", score=" << score << ", lines=" << lines;
    return true;
}

// ---------- 获取用户最高分 ----------
int DatabaseHelper::getHighScore(int userId)
{
    QSqlQuery query;
    query.prepare("SELECT MAX(score) FROM scores WHERE user_id = ?");
    query.addBindValue(userId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();  // 如果没有记录，返回 0
    }

    return 0;
}

// ---------- 获取排行榜 ----------
QList<RankItem> DatabaseHelper::getRankList(int limit)
{
    QList<RankItem> result;

    QSqlQuery query;
    query.prepare(R"(
        SELECT u.username, s.score, s.lines, s.game_date
        FROM scores s
        JOIN users u ON s.user_id = u.user_id
        ORDER BY s.score DESC
        LIMIT ?
    )");
    query.addBindValue(limit);

    if (!query.exec()) {
        qDebug() << "❌ 获取排行榜失败：" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        RankItem item;
        item.username = query.value(0).toString();
        item.score = query.value(1).toInt();
        item.lines = query.value(2).toInt();
        item.date = query.value(3).toDateTime().toString("yyyy-MM-dd");
        result.append(item);
    }

    qDebug() << "✅ 获取排行榜成功，共" << result.size() << "条记录";
    return result;
}
