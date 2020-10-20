#include "AddHistory.h"
#include "ui_AddHistory.h"

#include <utils/CommonUtils.h>
#include <utils/DatabaseUtils.h>

#include <notice/NoticeWidget.h>

#include <QTextBlock>

#include <index/IndexMain.h>

AddHistory::AddHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddHistory)
{
    ui->setupUi(this);
    AddHistory::initTypeSelect();
    AddHistory::initDate();
}

AddHistory::AddHistory(QWidget *parent, int historyId) :
    QWidget(parent),
    ui(new Ui::AddHistory)
{
    ui->setupUi(this);
    int typeId = 0;
    if(historyId == 0)
    {
        AddHistory::initTypeSelect();
        AddHistory::initDate();
        return;
    }
    QString sqlStr = QString("SELECT id,type_id, money_status, money, record_time, create_time, remark FROM ledger_history WHERE id=").append(QString::number(historyId));
    DataResult history = DatabaseUtils::execSql(sqlStr);
    QSqlQuery *query = history.query;
    if(!query->seek(0)){
        AddHistory::initTypeSelect();
        AddHistory::initDate();
        return;
    }
    //设置id
    this->historyId = query->value(0).toInt();

    //设置类型
    typeId = query->value(1).toInt();
    this->initTypeSelect(typeId);
    //设置支出方式
    int moneyStatus = query->value(2).toInt();
    if(moneyStatus == 1)
    {
        this->ui->recordIn->setChecked(false);
        this->ui->recordOut->setChecked(true);
    }
    else if (moneyStatus == 2)
    {
        this->ui->recordIn->setChecked(true);
        this->ui->recordOut->setChecked(false);
    }
    //设置钱
    int money = query->value(3).toInt();
    QString yuanStr = CommonUtils::getMoneyYuanStr(money);
    this->ui->money_yuan_input->setText(yuanStr);
    QString pointStr = CommonUtils::getMoneyPointStr(money);
    this->ui->money_input->setText(pointStr);

    //设置记录时间
    QDate recordTime = query->value(4).toDate();
    this->ui->moneyCalendar->setSelectedDate(recordTime);

    //备注
    QString remark = query->value(6).toString();
    this->ui->remarkTextEdit->setPlainText(remark);

}

AddHistory::~AddHistory()
{
    this->historyId = 0;
    delete ui;
}

void AddHistory::initTypeSelect(int typeId)
{
    QString sqlStr = "SELECT id,type_name FROM ledger_type";
    DataResult dataReslut = DatabaseUtils::execSql(sqlStr);
    QSqlQuery *query = dataReslut.query;
    this->ui->type_select->clear();
    int currentIndex = 0;
    int selectedIndex = 0;
    while (query->next())
    {
        int currentTypeId = query->value(0).toInt();
        QString typeName = query->value(1).toString();
        QVariant typeQVariant;
        typeQVariant.setValue(currentTypeId);
        this->ui->type_select->addItem(typeName, typeQVariant);
        if(currentTypeId == typeId)
        {
            selectedIndex = currentIndex;
        }
        currentIndex++;
    }
    this->ui->type_select->setCurrentIndex(selectedIndex);
}

/**
 * @brief AddHistory::initDate 初始化时间
 */
void AddHistory::initDate()
{
    QDate now;
    this->ui->moneyCalendar->setSelectedDate(now);
}
//void AddHistory::

void AddHistory::on_commitBtn_clicked()
{
    //获取支出类型
    QVariant moneyStatus;
    //支出
    bool outChecked = this->ui->recordOut->isChecked();
    if(outChecked)
    {
        moneyStatus.setValue(1);
    }
    else
    {
        moneyStatus.setValue(2);
    }
    //获取钱
    QString moneyYuan = this->ui->money_yuan_input->text();
    QString moneyPoint = this->ui->money_input->text();
    int pointInt = moneyPoint.toInt();
    if(pointInt > 100 || pointInt < 0)
    {
        this->ui->returnMsg->setText("请在财富值填写不对");
        this->ui->returnMsg->setStyleSheet("color:red");
        return ;
    }
    if(pointInt < 10)
    {
        pointInt = pointInt*10;
    }
    moneyPoint = QString::number(pointInt);
    if(pointInt == 0)
    {
        moneyPoint = "00";
    }
    int moneyFloat = (moneyYuan.append(moneyPoint)).toInt();
    QVariant money;
    money.setValue(moneyFloat);
    if(moneyFloat == 0)
    {
        this->ui->returnMsg->setText("请在财富值上填写数字!");
        this->ui->returnMsg->setStyleSheet("color:red");
        return ;
    }
    //获取类型
    QVariant typeId = this->ui->type_select->currentData();
    //获取选择的时间
    QDate selectDate = this->ui->moneyCalendar->selectedDate();
    QString recordTime = selectDate.toString("yyyy-MM-dd");
    //获取备注
    QString remark = QString("");
    QTextBlock textBlock;
    QTextDocument *document = this->ui->remarkTextEdit->document();
    for (textBlock = document->begin(); textBlock != document->end(); textBlock = textBlock.next())
    {
        remark.append(textBlock.text());
    }
    QString insertSql = QString("");
    QMap<QString, QVariant> queryMap;
    if(this->historyId != 0)
    {
        insertSql.append("UPDATE ledger_history SET type_id=:typeId, money_status=:moneyStatus, money=:money, record_time=:recordTime, remark=:remark")
                .append(" WHERE id=").append(QString::number(this->historyId));
        queryMap.insert(":typeId", typeId);
        queryMap.insert(":moneyStatus", moneyStatus);
        queryMap.insert(":money", money);
        queryMap.insert(":recordTime", recordTime);
        queryMap.insert(":remark", remark);
    }
    else
    {
        //创建时间
        QDateTime now = QDateTime::currentDateTime();
        QString nowStr = now.toString("yyyy-MM-dd HH:mm:ss");
        insertSql.append("INSERT INTO ledger_history (type_id, money_status, money, record_time, create_time, remark) VALUES ")
                .append("(:typeId, :moneyStatus, :money, :recordTime, :createTime, :remark);");

        queryMap.insert(":typeId", typeId);
        queryMap.insert(":moneyStatus", moneyStatus);
        queryMap.insert(":money", money);
        queryMap.insert(":recordTime", recordTime);
        queryMap.insert(":createTime", nowStr);
        queryMap.insert(":remark", remark);
    }
    DataResult dataResult = DatabaseUtils::exec(insertSql, queryMap, true);
    if(dataResult.status)
    {
        this->ui->returnMsg->setText("成功");
        this->ui->returnMsg->setStyleSheet("color:green");
        //延时500毫秒
        CommonUtils::delayMs(500);
        emit refreshData(1);
        this->close();
    }
    else
    {
        this->ui->returnMsg->setText("失败!");
        this->ui->returnMsg->setStyleSheet("color:red");
    }
}
