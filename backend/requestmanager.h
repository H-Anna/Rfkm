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
    void Login(const Request& request, Response& response);

    void CreateFood(const httplib::Request &request, Response &response);
    void CreateFoodTag(const httplib::Request &request, Response &response);

    void ListFood(const httplib::Request &request, Response &response);
    void ListRestaurant(const httplib::Request &request, Response &response);
    void ListRestaurantTag(const httplib::Request &request, Response &response);
    void ListFoodTag(const httplib::Request &request, Response &response);

    void UpdateFood(const httplib::Request &request, Response &response);
    void UpdateRestaurantBasics(const httplib::Request &request, Response &response);
    void UpdateRestaurantAddress(const httplib::Request &request, Response &response);
    void SetOpenHours(const Request& request, Response& response);
    void UpdateUser(const httplib::Request &request, Response &response);

    void DeleteFood(const httplib::Request &request, Response &response);
    void DeleteFoodTag(const httplib::Request &request, Response &response);

private:
    DBServer database;

    void JSONMessage(Response& response, int status, const QString& message);
    bool CheckJSON(const QJsonObject& toCheck, const QStringList& requiredKeys);
};

#endif // REQUESTMANAGER_H
