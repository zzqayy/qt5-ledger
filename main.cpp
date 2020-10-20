#include "login/LoginMain.h"
#include "login/LoginController.h"

#include <QApplication>
#include <main/MainWidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginController::initLogin();
    //将页面塞入
    MainWidget *mainWidget = new MainWidget;
    mainWidget->initWindows();
//    mainWidget.hide();
    return a.exec();
}
