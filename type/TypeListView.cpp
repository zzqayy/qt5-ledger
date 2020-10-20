#include "TypeDetails.h"
#include "TypeListView.h"
#include "ui_TypeListView.h"
#include <QDebug>

TypeListView::TypeListView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TypeListView)
{
    ui->setupUi(this);
}

TypeListView::~TypeListView()
{
    delete ui;
}

void TypeListView::freshData()
{
    QString sqlStr = QString("SELECT id,type_name,create_time"
                " FROM ledger_type"
                " ORDER BY id DESC;");
    QSqlQueryModel *queryModel = new QSqlQueryModel;
    queryModel->setQuery(sqlStr);
    this->ui->typeListView->setModel(queryModel);
    this->ui->typeListView->setItemDelegate(new TypeListDelegate);
    this->ui->typeListView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void TypeListView::on_typeListView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *menu = new QMenu(this);
    QAction *mDel;
    QAction *mEdit;
    QAction *mFresh;
    QAction *mAdd;

    mAdd = new QAction("添加类型", this);
    mEdit = new QAction("编辑类型", this);
    mDel = new QAction("删除类型", this);
    mFresh = new QAction("刷新数据", this);

    menu->addAction(mAdd);
    menu->addAction(mEdit);
    menu->addAction(mDel);
    menu->addAction(mFresh);

    connect(mAdd, &QAction::triggered, this, &TypeListView::mAddActionSlot);
    connect(mEdit, &QAction::triggered, this, &TypeListView::mEditActionSlot);
    connect(mDel, &QAction::triggered, this, &TypeListView::mDelActionSlot);
    connect(mFresh, &QAction::triggered, this, &TypeListView::mFreshActionSlot);
    menu->exec(QCursor::pos());
}

void TypeListView::mDelActionSlot()
{
    QAbstractItemModel *models = this->ui->typeListView->model();
    QItemSelectionModel *selectModel = this->ui->typeListView->selectionModel();
    QModelIndexList selectedIndexes = selectModel->selectedIndexes();
    foreach (QModelIndex modelIndex, selectedIndexes)
    {
        int historyId = models->data(modelIndex).toInt();
        QString delSql = "delete from ledger_type where id=:historyId";
        QMap<QString, QVariant> paramsMap;
        paramsMap.insert(":historyId", historyId);
        DatabaseUtils::exec(delSql, paramsMap, true);
        freshData();
    }
}

void TypeListView::mEditActionSlot()
{
    QAbstractItemModel *models = this->ui->typeListView->model();
    QItemSelectionModel *selectModel = this->ui->typeListView->selectionModel();
    QModelIndexList selectedIndexes = selectModel->selectedIndexes();
    foreach (QModelIndex modelIndex, selectedIndexes)
    {
        int historyId = models->data(modelIndex).toInt();
        TypeDetails *typeDetails = new TypeDetails;
        typeDetails->start(historyId);
        connect(typeDetails, &TypeDetails::freshParent, this, &TypeListView::mFreshActionSlot);
        typeDetails->show();
    }
}

void TypeListView::mAddActionSlot()
{
    TypeDetails *typeDetails = new TypeDetails;
    typeDetails->start(0);
    connect(typeDetails, &TypeDetails::freshParent, this, &TypeListView::mFreshActionSlot);
    typeDetails->show();
}

void TypeListView::mFreshActionSlot()
{
    this->freshData();
}
