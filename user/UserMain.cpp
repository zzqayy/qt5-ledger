#include "UserMain.h"
#include "ui_UserMain.h"

#include <utils/CommonUtils.h>
#include <utils/DatabaseUtils.h>
#include <utils/PasswordUtils.h>

UserMain::UserMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMain)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
}

UserMain::~UserMain()
{
    delete ui;
}

void UserMain::freshData()
{
    QString sqlStr = "SELECT username,password FROM ledger_user WHERE uid=1;";
    DataResult result = DatabaseUtils::execSql(sqlStr);
    QSqlQuery* query = result.query;
    if(query->seek(0))
    {
        QString username = query->value(0).toString();
        this->ui->username->setText(username);
    }
}

void UserMain::on_pushButton_clicked()
{
    QString username = this->ui->username->text();
    QString password = this->ui->password->text();
    if(username == NULL || username.trimmed() == "")
    {
        this->ui->notice->setText("用户名必填");
        this->ui->notice->setStyleSheet("color: red;");
        return;
    }
    QString sqlStr = QString("UPDATE ledger_user SET username='")
            .append(username)
            .append("'");
    if(password != NULL || password.trimmed() != "")
    {
        if(password.size() < 6)
        {
            this->ui->notice->setText("密码需要设置6位以上");
            this->ui->notice->setStyleSheet("color: red;");
            return;
        }
        else
        {
            QString passwordNew = PasswordUtils::getPassword(password);
            sqlStr.append(" ,password='")
                    .append(passwordNew)
                    .append("' WHERE uid=1;");
        }
    }
    DataResult result = DatabaseUtils::execSql(sqlStr);
    if(result.status)
    {
        this->ui->notice->setText("设置成功");
        this->ui->notice->setStyleSheet("color: green;");
    }
    else
    {
        this->ui->notice->setText("设置失败");
        this->ui->notice->setStyleSheet("color: red;");
    }
    CommonUtils::delayMs(3000);
    this->ui->notice->setText("");
}
