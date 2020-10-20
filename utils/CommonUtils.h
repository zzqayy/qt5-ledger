#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QObject>

class CommonUtils : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief delayMs 延时
     * @param msec 毫秒
     */
    static void delayMs(int msec);
    /**
     * @brief getMoneyStr 将数据库的钱转换成带浮点型
     * @param money
     * @return
     */
    static QString getMoneyStr(int money);
    /**
     * @brief getMoneyYuanStr 将数据库的钱转换成浮点型的整数部分
     * @param money
     * @return
     */
    static QString getMoneyYuanStr(int money);
    /**
     * @brief getMoneyPointStr 将数据库的钱转换成浮点型的小数部分
     * @param money
     * @return
     */
    static QString getMoneyPointStr(int money);
};

#endif // COMMONUTILS_H
