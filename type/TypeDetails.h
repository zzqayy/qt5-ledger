#ifndef TYPEDETAILS_H
#define TYPEDETAILS_H

#include <QWidget>
#include <utils/DatabaseUtils.h>

namespace Ui {
class TypeDetails;
}

class TypeDetails : public QWidget
{
    Q_OBJECT

public:
    explicit TypeDetails(QWidget *parent = nullptr);
    ~TypeDetails();
    void start(int typeId = 0);

signals:
    void freshParent();

private slots:
    void on_commitButton_clicked();

private:
    Ui::TypeDetails *ui;
    int currentTypeId;
};

#endif // TYPEDETAILS_H
