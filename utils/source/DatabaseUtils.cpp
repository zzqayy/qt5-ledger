#include "../DatabaseUtils.h"

#include "QDebug"
#include "QCoreApplication"

#include <QStandardPaths>

bool DatabaseUtils::DEBUG_STATUS = false;

//定义常量
//相对目录
QString DatabaseUtils::RELATE_LOCATION_DIR = "data";
//本地路径
QString DatabaseUtils::LOCAL_PATH = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.ledger/");
//数据库名
QString DatabaseUtils::DB_NAME = "data";
////用户名
//QString DatabaseUtils::DB_USERNAME = "ledger_user";
////密码
QString DatabaseUtils::DB_PASSWORD = "l0e1@d2g!3e4r";
//数据库链接类型"SQLITECIPHER"
QString DatabaseUtils::DATASOURCE_TYPE = "SQLITECIPHER";
//QString DatabaseUtils::DATASOURCE_TYPE = "QSQLITE";
//数据库绝对地址
QString DatabaseUtils::DATASOURCE_NAME = QString("")
        .append(DatabaseUtils::LOCAL_PATH)
        .append(DatabaseUtils::RELATE_LOCATION_DIR)
        .append("/")
        .append(DatabaseUtils::DB_NAME);

//初始化数据库的sql
QString DatabaseUtils::INIT_DB_SQL = "create table if not exists ledger_user"
                                     "("
                                     "uid INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     "username VARCHAR(255) not null,"
                                     "password VARCHAR(255),"
                                     " create_time NUMERIC,"
                                     "is_locked INTEGER default 0"
                                     ");";
QString DatabaseUtils::INIT_DB_USER_SQL_INDEX = "create unique index IF NOT EXISTS ledger_user_uid_index on ledger_user (uid);";
QString DatabaseUtils::INIT_DB_USER_INSERT =  "INSERT INTO ledger_user (uid, username, password, create_time, is_locked) VALUES (1, 'admin', 'e17902748d2cc6c7da85a5b5bd862ec2', '2020-09-08 22:42:29.000', 0);";

//初始化历史和类型sql
QString DatabaseUtils::INIT_HISTORY_SQL = "create table if not exists ledger_history ("
                                      "id INTEGER not null constraint ledger_history_pk primary key autoincrement,"
                                      "type_id INTEGER,"
                                      "money_status INTEGER default 1,"
                                      "money INTEGER default 0,"
                                      "record_time NUMERIC,"
                                      "create_time NUMERIC,"
                                      "remark TEXT"
                                      ");";

QString DatabaseUtils::INIT_HISTORY_INDEX_SQL = "create unique index if not exists ledger_history_id_uindex on ledger_history (id);";

QString DatabaseUtils::INIT_TYPE_SQL = "create table if not exists ledger_type ("
                                   "id INTEGER not null constraint ledger_type_pk primary key autoincrement,"
                                   "type_name TEXT,"
                                   "create_time NUMERIC"
                                   ");";

QString DatabaseUtils::INIT_TYPE_INDEX_SQL = "create unique index if not exists ledger_type_id_uindex on ledger_type (id);";

QString DatabaseUtils::INIT_TYPE_INSERT_SQL = "INSERT INTO ledger_type (id, type_name, create_time) VALUES (1, '生活缴费', '2020-09-08 22:42:29.000'), (2, '通勤费用', '2020-09-08 22:42:29.000'), (3, '饮食缴费', '2020-09-08 22:42:29.000'), (4, '娱乐缴费', '2020-09-08 22:42:29.000'), (5, '工资', '2020-09-08 22:42:29.000'), (6, '其他', '2020-09-08 22:42:29.000');";

QString DatabaseUtils::INIT_SYSTEM_TABLE = "create table if not exists ledger_system_cache"
        "("
            "id INTEGER not null primary key autoincrement constraint system_cache_pk,"
            "type TEXT not null,"
            "type_value TEXT"
        ");";

QString DatabaseUtils::INIT_SYSTEM_TABLE_INDEX = "create unique index if not exists system_cache_id_uindex on ledger_system_cache (id);";

/**
 * @brief DatabaseUtils::getDatabase 获取链接
 * @return
 */
QSqlDatabase DatabaseUtils::getDatabase()
{
    QSqlDatabase sqliteDatabase;
    bool isConnection = QSqlDatabase::contains(DatabaseUtils::DATASOURCE_NAME);
    if (isConnection)
    {
        sqliteDatabase = QSqlDatabase::database(DatabaseUtils::DATASOURCE_NAME);
    }
    bool localExists = QFile::exists(DatabaseUtils::DATASOURCE_NAME);
    if(!localExists){
        QDir qDir;
        qDir.mkpath(QString("")
                   .append(DatabaseUtils::LOCAL_PATH)
                   .append(DatabaseUtils::RELATE_LOCATION_DIR));

    }

    sqliteDatabase = QSqlDatabase::addDatabase(DatabaseUtils::DATASOURCE_TYPE);
    sqliteDatabase.setDatabaseName(DatabaseUtils::DATASOURCE_NAME);
//    sqliteDatabase.setUserName(DatabaseUtils::DB_USERNAME);
    sqliteDatabase.setPassword(DatabaseUtils::DB_PASSWORD);
    if(!sqliteDatabase.open())
    {
        qDebug() << "Sqlite Connect Is Failed";
    }
    if (!sqliteDatabase.isValid())
    {
        qDebug() << "Sqlite Connect is Failed";
    }
    return sqliteDatabase;
}

/**
 * @brief DatabaseUtils::openTranslation 开启事务
 * @param sqlDatabase
 * @return
 */
