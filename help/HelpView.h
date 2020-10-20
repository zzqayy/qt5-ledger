#ifndef HELPVIEW_H
#define HELPVIEW_H

#include <QWidget>

namespace Ui {
class HelpView;
}

class HelpView : public QWidget
{
    Q_OBJECT

public:
    explicit HelpView(QWidget *parent = nullptr);
    ~HelpView();

private:
    Ui::HelpView *ui;
};

#endif // HELPVIEW_H
