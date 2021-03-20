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
    void RegisterNewRestaurant(const Request& request, Response& response);
private:
    DBServer database;

    void JSONMessage(Response& response, int status, const QString& message);
};

#endif // REQUESTMANAGER_H
