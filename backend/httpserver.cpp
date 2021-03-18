#include "httpserver.h"

using httplib::Request, httplib::Response;

HTTPServer::HTTPServer(const QString& dbFilePath):
    manager(dbFilePath)
{
    Get(R"(/helloworld)", [&](const Request& request, Response& response){ response.body = "hello"; response.headers = request.headers; });
    Post(R"(/regisztracio/etterem)", [&](const Request& request, Response& response){ manager.RegisterRestaurant(request, response); });
}
