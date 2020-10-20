//
// Created by zzq on 2020/9/12.
//

#ifndef SMALL_LEDGER_LOGINCONTROLLER_H
#define SMALL_LEDGER_LOGINCONTROLLER_H

#include <QObject>
#include <qmainwindow.h>
#include "../utils/DatabaseUtils.h"
#include "../utils/PasswordUtils.h"

class LoginController : public QObject
{
Q_OBJECT
public:
    LoginController();
    ~LoginController();
    int login(QString username, QString password);
    //登录成功
    static int LOGIN_SUCCESS_CODE;
    //登录失败
    static int LOGIN_ERROR_CODE;
    //初始化页面
    static int initLogin();
};

#endif //SMALL_LEDGER_LOGINCONTROLLER_H
