#include "dbmysql.h"
#include "src/config/globals.h"
#include "filehelper.h"
#include <QSqlRecord>

DbMySql::DbMySql()
{
    // 使用 SQLite 替代 MySQL（Qt 内置支持，无需额外驱动）
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

DbMySql::~DbMySql()
{
    if(m_db.isOpen())
    {
        m_db.close();
    }
}

void DbMySql::connect(const QString &dbname)
{
    // SQLite 使用本地文件，不需要主机名、用户名、密码
    // 数据库文件保存在临时目录
    QString dbPath = FileHelper::joinPath(GLOBAL::PATH::TMP, dbname + ".db");
    m_db.setDatabaseName(dbPath);

    if(!m_db.open())
    {
        throw QString::fromUtf8("打开数据库失败: %1 %2").arg(dbPath, m_db.lastError().text());
    }
}


bool DbMySql::exists(const QString &sql)
{
    QSqlQuery query = exec(sql);
    return query.next();        // 如果有数据，next == true
}


QSqlQuery DbMySql::exec(const QString &sql)
{
    QSqlQuery query;
    if(!query.exec(sql))
    {
        throw QString::fromUtf8("执行sql失败：%1 %2").arg(sql,m_db.lastError().text());
    }
    return query;
}

// 执行sql，防止sql注入
QSqlQuery DbMySql::exec(const QString &sql, QVariantList &variantList)
{
    QSqlQuery query;

    // 判断sql有没有准备好，是否是可绑定后执行的语句
    if(!query.prepare(sql))
    {
        throw QString::fromUtf8("预编译sql失败：%1 %2").arg(sql,m_db.lastError().text());
    }

    // 绑定参数
    for(int i = 0; i < variantList.size(); i++)
    {
        query.bindValue(i, variantList[i]);
    }

    if(!query.exec())
    {
        throw QString::fromUtf8("执行sql bindvalue失败：%1 %2").arg(sql,m_db.lastError().text());
    }

    return query;
}

// 根据sql进行查询，返回查询结果的maplist
QList<RECORD> DbMySql::select(const QString& sql)
{
    QList<RECORD> retList;
    QSqlQuery query = exec(sql);

    while(query.next())             // 遍历结果集
    {
        RECORD ret;                     // RECORD是一种map结构{qstring,qvariant}

        QSqlRecord record = query.record();         // 数据库中的一行记录
        for(int i = 0; i < record.count(); ++i)     // 其count为一条记录有多少个元素
        {
            QString name = record.fieldName(i);     // 元素名
            QVariant value = record.value(i);       // 元素内容
            ret[name] = value;
        }

        retList.append(ret);
    }
    return retList;
}
