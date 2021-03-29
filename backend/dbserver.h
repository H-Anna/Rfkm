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
    void queryLogin(const QStringList& auth, QString* message, QString* queryMsg);

    void queryCreateFood(const QVariantMap &data, QString *message);
    void queryCreateFoodTag(const QVariantMap &data, QString *message);

    void queryListFood(const QVariantMap &data, QString *message, QString *QueryMessage);
    void queryListRestaurant(const QVariantMap &data, QString *message, QString *QueryMessage);
    void queryListRestaurantTag(QString *message, QString *QueryMessage);
    void queryListFoodTag(QString *message, QString *QueryMessage);

    void queryUpdateFood(const QVariantMap &data, QString *message, QString *QueryMessage);
    void queryUpdateRestaurantBasics(const QVariantMap &data, QString *message);
    void queryUpdateRestaurantAddress(const QVariantMap &data, QString *message);
    void querySetOpenHours(const QVariantMap& data, QString* message);
    void queryUpdateUser(const QVariantMap &data, QString *message);

    void queryDeleteFood(const QVariantMap &data, QString *message);
    void queryDeleteFoodTag(const QVariantMap &data, QString *message);


    bool insertUserAddress(const QVariantMap &cim, QString *message, QString &fields, QString &binds);
private:
    QSqlDatabase db;
    QString dbName;
    QSqlQuery query;

    void GenerateStrings(const QVariantMap& map, QString& fields, QString& binds);
    void GenerateUpdateString(const QVariantMap &map, QString &string);

    void BindValues(QSqlQuery& query, const QVariantMap& map);
    bool ExecuteQuery(QSqlQuery& query, QString* message, bool batch = false);
    QPair<QString, int> UserExists(QSqlQuery& query, QString* message, const QStringList& auth);

    bool insertFoodOpenHours(const QVariantMap &idoszak, QString *message, QString &fields, QString &binds);
    bool insertFoodTags(const QVariantList &cimke, QString *message, int etelID);
    bool insertRestaurantDelivers(const QVariantList &szallit, QString *message, int etteremID);
    bool insertRestaurantTags(const QVariantList &cimke, QString *message, int etteremID);
    bool insertRestaurantOpenHours(const QVariantList &nyitva, QString *message, int etteremID);

};

#endif // DBSERVER_H
