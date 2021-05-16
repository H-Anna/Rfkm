#include "httpserver.h"

using httplib::Request, httplib::Response;

HTTPServer::HTTPServer(const QString& dbFilePath):
    manager(dbFilePath)
{
    Get(R"(/helloworld)", [&](const Request& request, Response& response){ response.body = "hello"; response.headers = request.headers; response.set_header("Access-Control-Allow-Origin", "*"); });

    //-- LÉTREHOZÁSOK --
    //Regisztrációk
    Post(R"(/regisztracio/etterem)", [&](const Request& request, Response& response){ manager.RegisterRestaurant(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/regisztracio/vendeg)", [&](const Request& request, Response& response){ manager.RegisterUser(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/regisztracio/futar)", [&](const Request& request, Response& response){ manager.RegisterWorker(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    //Bejelentkezés
    Post(R"(/belepes)", [&](const Request& request, Response& response){ manager.Login(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    //Étel felvitele
    Post(R"(/etterem/etel)", [&](const Request& request, Response& response){ manager.CreateFood(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    //Étel kategória felvitele
    Post(R"(/cimke/etel)", [&](const Request& request, Response& response){ manager.CreateFoodTag(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    //Akció felvitele
    Post(R"(/etterem/akcio)", [&](const Request& request, Response& response){ manager.CreateDiscount(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    //Rendelés küldése
    Post(R"(/vendeg/rendeles)", [&](const Request& request, Response& response){ manager.PlaceUserOrder(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    //-- LEKÉRÉSEK --
    //Éttermek listázása
    Post(R"(/etterem)", [&](const Request& request, Response& response){ manager.ListRestaurant(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    //Ételek lekérése éttermenként
    Get(R"(/etterem/(\d+))", [&](const Request& request, Response& response){ manager.ListFood(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    //Étel információ lekérése ID alapján
    Get(R"(/etel/info/(\d+))", [&](const Request& request, Response& response){ manager.GetSimpleFoodInfo(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    //Cimkék lekérése
    Get(R"(/cimke/etterem)", [&](const Request& request, Response& response){ manager.ListRestaurantTag(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Get(R"(/cimke/etel)", [&](const Request& request, Response& response){ manager.ListFoodTag(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Get(R"(/cimke/etterem/fizetes/(\d+))", [&](const Request& request, Response& response){ manager.ListPaymentTag(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    //Akciók lekérése
    Get(R"(/etterem/akcio)", [&](const Request& request, Response& response){ manager.ListDiscounts(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    //Futárok lekérése
    Post(R"(/futarok)", [&](const Request& request, Response& response){ manager.ListWorkers(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    //Rendelések lekérése
    Get(R"(/etterem/rendelesek/(\d+))", [&](const Request& request, Response& response){ manager.ListRestaurantOrders(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Get(R"(/vendeg/rendelesek/(\d+))", [&](const Request& request, Response& response){ manager.ListUserOrders(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Get(R"(/futar/rendelesek/(\d+))", [&](const Request& request, Response& response){ manager.ListWorkerOrders(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });


    //-- UPDATE --
    //Vendég módosítás
     Post(R"(/vendeg/modositas)", [&](const Request& request, Response& response){ manager.UpdateUser(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    //Étterem módosítás
    Post(R"(/etterem/alapok/modositas)", [&](const Request& request, Response& response){ manager.UpdateRestaurantBasics(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/etterem/cim/modositas)", [&](const Request& request, Response& response){ manager.UpdateRestaurantAddress(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/etterem/nyitvatartas/modositas)", [&](const Request& request, Response& response){ manager.SetRestaurantOpenHours(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    //Futár módosítás
    Post(R"(/futar/modositas)", [&](const Request& request, Response& response){ manager.UpdateWorker(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/futar/modositas/idoszak)", [&](const Request& request, Response& response){ manager.UpdateWorkerHours(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    //Étel módosítás
    Post(R"(/etterem/etel/modositas)", [&](const Request& request, Response& response){ manager.UpdateFood(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    //Akció módosítás
    Post(R"(/etterem/akcio/modositas)", [&](const Request& request, Response& response){ manager.UpdateDiscount(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    //Rendelés módosítás
    Post(R"(/etterem/rendeles/modositas)", [&](const Request& request, Response& response){ manager.UpdateRestaurantOrder(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/etterem/futarreszesedes/modositas)", [&](const Request& request, Response& response){ manager.SetWorkerShare(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/futar/prioritas)", [&](const Request& request, Response& response){ manager.UpdateOrderPriorityByWorker(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/futar/elutasit)", [&](const Request& request, Response& response){ manager.RejectWorkerOrder(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    Post(R"(/futar/kesz)", [&](const Request& request, Response& response){ manager.CompleteWorkerOrder(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    //TÖRLÉS
    //Étel törlése
    Post(R"(/etterem/etel/torles/(\d+))", [&](const Request& request, Response& response){ manager.DeleteFood(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    //Étel kategória törlése
    Post(R"(/cimke/etel/torles/(\d+))", [&](const Request& request, Response& response){ manager.DeleteFoodTag(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
    //Akció törlése
    Post(R"(/etterem/akcio/torles/(\d+))", [&](const Request& request, Response& response){ manager.DeleteDiscount(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });

    Get(R"(/futar/reszesedes/(\d+))", [&](const Request& request, Response& response){ manager.ShowWorkerShare(request, response); response.set_header("Access-Control-Allow-Origin", "*"); });
}
