#include <iostream>
#include "httpserver.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    HTTPServer server(R"(D:\Egyetem\negyedikrendszer\Rfkm\backend\db_browser_test.db)");
    server.listen("0.0.0.0", 3000);

    return 0;
}
