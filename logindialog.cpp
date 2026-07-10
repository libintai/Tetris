#include "logindialog.h"
#include <QFont>
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , m_userId(-1)
{
    initUI();
    connectSignals();

    setWindowTitle("俄罗斯方块 - 登录");
    setFixedSize(350, 400);
    setModal(true);  // 模态对话框，必须先处理才能操作主窗口
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::initUI()
{
    // 主垂直布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // 标题
    m_titleLabel = new QLabel("🎮 俄罗斯方块", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #FFD700;");
    mainLayout->addWidget(m_titleLabel);

    QLabel *subTitle = new QLabel("Tetris", this);
    subTitle->setAlignment(Qt::AlignCenter);
    subTitle->setStyleSheet("font-size: 14px; color: #aaa;");
    mainLayout->addWidget(subTitle);

    mainLayout->addSpacing(20);

    // 用户名
    m_usernameLabel = new QLabel("👤 用户名", this);
    m_usernameLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(m_usernameLabel);

    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("请输入用户名");
    m_usernameEdit->setStyleSheet("padding: 8px; border-radius: 4px;");
    mainLayout->addWidget(m_usernameEdit);

    // 密码
    m_passwordLabel = new QLabel("🔒 密码", this);
    m_passwordLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(m_passwordLabel);

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("请输入密码");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setStyleSheet("padding: 8px; border-radius: 4px;");
    mainLayout->addWidget(m_passwordEdit);

    mainLayout->addSpacing(10);

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout();
    m_loginBtn = new QPushButton("登 录", this);
    m_loginBtn->setFixedHeight(40);
    m_loginBtn->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; font-size: 14px; border-radius: 6px;");
    btnLayout->addWidget(m_loginBtn);

    m_registerBtn = new QPushButton("注 册", this);
    m_registerBtn->setFixedHeight(40);
    m_registerBtn->setStyleSheet("background-color: #2196F3; color: white; font-weight: bold; font-size: 14px; border-radius: 6px;");
    btnLayout->addWidget(m_registerBtn);

    mainLayout->addLayout(btnLayout);

    // 状态信息
    m_statusLabel = new QLabel("请输入用户名和密码", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: #FFD700; font-size: 12px;");
    mainLayout->addWidget(m_statusLabel);

    // 设置背景色
    setStyleSheet("QDialog { background-color: #1a1a2e; }");
}

void LoginDialog::connectSignals()
{
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(m_registerBtn, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
}

void LoginDialog::onLoginClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        m_statusLabel->setText("⚠️ 用户名和密码不能为空！");
        return;
    }

    DatabaseHelper db;
    if (!db.initDatabase()) {
        m_statusLabel->setText("❌ 数据库连接失败！");
        return;
    }

    int userId = db.loginUser(username, password);
    if (userId > 0) {
        m_userId = userId;
        m_statusLabel->setText("✅ 登录成功！欢迎 " + username);
        qDebug() << "✅ 登录成功，user_id:" << userId;
        accept();  // 关闭对话框，返回 QDialog::Accepted
    } else {
        m_statusLabel->setText("❌ 用户名或密码错误！");
    }
}

void LoginDialog::onRegisterClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        m_statusLabel->setText("⚠️ 用户名和密码不能为空！");
        return;
    }

    if (password.length() < 6) {
        m_statusLabel->setText("⚠️ 密码长度不能少于6位！");
        return;
    }

    DatabaseHelper db;
    if (!db.initDatabase()) {
        m_statusLabel->setText("❌ 数据库连接失败！");
        return;
    }

    bool success = db.registerUser(username, password);
    if (success) {
        m_statusLabel->setText("✅ 注册成功！请点击「登录」进入游戏");
        m_usernameEdit->setText(username);
        m_passwordEdit->clear();
        QMessageBox::information(this, "注册成功", "🎉 注册成功！\n\n用户名: " + username + "\n请点击「登录」进入游戏");
    } else {
        m_statusLabel->setText("❌ 用户名已被占用，请换个名字！");
    }
}
