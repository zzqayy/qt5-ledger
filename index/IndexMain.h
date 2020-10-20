#ifndef INDEXMAIN_H
#define INDEXMAIN_H

#include <QMainWindow>
#include <QDebug>
#include <index/MainForm.h>
#include <history/HistoryList.h>
#include <type/TypeListView.h>
#include <user/UserMain.h>

namespace Ui {
    class IndexMain;
}

class IndexMain : public QMainWindow
{
    Q_OBJECT


public:
    explicit IndexMain(QWidget *parent = nullptr);
    ~IndexMain();
    void startView();
    bool getInitFlag();

signals:
    void loginIndex(int num);
    void swithHistory(int num);

private slots:

    void on_actionrefresh_triggered();

    void on_close_triggered();

    void on_actionManager_triggered();

    void on_actionshouye_triggered();

    void on_typeManager_triggered();

    void on_actionuser_triggered();

    void on_action_help_triggered();

private:
    Ui::IndexMain *ui;
    MainForm *mainForm;
    HistoryList *historyList;
    TypeListView *typeListView;
    UserMain *userMain;
    bool initFlag = true;
};

#endif // INDEXMAIN_H
