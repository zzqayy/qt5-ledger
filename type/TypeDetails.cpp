#include "TypeDetails.h"
#include "ui_TypeDetails.h"

#include <QDate>
#include <QDebug>

#include <utils/CommonUtils.h>

TypeDetails::TypeDetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TypeDetails)
{
    ui->setupUi(this);
}

TypeDetails::~TypeDetails()
{
    delete ui;
}

void TypeDetails::start(int typeId)
{
    currentTypeId = typeId;
    if(typeId != 0)
    {
        QString sqlStr = "SELECT type_name FROM ledger_type WHERE id=";
        sqlStr.append(QString::number(typeId));
        DataResult dataResult = DatabaseUtils::execSql(sqlStr);
        QSqlQuery *query = dataResult.query;
        if(query->seek(0))
        {
            this->ui->typeNameLineEdit->setText(query->value(0).toString());
        }
    }
}

void TypeDetails::on_commitButton_clicked()
{
    QString typeNameStr = this->ui->typeNameLineEdit->text();
    if(typeNameStr == NULL || typeNameStr.trimmed() == "")
    {
        this->ui->notice->setText("必须填写类型");
        return;
    }
    QString sqlStr = "";
    if(currentTypeId == 0)
    {
        QDateTime now = QDateTime::currentDateTime();
        sqlStr.append("INSERT INTO ledger_type (type_name, create_time) VALUES ('")
                .append(typeNameStr)
                .append("','")
                .append(now.toString("yyyy-MM-dd HH:mm:ss"))
                .append("')");
    }
    else
    {
        sqlStr.append("UPDATE ledger_type SET type_name='")
                .append(typeNameStr)
                .append("' WHERE id=")
                .append(QString::number(currentTypeId));
    }
    DataResult result = DatabaseUtils::execSql(sqlStr);
    if(result.status)
    {
        this->ui->notice->setText("操作成功");
        this->ui->notice->setStyleSheet("color: green");
        CommonUtils::delayMs(500);
        emit freshParent();
        this->close();
    }
    else
    {
        this->ui->notice->setText("操作失败");
        this->ui->notice->setStyleSheet("color: red");
    }
}
