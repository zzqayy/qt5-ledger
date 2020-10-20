#ifndef PASSWORDUTILS_H
#define PASSWORDUTILS_H

#include <QString>
#include <QDebug>
#include <QCryptographicHash>


class PasswordUtils
{
public:
    PasswordUtils();
    ~PasswordUtils();
    static QString getPassword(QString oldPassword);

private:
    static QString MD5_HASH_STR;
};

#endif // PASSWORDUTILS_H
