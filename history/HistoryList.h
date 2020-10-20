#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#include <QWidget>
#include <QCloseEvent>
#include "HistoryListDelegate.cpp"
#include <utils/DatabaseUtils.h>
//#include <QMenu.h>
#include <qmenu.h>
//#include <QAction.h>
#include <qaction.h>
//#include <QSqlQueryModel.h>
#include <qsqlquerymodel.h>

namespace Ui {
class HistoryList;
}

class HistoryList : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryList(QWidget *parent = nullptr);
    ~HistoryList();
    void freshData();

signals:
    void backToMain();

private slots:
    void on_listView_customContextMenuRequested(const QPoint &pos);
    void mEditActionSlot();
    void mDelActionSlot();
    void mFreshActionSlot();

private:
    Ui::HistoryList *ui;
    DataResult getHistoryByTime();
};

#endif // HISTORYLIST_H
