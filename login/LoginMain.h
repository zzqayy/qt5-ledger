#ifndef LOGINMAIN_H
#define LOGINMAIN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginMain; }
QT_END_NAMESPACE

class LoginMain : public QWidget
{
    Q_OBJECT

public:
    LoginMain(QWidget *parent = nullptr);
    ~LoginMain();

    int getLoginStatus();
    void loginCheck();

signals:
    void loginSuccess();

private slots:
    void on_commitButton_clicked();

private:
    Ui::LoginMain *ui;
    void nextMain(bool autoLoginStatus);
    int loginStatus;
    void success();
    void setMainStatus(bool status);
    bool savePasswordStatus;
};
#endif // LOGINMAIN_H
