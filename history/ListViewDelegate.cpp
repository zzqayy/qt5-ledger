#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <utils/CommonUtils.h>

class ListViewDelegate : public QStyledItemDelegate
{
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        int totalCountIndex = 5;

        QString historyId = index.model()->data(index.model()->index(index.row(), 0)).toString();
        int moneyStatus = index.model()->data(index.model()->index(index.row(), 1)).toInt();
        int moneyTotal = index.model()->data(index.model()->index(index.row(), 2)).toInt();
        QString money = CommonUtils::getMoneyStr(moneyTotal);
        QString recordTime = index.model()->data(index.model()->index(index.row(), 3)).toString();
         QString typeName = index.model()->data(index.model()->index(index.row(), 4)).toString();

         QString moneyStatusStr = "支出";
         if(moneyStatus == 2)
         {
            moneyStatusStr = "收入";
         }

        // draw correct background
        opt.text = "";
        QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

        QRect rect = opt.rect;
        QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        // set pen color
        if (opt.state & QStyle::State_Selected)
        {
            painter->setPen(opt.palette.color(cg, QPalette::PlaceholderText));
        }
        else
            painter->setPen(opt.palette.color(cg, QPalette::Text));

        // draw 2 lines of text
        painter->drawText(QRect(rect.left(), rect.top(), rect.width()/totalCountIndex, rect.height()),
                          opt.displayAlignment, historyId);
        painter->drawText(QRect(rect.left() + (rect.width()/totalCountIndex) * 1, rect.top(), (rect.width()/totalCountIndex) * 1, rect.height()),
                          opt.displayAlignment, money);
        painter->drawText(QRect(rect.left() + (rect.width()/totalCountIndex) * 2, rect.top(), (rect.width()/totalCountIndex) * 1, rect.height()),
                          opt.displayAlignment, moneyStatusStr);
        painter->drawText(QRect(rect.left() + (rect.width()/totalCountIndex) * 3, rect.top(), (rect.width()/totalCountIndex) * 1, rect.height()),
                          opt.displayAlignment, typeName);
        painter->drawText(QRect(rect.left() + (rect.width()/totalCountIndex) * 4, rect.top(), (rect.width()/totalCountIndex) * 1, rect.height()),
                          opt.displayAlignment, recordTime);
    }

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
        QSize result = QStyledItemDelegate::sizeHint(option, index);
        result.setHeight(result.height());
        result.setWidth(result.width());
        return result;
    }
};
