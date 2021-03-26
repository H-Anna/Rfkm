#include "httpserver.h"

using httplib::Request, httplib::Response;

HTTPServer::HTTPServer(const QString& dbFilePath):
    manager(dbFilePath)
{
    Get(R"(/helloworld)", [&](const Request& request, Response& response){ response.body = "hello"; response.headers = request.headers; });
    Post(R"(/regisztracio/etterem)", [&](const Request& request, Response& response){ manager.RegisterRestaurant(request, response); });

    Post(R"(/etterem/etel)", [&](const Request& request, Response& response){ manager.CreateFood(request, response); });
    Get(R"(/etterem/(\d+))", [&](const Request& request, Response& response){ manager.ListFood(request, response); });
    Post(R"(/etterem)", [&](const Request& request, Response& response){ manager.ListRestaurant(request, response); });
}
