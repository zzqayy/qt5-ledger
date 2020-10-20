#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <history/AddHistory.h>
#include <history/ListViewDelegate.cpp>
#include <qchartview.h>
#include <qchar.h>
//#include <QChartView>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QSqlQueryModel>
#include <QStandardItem>
#include <QValueAxis>
#include <utils/DatabaseUtils.h>
#include <QVariant>
#include <QDate>
#include <QMenu>
#include <QPieSeries>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE
namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();
    void refreshData();

signals:
    void swithHistory(int num);

private slots:
    void on_addHistory_clicked();

    void on_historyListView_customContextMenuRequested(const QPoint &pos);

    void mDelActionSlot();

    void mEditActionSlot();

    void refreshDataSlot();


private:
    Ui::MainForm *ui;
    //通过起始时间获取历史列表
    DataResult getHistoryByTime(QString startTime, QString endTime, int moneyStatus);
    void initHistoryList();
    void initTotalCount();
    void initLineChart();
    void initAllTypeChart();
    void initMonthTypeChart();
    /**
     * @brief getChartByMoneyStatus 通过收入,支出获取曲线
     * @param status
     * @return
     */
    QChart* getChartByMoneyStatus(int status);
};

#endif // MAINFORM_H
