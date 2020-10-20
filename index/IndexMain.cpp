#include "IndexMain.h"
#include "ui_IndexMain.h"

#include <help/HelpView.h>

IndexMain::IndexMain(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::IndexMain)
{
    ui->setupUi(this);
}

IndexMain::~IndexMain()
{
    delete ui;
}

/**
 * @brief IndexMain::startView 启动视图
 */
void IndexMain::startView()
{
    if(initFlag)
    {
        initFlag = false;

        mainForm = new MainForm;
        historyList = new HistoryList;
        typeListView = new TypeListView;
        userMain = new UserMain;

        this->ui->mainStackedWidget->addWidget(mainForm);
        this->ui->mainStackedWidget->addWidget(historyList);
        this->ui->mainStackedWidget->addWidget(typeListView);
        this->ui->mainStackedWidget->addWidget(userMain);
    }
    mainForm->refreshData();
}
//刷新按钮
void IndexMain::on_actionrefresh_triggered()
{
    int currentIndex = this->ui->mainStackedWidget->currentIndex();
    if(currentIndex == 0)
    {
        this->mainForm->refreshData();
    }
    else if (currentIndex == 1)
    {
        this->historyList->freshData();
    }

}
//退出登录
void IndexMain::on_close_triggered()
{
    emit loginIndex(1);
}
//跳转历史
void IndexMain::on_actionManager_triggered()
{
    this->ui->mainStackedWidget->setCurrentWidget(historyList);
    this->historyList->freshData();
}
//跳转首页
void IndexMain::on_actionshouye_triggered()
{
    this->ui->mainStackedWidget->setCurrentWidget(mainForm);
    this->mainForm->refreshData();
}

void IndexMain::on_typeManager_triggered()
{
    this->ui->mainStackedWidget->setCurrentWidget(typeListView);
    typeListView->freshData();
}

void IndexMain::on_actionuser_triggered()
{
    this->ui->mainStackedWidget->setCurrentWidget(userMain);
    userMain->freshData();
}

void IndexMain::on_action_help_triggered()
{
    HelpView* helpView = new HelpView;
    helpView->show();
}
