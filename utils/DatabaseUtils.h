#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QObject>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QMap>

struct DataResult{
    bool status;
    QSqlQuery *query;
};

class DatabaseUtils : QObject
{
Q_OBJECT
public:
    /**
     * 获取数据源
     * @brief getDataSource
     * @return
     */
    static QSqlDatabase getDatabase();
    static void initDatabase();
    static bool openTranslation(QSqlDatabase sqlDatabase);
    static bool commitTranslation(QSqlDatabase sqlDatabase, bool commitStatus);
    static DataResult execSql(QString str, QSqlDatabase sqlDatabase);
    static DataResult execSql(QString str);
    static void closeDatabase(QSqlDatabase sqlDatabase);
    static DataResult exec(QString sqlStr, QMap<QString, QVariant> paramsMap, bool isTransactionStatus);

private:
    static QString RELATE_LOCATION_DIR;
    static QString DB_NAME;
    static QString LOCAL_PATH;
//    static QString DB_USERNAME;
    static QString DB_PASSWORD;
    static QString DATASOURCE_TYPE;
    static QString DATASOURCE_NAME;
    static QString INIT_DB_SQL;
    static QString INIT_DB_USER_SQL_INDEX;
    static QString INIT_DB_USER_INSERT;
    static QString INIT_HISTORY_SQL;
    static QString INIT_HISTORY_INDEX_SQL;
    static QString INIT_TYPE_SQL;
    static QString INIT_TYPE_INDEX_SQL;
    static bool DEBUG_STATUS;
    static QString INIT_TYPE_INSERT_SQL;
    static QString INIT_SYSTEM_TABLE;
    static QString INIT_SYSTEM_TABLE_INDEX;
};

#endif // DATABASEUTILS_H
