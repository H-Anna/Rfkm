#ifndef DBSERVER_H
#define DBSERVER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QString>
#include <QVariantMap>

class DBServer
{
public:
    DBServer(const QString& driver, QString connectionName, const QString& dbName);
    bool OpenCon();
    void queryRegisterRestaurant(const QVariantMap& data, QString* message);
private:
    QSqlDatabase db;
    QString dbName;
    QSqlQuery query;
};

#endif // DBSERVER_H
