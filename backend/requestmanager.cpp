#include "requestmanager.h"

#include <QJsonObject>
#include <QJsonDocument>

#include <QSet>

using namespace std;

RequestManager::RequestManager(const QString& dbFilePath):
    database("QSQLITE", "Etelszallitas", dbFilePath)
{
    if (!database.OpenCon()) {
        cout << "[Database] Error : Could not open database";
    }
}

void RequestManager::JSONMessage(Response& response, int status, const QString& message) {

    QJsonObject obj;
    obj.insert("message", message);
    response.set_content(QJsonDocument(obj).toJson().toStdString(), "application/json");
    response.status = status;

    cout << message.toStdString() << endl;
}

void RequestManager::RegisterRestaurant(const Request& request, Response& response) {

    cout << "[RequestManager] Log : Starting Restaurant registration..." << endl;
    cout << "[RequestManager] Log : Checking for errors..." << endl;

    //Hibakezelés

    //Nem kaptunk adatot?
    if (request.body.empty()) {
        JSONMessage(response, 400, "[RequestManager] Error : Empty request body");
        return;
    }

    //Nem megfelelő típusú adatot kaptunk?
    QVariantMap jsonData = QJsonDocument::fromJson(QString::fromStdString(request.body).toUtf8()).toVariant().toMap();

    if (jsonData.value("Tipus") != "etterem") {
        JSONMessage(response, 400, "[RequestManager] Error : Incorrect type in JSON");
        return;
    }

    //Nem annyit, vagy nem azokat a kulcsokat kaptuk amiket vártunk?
    QVariantMap data = jsonData.value("Adatok").toJsonObject().toVariantMap();

    QStringList validKeys = { "Email", "Jelszo", "Nev", "Leiras", "Cim" };
    QSet<QString> valid, received;

    for (auto const &key : validKeys) {
        valid.insert(key);
    }
    for (auto const &key : data.keys()) {
        received.insert(key);
    }

    if (valid != received) {
        JSONMessage(response, 400, "[RequestManager] Error : Bad JSON");
        return;
    }

    //Hibakezelés vége

    cout << "[RequestManager] Log : Forwarding data to DBServer..." << endl;

    //Ha minden oké, továbbadjuk az adatbázisnak az adatokat
    QString msg("OK");
    database.queryRegisterRestaurant(data, &msg);

    int status = 200;

    //Ellenőrizzük a query eredményét
    if (msg != "OK") {
        cout << "[RequestManager] Error occured during DB write" << endl;
        status = 500;
    }

    JSONMessage(response, status, msg);

    cout << "[RequestManager] Log : Response sent." << endl;
    return;
}
