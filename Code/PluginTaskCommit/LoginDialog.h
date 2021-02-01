#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

namespace Plugins {

/**
 * @brief The LoginDialog class
 * * 登录窗口 *
 */
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_loginPushButton_clicked();

private:
    Ui::LoginDialog *ui;
    QString user;
    QString password;

};
}
#endif // LOGINDIALOG_H
