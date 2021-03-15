#include <iostream>
#include "httplib.h"

using namespace std;
using httplib::Server, httplib::Request, httplib::Response;

int main()
{
    cout << "Hello World!" << endl;

    Server server;
    server.Get(R"(/helloworld)", [&](const Request& request, Response& response){ response.body = "hello"; response.headers = request.headers; });

    server.listen("0.0.0.0", 3000);
    return 0;
}
