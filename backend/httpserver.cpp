#include "httpserver.h"

using httplib::Request, httplib::Response;

HTTPServer::HTTPServer(const QString& dbFilePath):
    manager(dbFilePath)
{
    Get(R"(/helloworld)", [&](const Request& request, Response& response){ response.body = "hello"; response.headers = request.headers; });
    Post(R"(/regisztracio/etterem)", [&](const Request& request, Response& response){ manager.RegisterRestaurant(request, response); });
    Post(R"(/regisztracio/vendeg)", [&](const Request& request, Response& response){ manager.RegisterUser(request, response); });
    Get(R"(/belepes)", [&](const Request& request, Response& response){ manager.Login(request, response); });
    Post(R"(/edit/nyitvatartas)", [&](const Request& request, Response& response){ manager.SetOpenHours(request, response); });

}
