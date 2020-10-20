#include "MainForm.h"
#include "ui_MainForm.h"

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::on_addHistory_clicked()
{
    AddHistory *addHistory = new AddHistory(nullptr, 0);
    addHistory -> setWindowModality (Qt::ApplicationModal);
    connect(addHistory, &AddHistory::refreshData, this, &MainForm::refreshDataSlot);
    addHistory -> show();
}

void MainForm::refreshDataSlot()
{
    this->refreshData();
}

void MainForm::on_historyListView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *menu = new QMenu(this);
    QAction *mDel;
    QAction *mEdit;

    mEdit = new QAction("编辑记录", this);
    mDel = new QAction("删除记录", this);
    menu->addAction(mEdit);
    menu->addAction(mDel);
    connect(mEdit, &QAction::triggered, this, &MainForm::mEditActionSlot);
    connect(mDel, &QAction::triggered, this, &MainForm::mDelActionSlot);
    menu->exec(QCursor::pos());
}

void MainForm::mDelActionSlot()
{
    QAbstractItemModel *models = this->ui->historyListView->model();
    QItemSelectionModel *selectModel = this->ui->historyListView->selectionModel();
    QModelIndexList selectedIndexes = selectModel->selectedIndexes();
    foreach (QModelIndex modelIndex, selectedIndexes)
    {
        int historyId = models->data(modelIndex).toInt();
        QString delSql = "delete from ledger_history where id=:historyId";
        QMap<QString, QVariant> paramsMap;
        paramsMap.insert(":historyId", historyId);
        DatabaseUtils::exec(delSql, paramsMap, true);
        refreshData();
    }
}

void MainForm::mEditActionSlot()
{
    QAbstractItemModel *models = this->ui->historyListView->model();
    QItemSelectionModel *selectModel = this->ui->historyListView->selectionModel();
    QModelIndexList selectedIndexes = selectModel->selectedIndexes();
    foreach (QModelIndex modelIndex, selectedIndexes)
    {
        int historyId = models->data(modelIndex).toInt();
        qDebug() << historyId;
        AddHistory *addHistory = new AddHistory(nullptr, historyId);
        connect(addHistory, &AddHistory::refreshData, this, &MainForm::refreshDataSlot);
        addHistory->show();
    }
}

/**
 * @brief IndexMain::initHistoryList 历史列表
 */
void MainForm::initHistoryList()
{
    QDate now = QDate::currentDate();
    QString monthStr = now.toString("yyyy-MM");
    QString startTime = QString(monthStr).append("-01");
    QString endTime = now.addDays(1).toString("yyyy-MM-dd");
    QString sqlStr = QString("SELECT lh.id,lh.money_status AS moneyStatus, lh.`money` AS money, lh.record_time AS recordTime, lt.type_name AS typeName"
            " FROM ledger_history lh"
            " INNER JOIN ledger_type lt ON lh.type_id=lt.id"
            " WHERE lh.record_time>='")
            .append(startTime)
            .append("' AND lh.record_time<'")
            .append(endTime)
            .append("' ORDER BY lh.record_time DESC;");
    QSqlDatabase database = DatabaseUtils::getDatabase();
    QSqlQueryModel *queryModel = new QSqlQueryModel;
    queryModel->setQuery(sqlStr);
    this->ui->historyListView->setModel(queryModel);
    this->ui->historyListView->setItemDelegate(new ListViewDelegate);
    this->ui->historyListView->setContextMenuPolicy(Qt::CustomContextMenu);
}

/**
 * @brief IndexMain::initTotalCount 统计数据返现
 */
