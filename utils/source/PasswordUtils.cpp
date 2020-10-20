#include "utils/PasswordUtils.h"

QString PasswordUtils::MD5_HASH_STR = "nvasduwr841859uiaf##fjdakfasa9013242)(";

PasswordUtils::PasswordUtils()
{

}

PasswordUtils::~PasswordUtils()
{
    delete this;
}

QString PasswordUtils::getPassword(QString oldPassword)
{
    if(oldPassword == NULL)
    {
        qDebug() << "password can not be null!";
        return "";
    }
    QByteArray passwordByteArray;
    for(int i = 0; i < 10; i++)
    {
        passwordByteArray.append(oldPassword).append(PasswordUtils::MD5_HASH_STR);
    }
    QByteArray hash = QCryptographicHash::hash(passwordByteArray, QCryptographicHash::Md5);
    QString password = hash.toHex();
    return password;
}
