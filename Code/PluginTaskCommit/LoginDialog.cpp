#include "LoginDialog.h"
#include "ui_LoginDialog.h"

#include "taskmgm/TaskCommit.h"

#include <QMessageBox>

using namespace  Plugins;

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    user("admin"),
    password("admin"),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QString qss = "min-width:40px;min-height:22px;";
    QPushButton *userBtn = new QPushButton;
    userBtn->setIcon(QIcon(":/images/images/user.png"));
    userBtn->resize(30, ui->loginLineEdit->height());
    userBtn->setFixedWidth(30);
    userBtn->setStyleSheet(qss);
    QHBoxLayout *loginLayout = new QHBoxLayout(ui->loginLineEdit);
    loginLayout->setMargin(0);
    loginLayout->addStretch();
    loginLayout->addWidget(userBtn);

    QPushButton *cypherBtn = new QPushButton;
    cypherBtn->setIcon(QIcon(":/images/images/login.png"));
    cypherBtn->resize(30, ui->cypherLineEdit->height());
    cypherBtn->setFixedWidth(30);
    cypherBtn->setStyleSheet(qss);
    QHBoxLayout *cypherLayout = new QHBoxLayout(ui->cypherLineEdit);
    cypherLayout->setMargin(0);
    cypherLayout->addStretch();
    cypherLayout->addWidget(cypherBtn);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void Plugins::LoginDialog::on_loginPushButton_clicked()
{
    if(ui->loginLineEdit->text() == user && ui->cypherLineEdit->text() == password)
    {
        if(ui->rememberCheckBox->isChecked())
        {
            QString appDir = QCoreApplication::applicationFilePath();
            appDir = appDir.left(appDir.lastIndexOf("/"));
            QString iniConfig = appDir + "/config/taskcmt.ini";
            qDebug() <<"iniConfig: " << iniConfig;
            QFile iniFile(iniConfig);
            if (iniFile.exists()) {
                 QSettings iniWrite(iniConfig, QSettings::IniFormat);
                 iniWrite.setValue("/task/auto", 1);
                 iniWrite.setValue("/task/user", ui->loginLineEdit->text());
                 iniWrite.setValue("/task/password", ui->cypherLineEdit->text());
            } else {
                qDebug() << "failed to read task.ini";
            }
        }
        QString mainAddr, userAddr, userName, hashKey;
        mainAddr = Config->GetMainServerName();
        userAddr = Config->GetUserServerName();
        userName = Config->GetUserName();
        hashKey = Config->GetHashKey();
        ///qDebug() << mainAddr << userAddr << userName << hashKey;
        if(1 == TaskCommitMgr::loginServer(mainAddr, userAddr, userName, hashKey))
        {
            QMessageBox::warning(this, tr("prompt"), tr("login succeed"));
        } else {
            QMessageBox::warning(this, tr("prompt"), tr("server login failed, please contact FaseCAE"));
        }
        this->close();
    } else {
        QMessageBox::warning(this, tr("prompt"), tr("user&password was inputed error, please repeat input"));
    }

}