void MainForm::initTotalCount()
{
    DataResult totalResult = DatabaseUtils::execSql("SELECT IFNULL(SUM(CASE WHEN money_status=1"
                           " then (-1*`money`) else `money` END), 0) AS totalSum,"
                           " COUNT(id) AS totalCount FROM ledger_history;"
                           );
    QString totalMoneyStr;
    QString totalCountStr;
    QSqlQuery *totalQuery = totalResult.query;
    if(totalQuery->seek(0))
    {
        int totalMoney = totalQuery->value(0).toInt();
        totalMoneyStr = CommonUtils::getMoneyStr(totalMoney);
        totalCountStr = totalQuery->value(1).toString();
    }
    QSqlDatabase database = DatabaseUtils::getDatabase();
    QString sqlStr = "SELECT IFNULL(SUM(CASE WHEN money_status=1 then (-1*`money`) else `money` END), 0) AS todaySum,"
                     " COUNT(id) AS todayCount"
                     " FROM ledger_history"
                     " WHERE record_time=:todayDate";
    QSqlQuery *query = new QSqlQuery(database);
    query->prepare(sqlStr);
    QDate now = QDate::currentDate();
    QString todayDate = now.toString("yyyy-MM-dd");
    query->bindValue(":todayDate", todayDate);
    query->exec();

    QString todayMoneyStr;
    QString todayCountStr;
    if (query->seek(0)) {
        int todayMoney = query->value(0).toInt();
        todayMoneyStr = CommonUtils::getMoneyStr(todayMoney);
        todayCountStr = query->value(1).toString();
    }
    this->ui->totalMoney->setText(totalMoneyStr);
    this->ui->totalCount->setText(totalCountStr);
    this->ui->todayMoney->setText(todayMoneyStr);
    this->ui->todayCount->setText(todayCountStr);
}

/**
 * @brief IndexMain::initLineChart 绘制历史折线图
 */
void MainForm::initLineChart()
{
    QChart* inChart = this->getChartByMoneyStatus(2);
    this->ui->anyGraphicsView->setChart(inChart);
    this->ui->anyGraphicsView->setRenderHint(QPainter::Antialiasing);

    QChart *outChart = this->getChartByMoneyStatus(1);
    this->ui->consumeView->setChart(outChart);
    this->ui->consumeView->setRenderHint(QPainter::Antialiasing);
}

QChart* MainForm::getChartByMoneyStatus(int status)
{
    QLineSeries* series = new QLineSeries();
    if(status == 1)
    {
        series->setName("支出趋势");
    }
    else
    {
        series->setName("收入趋势");
    }
    QString formatStr = "yyyy-MM-dd";
    DataResult resultData = this->getHistoryByTime(NULL, NULL, status);
    QSqlQuery *query = resultData.query;
    int count = 0;
    while (query->next()) {
        count++;
        float moneyFloat = query->value(0).toFloat();
        QDateTime dateTime = query->value(1).toDateTime();
        qint64 dateInt = dateTime.toMSecsSinceEpoch();
        series->append(dateInt, moneyFloat);
    }

    QChart *chart = new QChart();
    series->setPointsVisible(true);
    series->setPointLabelsFormat("@yPoint");
    series->setPointLabelsVisible(true);
    chart->addSeries(series);
    chart->legend()->hide();

    QDateTimeAxis *axisX = new QDateTimeAxis;
    if(count > 5)
    {
        axisX->setTickCount(5);
    }
    else
    {
        axisX->setTickCount(count);
    }

    axisX->setFormat("yyyy/MM/dd");
    axisX->setGridLineVisible(true);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    return chart;
}

/**
 * @brief IndexMain::initAllTypeChart 全部消费统计
 */
void MainForm::initAllTypeChart()
{
    QString allCountSql = "SELECT round((IFNULL(SUM(money), 0))/100,2) FROM ledger_history WHERE money_status=1;";
    DataResult allCountResult = DatabaseUtils::execSql(allCountSql);
    QSqlQuery *allCountQuery = allCountResult.query;
    float allCount = 0;
    if(allCountQuery->seek(0))
    {
        allCount = allCountQuery->value(0).toFloat();
    }
    if(allCount == 0)
    {
        return;
    }
    QString querySql = "SELECT round((IFNULL(SUM(lh.money), 0))/100, 2) AS money, lt.type_name AS typeName FROM ledger_history lh"
            " left JOIN ledger_type lt ON lh.type_id=lt.id"
            " WHERE money_status=1"
            " GROUP BY lh.type_id;";
    DataResult allTypeResult = DatabaseUtils::execSql(querySql);
    QSqlQuery *query = allTypeResult.query;
    QPieSeries *pieSeries = new QPieSeries;
    while (query->next()) {
        float money = query->value(0).toFloat();
        float moneyPercent = (money/allCount)*100;
        QString moneyPercentStr = QString::number(moneyPercent,'f', 1).append("%");
        QString typeName = query->value(1).toString();
        QPieSlice *slice = new QPieSlice;
        slice->setLabel(typeName.append(":").append(moneyPercentStr));
        slice->setValue(money);
        slice->setLabelVisible(true);
        pieSeries->append(slice);
        //connect(slice, SIGNAL(clicked()), this, SLOT());
    }
    QChart *chart = new QChart;
    chart->addSeries(pieSeries);
    chart->legend()->setAlignment(Qt::AlignTop);
    this->ui->allTypeVieaw->setChart(chart);
}

