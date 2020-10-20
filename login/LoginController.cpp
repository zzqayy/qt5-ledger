//
// Created by zzq on 2020/9/12.
//

#include "LoginController.h"
#include <QDebug>
#include <utils/PasswordUtils.h>


int LoginController::LOGIN_SUCCESS_CODE = 1;
int LoginController::LOGIN_ERROR_CODE = -1;

LoginController::LoginController()
{

}

LoginController::~LoginController()
{
    delete this;
}

int LoginController::login(QString username, QString password)
{
    if(username.isNull() || username.isEmpty())
    {
        return LOGIN_ERROR_CODE;
    }
    password = PasswordUtils::getPassword(password);
    QString sqlStr = QString("SELECT COUNT(uid) FROM ledger_user WHERE username=:username and password=:password");
    QSqlDatabase dataBase = DatabaseUtils::getDatabase();
    QSqlQuery *query = new QSqlQuery(dataBase);
    query->prepare(sqlStr);
    query->bindValue(":username", username);
    query->bindValue(":password", password);
    query->exec();
    if(query->first())
    {
        int passwordResult = query->value(0).toInt();
        if(passwordResult == 0)
        {
            return LOGIN_ERROR_CODE;
        }
    }else {
        return LOGIN_ERROR_CODE;
    }
    return LOGIN_SUCCESS_CODE;
}

/**
 * @brief LoginController::initWindow 初始化页面
 */
int LoginController::initLogin()
{
    //初始化sql
    DatabaseUtils::initDatabase();
    return 1;
}
