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
    bool ExecuteQuery(QSqlQuery& query, QString* message, bool batch = false);

    void queryRegisterRestaurant(const QVariantMap& data, QString* message);
    void queryCreateFood(const QVariantMap &data, QString *message);
    void queryListFood(const QVariantMap &data, QString *message, QString *QueryMessage);
    void queryListRestaurant(const QVariantMap &data, QString *message, QString *QueryMessage);
private:
    QSqlDatabase db;
    QString dbName;
    QSqlQuery query;
};

#endif // DBSERVER_H
