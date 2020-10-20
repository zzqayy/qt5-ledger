//
// Created by zzq on 2020/9/12.
//

#ifndef SMALL_LEDGER_MAINWIDGET_H
#define SMALL_LEDGER_MAINWIDGET_H

#include <login/LoginMain.h>
#include <index/IndexMain.h>
#include <QtWidgets/QStackedLayout>
#include <history/HistoryList.h>

class MainWidget: public QObject
{
    Q_OBJECT

public:
    explicit MainWidget(QObject *parent = nullptr);
    void initWindows();
    ~MainWidget();

private slots:
    void switchView();
    void toLogin(int loginIndex);

private:
    LoginMain *loginMain;
    IndexMain *indexMain;
    QStackedLayout *stackedLayout;
    QVBoxLayout *mainLayout;
};

#endif //SMALL_LEDGER_MAINWIDGET_H
