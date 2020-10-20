#include <QDate>
#include <index/IndexMain.h>
#include <utils/CommonUtils.h>
#include <utils/DatabaseUtils.h>
#include "LoginMain.h"
#include "ui_LoginMain.h"
#include "LoginController.h"
#include "QDebug"

LoginMain::LoginMain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginMain)
{
    ui->setupUi(this);
}

void LoginMain::loginCheck()
{
    //查询系统中有没有存用户
    int remeberStatus = 0;
    DataResult rememberMeResult = DatabaseUtils::execSql("SELECT type_value FROM ledger_system_cache WHERE type='remeberMe';");
    QSqlQuery *remeberMeQuery = rememberMeResult.query;
    if(remeberMeQuery->first())
    {
         remeberStatus = remeberMeQuery->value(0).toInt();
    }
    //查询用户名
    QString username = "";
     DataResult usernameResult = DatabaseUtils::execSql("SELECT type_value FROM ledger_system_cache WHERE type='username';");
     QSqlQuery *usernameQuery = usernameResult.query;
     if(usernameQuery->seek(0))
     {
          username = usernameQuery->value(0).toString();
     }
    //查询密码
    QString password = "";
    DataResult passwordResult = DatabaseUtils::execSql("SELECT type_value FROM ledger_system_cache WHERE type='password';");
    QSqlQuery *passwordQuery = passwordResult.query;
    if(passwordQuery->seek(0))
    {
         password = passwordQuery->value(0).toString();
    }
    this->ui->username_input->setText(username);
    this->ui->password_input->setText(password);
    ui->password_input->setEchoMode(QLineEdit::Password);
    //自动登录
    int autoLogin = 0;
    DataResult autoLoginResult = DatabaseUtils::execSql("SELECT type_value FROM ledger_system_cache WHERE type='remeberMe';");
    QSqlQuery *autoLoginQuery = autoLoginResult.query;
    if(autoLoginQuery->first())
    {
         autoLogin = autoLoginQuery->value(0).toInt();
    }
    if(autoLogin == 1)
    {
       this->nextMain(true);
    }
}

LoginMain::~LoginMain()
{
    delete ui;
}

int LoginMain::getLoginStatus()
{
    return this->loginStatus;
}

void LoginMain::on_commitButton_clicked()
{
    this->nextMain(false);
}

void LoginMain::nextMain(bool autoLoginStatus)
{
    setMainStatus(false);
    QString username = this->ui->username_input->text();
    QString passwod = this->ui->password_input->text();
    if(username == "" || passwod == "")
    {
        setMainStatus(true);
        return;
    }
//    if (!autoLoginStatus)
//    {
//        passwod = PasswordUtils::getPassword(passwod);
//    }
    //获取记录信息存储
    bool remeberMeCheckStaus = this->ui->remeberMe->isChecked();
    if(remeberMeCheckStaus)
    {
        DatabaseUtils::execSql("update ledger_system_cache set type_value=1 where type='remeberMe';");
        QString usernameSql = QString("update ledger_system_cache set type_value='").append(username).append("' where type='username';");
        DatabaseUtils::execSql(usernameSql);
        QString passwordSql = QString("update ledger_system_cache set type_value='").append(passwod).append("' where type='password';");
        DatabaseUtils::execSql(passwordSql);
    }
    else
    {
        DatabaseUtils::execSql("update ledger_system_cache set type_value=0 where type='remeberMe';");
    }
    //获取自动登录信息存储
    bool autoLoginStaus = this->ui->autoLogin->isChecked();
    if(autoLoginStaus)
    {
        DatabaseUtils::execSql("update ledger_system_cache set type_value=1 where type='autoLogin';");
    }
    else
    {
        DatabaseUtils::execSql("update ledger_system_cache set type_value=0 where type='autoLogin';");
    }
    LoginController *loginController = new LoginController();
    int loginStatus = loginController->login(username, passwod);
    if (LoginController::LOGIN_ERROR_CODE == loginStatus)
    {
        this->ui->warning_lab->setStyleSheet("color:red;");
        this->ui->warning_lab->setText(QString("用户名或密码错误!"));
    }else if (LoginController::LOGIN_SUCCESS_CODE == loginStatus) {
        this->ui->warning_lab->setStyleSheet("color:green;");
        this->ui->warning_lab->setText(QString("登录成功!"));
        success();
    }
    setMainStatus(true);
}

void LoginMain::success()
{
    this->loginStatus = 1;
    CommonUtils::delayMs(1000);
    emit loginSuccess();
}

void LoginMain::setMainStatus(bool status)
{
    this->ui->username_input->setEnabled(status);
    this->ui->password_input->setEnabled(status);
    this->ui->commitButton->setEnabled(status);
    this->ui->autoLogin->setEnabled(status);
    this->ui->remeberMe->setEnabled(status);
}
