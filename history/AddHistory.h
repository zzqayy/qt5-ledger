#ifndef ADDHISTORY_H
#define ADDHISTORY_H

#include <QWidget>
#include <QDebug>

namespace Ui {
    class AddHistory;
}

class AddHistory : public QWidget
{
    Q_OBJECT

public:
    explicit AddHistory(QWidget *parent = nullptr);
    ~AddHistory();
    explicit AddHistory(QWidget *parent = nullptr, int historyId = 0);
    /**
     * @brief initTypeSelect 初始化选择框
     */
    void initTypeSelect(int typeId = 0);

signals:
    void refreshData(int flag);

private slots:
    void on_commitBtn_clicked();

private:
    void initDate();
    Ui::AddHistory *ui;
    int historyId = 0;
    int refreshFlag;
};

#endif // ADDHISTORY_H
