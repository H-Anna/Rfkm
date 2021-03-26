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
    void queryRegisterUser(const QVariantMap& data, QString* message);
    void queryLogin(const QStringList& auth, QString* message);
    void querySetOpenHours(const QVariantMap& data, QString* message);
	void queryCreateFood(const QVariantMap &data, QString *message);
    void queryListFood(const QVariantMap &data, QString *message, QString *QueryMessage);
    void queryListRestaurant(const QVariantMap &data, QString *message, QString *QueryMessage);

private:
    QSqlDatabase db;
    QString dbName;
    QSqlQuery query;

    void GenerateStrings(const QVariantMap& map, QString& fields, QString& binds);
    void BindValues(QSqlQuery& query, const QVariantMap& map);
    bool ExecuteQuery(QSqlQuery& query, QString* message, bool batch = false);
    QPair<QString, int> UserExists(QSqlQuery& query, QString* message, const QStringList& auth, const QStringList& tables = {"Etterem", "RegisztraltVendeg", "Futar"});

};

#endif // DBSERVER_H
