#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "databasehelper.h"

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    // 获取登录后的用户ID（登录成功返回正数，未登录返回 -1）
    int getUserId() const { return m_userId; }

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    // UI 控件
    QLabel *m_titleLabel;
    QLabel *m_usernameLabel;
    QLineEdit *m_usernameEdit;
    QLabel *m_passwordLabel;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginBtn;
    QPushButton *m_registerBtn;
    QLabel *m_statusLabel;

    int m_userId;  // 登录成功后保存用户ID

    void initUI();
    void connectSignals();
};

#endif // LOGINDIALOG_H
