#include "httpserver.h"

using httplib::Request, httplib::Response;

HTTPServer::HTTPServer(const QString& dbFilePath):
    manager(dbFilePath)
{
    Get(R"(/helloworld)", [&](const Request& request, Response& response){ response.body = "hello"; response.headers = request.headers; response.set_header("Access-Control-Allow-Origin", "*"); });

    Post(R"(/regisztracio/etterem)", [&](const Request& request, Response& response){ manager.RegisterRestaurant(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/regisztracio/vendeg)", [&](const Request& request, Response& response){ manager.RegisterUser(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/belepes)", [&](const Request& request, Response& response){ manager.Login(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    Post(R"(/etterem/etel)", [&](const Request& request, Response& response){ manager.CreateFood(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/cimke/etel)", [&](const Request& request, Response& response){ manager.CreateFoodTag(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    Get(R"(/etterem/(\d+))", [&](const Request& request, Response& response){ manager.ListFood(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/etterem)", [&](const Request& request, Response& response){ manager.ListRestaurant(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Get(R"(/cimke/etterem)", [&](const Request& request, Response& response){ manager.ListRestaurantTag(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Get(R"(/cimke/etel)", [&](const Request& request, Response& response){ manager.ListFoodTag(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    Post(R"(/etterem/etel/modositas)", [&](const Request& request, Response& response){ manager.UpdateFood(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/etterem/alapok/modositas)", [&](const Request& request, Response& response){ manager.UpdateRestaurantBasics(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/etterem/cim/modositas)", [&](const Request& request, Response& response){ manager.UpdateRestaurantAddress(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/edit/nyitvatartas)", [&](const Request& request, Response& response){ manager.SetOpenHours(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/vendeg/modositas)", [&](const Request& request, Response& response){ manager.UpdateUser(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    Post(R"(/etterem/etel/torles/(\d+))", [&](const Request& request, Response& response){ manager.DeleteFood(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/cimke/etel/torles/(\d+))", [&](const Request& request, Response& response){ manager.DeleteFoodTag(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });


}
