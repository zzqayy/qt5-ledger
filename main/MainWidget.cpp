//
// Created by zzq on 2020/9/12.
//

#include "MainWidget.h"

MainWidget::MainWidget(QObject *parent)
{

}

MainWidget::~MainWidget()
{
    delete this;
}

void MainWidget::initWindows()
{
    loginMain = new LoginMain;
    indexMain = new IndexMain;
    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(loginMain);
    stackedLayout->addWidget(indexMain);
    connect(loginMain, &LoginMain::loginSuccess, this, &MainWidget::switchView);
    connect(indexMain, &IndexMain::loginIndex, this, &MainWidget::toLogin);
    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(stackedLayout);
    loginMain->loginCheck();
}

void MainWidget::switchView()
{
    LoginMain *loginMain = qobject_cast<LoginMain *>(sender());
    int loginStatus = loginMain->getLoginStatus();
    if(loginStatus == 1)
    {
        stackedLayout->setCurrentIndex(1);
        this->indexMain->startView();
    }
}

void MainWidget::toLogin(int loginIndex)
{
    if(loginIndex == 1)
    {
        stackedLayout->setCurrentIndex(0);
    }
}



