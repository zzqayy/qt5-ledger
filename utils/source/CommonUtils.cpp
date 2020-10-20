#include <utils/CommonUtils.h>

#include <QCoreApplication>
#include <QDate>
#include <QEventLoop>
#include <QtMath>

void CommonUtils::delayMs(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

QString CommonUtils::getMoneyStr(int money)
{
   QString yuanStr = QString::number(qCeil(money/100));
   QString fenStr = QString::number(qAbs(money%100));
   return yuanStr.append(".").append(fenStr);
}

QString CommonUtils::getMoneyYuanStr(int money)
{
   return QString::number(qCeil(money/100));
}

QString CommonUtils::getMoneyPointStr(int money)
{
   return QString::number(qAbs(money%100));
}
