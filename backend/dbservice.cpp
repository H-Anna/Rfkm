#include "dbservice.h"



DBService::DBService()
{

    db = new Db_server("QSQLITE", "Auction", route);
    closes = new QMap<QString,QString>();
    closer = new Auction_closer(closes);
    db->init();


    Server server;
    server.Get(R"(/helloworld)", [&](const Request& request, Response& response){ response.body = "hello"; response.headers = request.headers; });

    server.listen("0.0.0.0", 3000);
}
