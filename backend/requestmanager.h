#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include "dbserver.h"

#include "httplib.h"

#include <QString>
#include <QVariantMap>

using httplib::Request, httplib::Response;

class RequestManager
{
public:
    RequestManager(const QString& dbFilePath);
    void RegisterRestaurant(const Request& request, Response& response);
    void RegisterUser(const Request& request, Response& response);
    void RegisterWorker(const Request& request, Response& response);
    void Login(const Request& request, Response& response);

    void CreateFood(const Request &request, Response &response);
    void CreateFoodTag(const Request &request, Response &response);
    void CreateDiscount(const Request &request, Response &response);
    void PlaceUserOrder(const Request& request, Response& response);

    void ListFood(const Request &request, Response &response);
    void ListRestaurant(const Request &request, Response &response);
    void ListRestaurantOrders(const Request& request, Response& response);
    void ListRestaurantTag(const Request &request, Response &response);
    void ListFoodTag(const Request &request, Response &response);
    void ListPaymentTag(const Request &request, Response &response);
    void ListUserOrders(const Request& request, Response& response);
    void ListDiscounts(const Request& request, Response& response);

    void UpdateFood(const Request &request, Response &response);
    void UpdateRestaurantBasics(const Request &request, Response &response);
    void UpdateRestaurantAddress(const Request &request, Response &response);
    void SetRestaurantOpenHours(const Request& request, Response& response);
    void SetWorkerShare(const Request& request, Response& response);
    void UpdateDiscount(const Request &request, Response &response);
    void UpdateUser(const Request &request, Response &response);
    void UpdateWorker(const Request &request, Response &response);
    void UpdateWorkerHours(const Request &request, Response &response);

    void DeleteFood(const Request &request, Response &response);
    void DeleteFoodTag(const Request &request, Response &response);
    void DeleteDiscount(const Request &request, Response &response);




    void GetSimpleFoodInfo(const Request& request, Response& response);
    void UpdateRestaurantOrder(const Request& request, Response& response);
    void ListWorkers(const Request &request, Response &response);
    void ListWorkerOrders(const Request &request, Response &response);
    void UpdateOrderPriorityByWorker(const Request &request, Response &response);
    void RejectWorkerOrder(const Request &request, Response &response);
    void CompleteWorkerOrder(const Request &request, Response &response);
    void ShowWorkerShare(const Request &request, Response &response);

private:
    DBServer database;

    void JSONMessage(Response& response, int status, const QString& message);
    bool JSONHasAllKeys(const QJsonObject& toCheck, const QStringList& requiredKeys);
    bool JSONHasAnyKey(const QJsonObject &toCheck, const QStringList &requiredKeys);
};

#endif // REQUESTMANAGER_H