/**
 * @brief IndexMain::initMonthTypeChart 月度消费统计
 */
void MainForm::initMonthTypeChart()
{
    QDate now = QDate::currentDate();
    QString monthStr = now.toString("yyyy-MM");
    QString startTime = QString(monthStr).append("-01");
    QString endTime = now.addDays(1).toString("yyyy-MM-dd");
    QString allCountSql = "SELECT round((IFNULL(SUM(money), 0))/100, 2) FROM ledger_history WHERE money_status=1  AND record_time>=:startTime AND record_time<:endTime;";
    QMap<QString, QVariant> paramsMap;
    paramsMap.insert(":startTime", startTime);
    paramsMap.insert(":endTime", endTime);
    DataResult allCountResult = DatabaseUtils::exec(allCountSql, paramsMap, false);
    QSqlQuery *allCountQuery = allCountResult.query;
    float allCount = 0;
    if(allCountQuery->seek(0))
    {
        allCount = allCountQuery->value(0).toFloat();
    }
    if(allCount == 0)
    {
        return;
    }
    QString querySql = "SELECT round((IFNULL(SUM(lh.money), 0))/100,2) AS money, lt.type_name AS typeName FROM ledger_history lh"
            " left JOIN ledger_type lt ON lh.type_id=lt.id"
            " WHERE money_status=1 AND record_time>=:startTime AND record_time<=:endTime"
            " GROUP BY lh.type_id;";
    DataResult allTypeResult = DatabaseUtils::exec(querySql, paramsMap, false);
    QSqlQuery *query = allTypeResult.query;
    QPieSeries *pieSeries = new QPieSeries;
    while (query->next()) {
        float money = query->value(0).toFloat();
        float moneyPercent = (money/allCount)*100;
        QString moneyPercentStr = QString::number(moneyPercent,'f', 1).append("%");
        QString typeName = query->value(1).toString();
        QPieSlice *slice = new QPieSlice;
        slice->setLabel(typeName.append(":").append(moneyPercentStr));
        slice->setValue(money);
        slice->setLabelVisible(true);
        pieSeries->append(slice);
    }
    QChart *chart = new QChart;
    chart->addSeries(pieSeries);
    chart->legend()->setAlignment(Qt::AlignTop);
    this->ui->monthTypeView->setChart(chart);
}

/**
 * @brief IndexMain::getHistoryByTime 查询数据
 * @param startTime
 * @param endTime
 * @param moneyStatus 2为收入,1为支出
 * @return
 */
DataResult MainForm::getHistoryByTime(QString startTime, QString endTime, int moneyStatus)
{
    QString sqlStr = QString("SELECT round((IFNULL(SUM(lh.`money`),0))/100, 2) AS money,")
            .append(" lh.record_time AS recordTime")
            .append(" FROM ledger_history lh")
            .append(" INNER JOIN ledger_type lt ON lh.type_id=lt.id WHERE 1=1");
    if(moneyStatus != 0)
    {
        sqlStr.append(" AND lh.money_status=").append(QString::number(moneyStatus));
    }
    if (startTime != NULL)
    {
        sqlStr.append(" AND lh.record_time>:startTime");
    }
    if (endTime != NULL)
    {
        sqlStr.append(" AND lh.record_time<:endTime");
    }
    sqlStr.append(" GROUP BY lh.record_time ORDER BY lh.record_time ASC");
    QSqlDatabase database = DatabaseUtils::getDatabase();
    QSqlQuery *query = new QSqlQuery(database);
    query->prepare(sqlStr);
    if (startTime != NULL)
    {
        query->bindValue(":startTime", startTime);
    }
    if (endTime != NULL)
    {
        query->bindValue(":endTime", endTime);
    }
    query->exec();
    return {
        true,
        query
    };
}


/**
 * @brief IndexMain::refreshData 刷新数据
 */
void MainForm::refreshData()
{
    this->initTotalCount();
    this->initLineChart();
    this->initHistoryList();
    this->initAllTypeChart();
    this->initMonthTypeChart();
}

