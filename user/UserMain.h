#ifndef USERMAIN_H
#define USERMAIN_H

#include <QWidget>

namespace Ui {
class UserMain;
}

class UserMain : public QWidget
{
    Q_OBJECT

public:
    explicit UserMain(QWidget *parent = nullptr);
    ~UserMain();
    void freshData();

private slots:
    void on_pushButton_clicked();

private:
    Ui::UserMain *ui;
};

#endif // USERMAIN_H