bool DatabaseUtils::openTranslation(QSqlDatabase sqlDatabase)
{
    return sqlDatabase.transaction();
}

/**
 * 关闭数据库链接
 * @param sqlDatabase
 */
void closeDatabase(QSqlDatabase sqlDatabase)
{
    sqlDatabase.close();
}

/**
 * @brief DatabaseUtils::openTranslation 开启事务
 * @param sqlDatabase
 * @return
 */
bool DatabaseUtils::commitTranslation(QSqlDatabase sqlDatabase, bool commitStatus)
{
    if(commitStatus)
    {
        if(!sqlDatabase.commit())
        {
            qDebug() << "Database commit is failed!";
            return false;
        }
    }else
    {
        if(!sqlDatabase.rollback())
        {
            qDebug() << "Database rollback is failed!";
            return false;
        }
    }
    return true;
}

/**
 * @brief DatabaseUtils::execSql 执行sql
 * @param sqlStr sql字符串
 * @return
 */
DataResult DatabaseUtils::execSql(QString sqlStr, QSqlDatabase sqliteDatabase)
{
    //当文件未被打开时候
    while(!sqliteDatabase.isOpen()){
        break;
    }
    //尝试打开文件
    if(!sqliteDatabase.open())
    {
        return {
                false,
                nullptr
        };
    }
    //创建数据库
    QSqlQuery *sqlQuery = new QSqlQuery(sqliteDatabase);
    //打印数据
    if (DEBUG_STATUS)
    {
        qDebug() << "Sql Run: " << sqlStr;
    }
    bool execStatus = sqlQuery -> exec(sqlStr);
    if (DEBUG_STATUS)
    {
        qDebug() << "Run Status: " << execStatus;
    }

    if(!execStatus)
    {
        QSqlError lastError = sqlQuery -> lastError();
        qDebug() << lastError.text();
        return {
                false,
                sqlQuery
        };
    }

    return {
            true,
            sqlQuery
    };
}

DataResult DatabaseUtils::execSql(QString sqlStr)
{
    QSqlDatabase sqliteDatabase = DatabaseUtils::getDatabase();
    return DatabaseUtils::execSql(sqlStr, sqliteDatabase);
}

/**
 * 初始化数据库
 * @brief DatabaseUtils::initDatabase
 * @param sqliteDatabase
 * @return
 */
void DatabaseUtils::initDatabase()
{
    //初始化用户
    QSqlDatabase dataBase = DatabaseUtils::getDatabase();
    DatabaseUtils::execSql(DatabaseUtils::INIT_DB_SQL, dataBase);
    DatabaseUtils::execSql(DatabaseUtils::INIT_DB_USER_SQL_INDEX, dataBase);
    //初始化历史和类型, 系统信息的sql
    DatabaseUtils::execSql(DatabaseUtils::INIT_HISTORY_SQL, dataBase);
    DatabaseUtils::execSql(DatabaseUtils::INIT_HISTORY_INDEX_SQL, dataBase);
    DatabaseUtils::execSql(DatabaseUtils::INIT_TYPE_SQL, dataBase);
    DatabaseUtils::execSql(DatabaseUtils::INIT_TYPE_INDEX_SQL, dataBase);
    DatabaseUtils::execSql(DatabaseUtils::INIT_SYSTEM_TABLE, dataBase);
    DatabaseUtils::execSql(DatabaseUtils::INIT_SYSTEM_TABLE_INDEX, dataBase);

    //插入数据初始化
    DatabaseUtils::openTranslation(dataBase);
    DatabaseUtils::execSql(DatabaseUtils::INIT_DB_USER_INSERT, dataBase);
    DatabaseUtils::commitTranslation(dataBase, true);

    DatabaseUtils::openTranslation(dataBase);
    DatabaseUtils::execSql(DatabaseUtils::INIT_TYPE_INSERT_SQL, dataBase);
    DatabaseUtils::commitTranslation(dataBase, true);

    //插入系统参数
    DatabaseUtils::openTranslation(dataBase);
    DatabaseUtils::execSql("INSERT INTO ledger_system_cache (id, type, type_value) VALUES (1, 'remeberMe', '0');", dataBase);
    DatabaseUtils::execSql("INSERT INTO ledger_system_cache (id, type, type_value) VALUES (2, 'autoLogin', '0');", dataBase);
    DatabaseUtils::execSql("INSERT INTO ledger_system_cache (id, type, type_value) VALUES (3, 'username', '');", dataBase);
    DatabaseUtils::execSql("INSERT INTO ledger_system_cache (id, type, type_value) VALUES (4, 'password', '');", dataBase);
    DatabaseUtils::commitTranslation(dataBase, true);

}

/**
 * @brief exec 执行sql
 * @param sqlStr
 * @param paramsMap
 * @return
 */
DataResult DatabaseUtils::exec(QString sqlStr, QMap<QString, QVariant> paramsMap, bool isTransactionStatus)
{
    QSqlDatabase database = DatabaseUtils::getDatabase();
    QSqlQuery *sqlQuery = new QSqlQuery(database);
    if(isTransactionStatus)
    {
        database.transaction();
    }
    sqlQuery->prepare(sqlStr);
    QMapIterator<QString, QVariant> mapIterator(paramsMap);
    while (mapIterator.hasNext())
    {
        mapIterator.next();
        sqlQuery->bindValue(mapIterator.key(), mapIterator.value());
    }

    bool execStatus = sqlQuery->exec();
    if(!execStatus)
    {
        return {
            false,
            nullptr
        };
    }
    if(isTransactionStatus)
    {
        bool localCommit = database.commit();
        if(!localCommit)
        {
            return {
              false,
              nullptr
            };
        }
    }
    return {
        true,
        sqlQuery
    };
}

