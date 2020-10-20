#include "AddHistory.h"
#include "HistoryList.h"
#include "ui_HistoryList.h"

HistoryList::HistoryList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryList)
{
    ui->setupUi(this);
}

HistoryList::~HistoryList()
{
    delete ui;
}

/**
 * @brief HistoryList::start 启动查询数据
 */
void HistoryList::freshData()
{
    QString sqlStr = QString("SELECT lh.id,lh.money_status AS moneyStatus, lh.`money` AS money, lh.record_time AS recordTime, lt.type_name AS typeName, lh.remark AS remark"
                " FROM ledger_history lh"
                " INNER JOIN ledger_type lt ON lh.type_id=lt.id ORDER BY lh.id DESC;");
    QSqlQueryModel *queryModel = new QSqlQueryModel;
    queryModel->setQuery(sqlStr);
    this->ui->listView->setModel(queryModel);
    this->ui->listView->setItemDelegate(new HistoryListDelegate);
    this->ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void HistoryList::on_listView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *menu = new QMenu(this);
    QAction *mDel;
    QAction *mEdit;
    QAction *mFresh;

    mEdit = new QAction("编辑记录", this);
    mDel = new QAction("删除记录", this);
    mFresh = new QAction("刷新数据", this);

    menu->addAction(mFresh);
    menu->addAction(mEdit);
    menu->addAction(mDel);

    connect(mEdit, &QAction::triggered, this, &HistoryList::mEditActionSlot);
    connect(mDel, &QAction::triggered, this, &HistoryList::mDelActionSlot);
    connect(mFresh, &QAction::triggered, this, &HistoryList::mFreshActionSlot);
    menu->exec(QCursor::pos());
}

void HistoryList::mDelActionSlot()
{
    QAbstractItemModel *models = this->ui->listView->model();
    QItemSelectionModel *selectModel = this->ui->listView->selectionModel();
    QModelIndexList selectedIndexes = selectModel->selectedIndexes();
    foreach (QModelIndex modelIndex, selectedIndexes)
    {
        int historyId = models->data(modelIndex).toInt();
        QString delSql = "delete from ledger_history where id=:historyId";
        QMap<QString, QVariant> paramsMap;
        paramsMap.insert(":historyId", historyId);
        DatabaseUtils::exec(delSql, paramsMap, true);
        freshData();
    }
}

void HistoryList::mEditActionSlot()
{
    QAbstractItemModel *models = this->ui->listView->model();
    QItemSelectionModel *selectModel = this->ui->listView->selectionModel();
    QModelIndexList selectedIndexes = selectModel->selectedIndexes();
    foreach (QModelIndex modelIndex, selectedIndexes)
    {
        int historyId = models->data(modelIndex).toInt();
        AddHistory *addHistory = new AddHistory(nullptr, historyId);
        connect(addHistory, &AddHistory::refreshData, this, &HistoryList::freshData);
        addHistory->show();
    }
}

void HistoryList::mFreshActionSlot()
{
    this->freshData();
}
