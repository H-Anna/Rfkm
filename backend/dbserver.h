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
    void GenerateStrings(const QVariantMap& map, QString& fields, QString& binds);
    void BindValues(QSqlQuery& query, const QVariantMap& map);
    void queryRegisterRestaurant(const QVariantMap& data, QString* message);
    void queryRegisterNewRestaurant(const QVariantMap& data, QString* message);
private:
    QSqlDatabase db;
    QString dbName;
    QSqlQuery query;
};

#endif // DBSERVER_H
