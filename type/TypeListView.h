#ifndef TYPELISTVIEW_H
#define TYPELISTVIEW_H

#include <QWidget>
#include <QSqlQueryModel>
#include "TypeListDelegate.cpp"
#include <QMenu>
#include <utils/DatabaseUtils.h>

namespace Ui {
class TypeListView;
}

class TypeListView : public QWidget
{
    Q_OBJECT

public:
    explicit TypeListView(QWidget *parent = nullptr);
    ~TypeListView();
    void freshData();

private slots:
    void on_typeListView_customContextMenuRequested(const QPoint &pos);
    void mFreshActionSlot();
    void mEditActionSlot();
    void mDelActionSlot();
    void mAddActionSlot();

private:
    Ui::TypeListView *ui;
};

#endif // TYPELISTVIEW_H
