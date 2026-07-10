#include <QApplication>
#include "logindialog.h"
#include "mainwindow.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. 先显示登录对话框
    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        // 2. 登录成功，获取用户ID
        int userId = login.getUserId();
        qDebug() << "✅ 登录成功，用户ID:" << userId;

        // 3. 创建主窗口并传入用户ID
        MainWindow w(userId);
        w.show();

        // 4. 进入程序事件循环
        return a.exec();
    } else {
        // 5. 用户取消登录，直接退出程序
        qDebug() << "❌ 用户取消登录，程序退出";
        return 0;
    }
}
