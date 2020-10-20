#ifndef NOTICEWIDGET_H
#define NOTICEWIDGET_H

#include <QWidget>

namespace Ui {
class NoticeWidget;
}

class NoticeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NoticeWidget(QWidget *parent = nullptr);
    ~NoticeWidget();
    /**
     * @brief notice
     * @param noticeStr
     */
    void notice(QString noticeStr);
    /**
     * @brief successNotice 成功的提示
     * @param noticeStr
     */
    void successNotice(QString noticeStr);
    /**
     * @brief errorNotice 失败的提示
     * @param noticeStr
     */
    void errorNotice(QString noticeStr);

private slots:
    void on_commit_clicked();

private:
    Ui::NoticeWidget *ui;
};

#endif // NOTICEWIDGET_H
