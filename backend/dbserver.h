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
    void queryRegisterWorker(const QVariantMap& data, QString* message);
    void queryLogin(const QStringList& auth, QString* message, QString* queryMsg);

    void queryCreateFood(const QVariantMap &data, QString *message);
    void queryCreateFoodTag(const QVariantMap &data, QString *message);
    void queryCreateDiscount(const QVariantMap &data, QString *message);
    void queryPlaceUserOrder(const QVariantMap &data, QString *message);

    void queryListFood(const QVariantMap &data, QString *message, QString *QueryMessage);
    void queryListRestaurant(const QVariantMap &data, QString *message, QString *QueryMessage);
    void queryListRestaurantOrders(const QVariantMap &data, QString *message, QString* queryMsg);
    void queryListRestaurantTag(QString *message, QString *QueryMessage);
    void queryListFoodTag(QString *message, QString *QueryMessage);
    void queryListPaymentTag(const QVariantMap &data,QString *message, QString *QueryMessage);
    void queryListUserOrders(const QVariantMap &data, QString *message, QString* queryMsg);
    void queryListDiscounts(QString *message, QString *QueryMessage);

    void queryUpdateFood(const QVariantMap &data, QString *message);
    void queryUpdateRestaurantBasics(const QVariantMap &data, QString *message);
    void queryUpdateRestaurantAddress(const QVariantMap &data, QString *message);
    void querySetRestaurantOpenHours(const QVariantMap& data, QString* message); 
    void querySetWorkerShare(const QVariantMap& data, QString* message);
    void queryUpdateDiscount(const QVariantMap& data, QString* message);
    void queryUpdateUser(const QVariantMap &data, QString *message);
    void queryUpdateWorker(const QVariantMap &data, QString *message);
    void queryUpdateWorkerHours(const QVariantMap &data, QString *message);

    void queryDeleteFood(const QVariantMap &data, QString *message);
    void queryDeleteFoodTag(const QVariantMap &data, QString *message);
    void queryDeleteDiscount(const QVariantMap &data, QString *message);




    void queryListWorkers(const QVariantMap &data, QString *message, QString *QueryMessage);
    void queryListWorkerOrders(const QVariantMap &data, QString *message, QString *QueryMessage);
    void queryGetSimpleFoodInfo(const QVariantMap &data, QString *message, QString *QueryMessage);
    void queryUpdateRestaurantOrders(const QVariantMap &data, QString *message);

    void queryUpdateOrderPriorityByWorker(const QVariantMap &data, QString *message);
    void queryRejectWorkerOrder(const QVariantMap &data, QString *message);
    void queryCompleteWorkerOrder(const QVariantMap &data, QString *message);
    void queryShowWorkerShare(const QVariantMap &data, QString *message, QString *QueryMessage);

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
    bool insertFoodTags(const QVariantList &cimke, QString *message, const int etelID);
    bool insertRestaurantDelivers(const QVariantList &szallit, QString *message, const int etteremID);
    bool insertRestaurantTags(const QVariantList &cimke, QString *message, const int etteremID);
    bool insertRestaurantOpenHours(const QVariantList &nyitva, QString *message, const int etteremID);
    bool insertWorkerOpenHours(const QVariantList &mikor, QString *message, const int futarID);
    bool insertWorkerDelivers(const QVariantList &szallit, QString *message, const int futarID);
    bool insertUserAddress(const QVariantMap &cim, QString *message, QString &fields, QString &binds);

};

#endif // DBSERVER_H
