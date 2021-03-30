#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "httplib.h"
#include "requestmanager.h"

using httplib::Server;

class HTTPServer: public Server
{
public:
    HTTPServer(const QString& dbFilePath);
private:
    RequestManager manager;
};

#endif // HTTPSERVER_H
