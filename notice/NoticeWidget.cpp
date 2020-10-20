#include "NoticeWidget.h"
#include "ui_NoticeWidget.h"

NoticeWidget::NoticeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoticeWidget)
{
    ui->setupUi(this);
}

NoticeWidget::~NoticeWidget()
{
    delete ui;
}

void NoticeWidget::on_commit_clicked()
{
      this->close();
}

void NoticeWidget::notice(QString noticeStr)
{
    this->ui->title_lab->setText(noticeStr);
}

void NoticeWidget::successNotice(QString noticeStr)
{
    this->notice(noticeStr);
    this->ui->title_lab->setStyleSheet("color:green;");
}

void NoticeWidget::errorNotice(QString noticeStr)
{
    this->notice(noticeStr);
    this->ui->title_lab->setStyleSheet("color:red;");
}
