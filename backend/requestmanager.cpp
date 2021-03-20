#include "requestmanager.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

//#include <QSet>

using namespace std;

RequestManager::RequestManager(const QString& dbFilePath):
    database("QSQLITE", "Etelszallitas", dbFilePath)
{
    if (!database.OpenCon()) {
        cout << "[Database] Error : Could not open database";
    }
}

void RequestManager::JSONMessage(Response& response, int status, const QString& message)
{

    QJsonObject obj;
    obj.insert("Message", message);
    response.set_content(QJsonDocument(obj).toJson().toStdString(), "application/json");
    response.status = status;

    cout << message.toStdString() << endl;
}

void RequestManager::RegisterRestaurant(const httplib::Request &request, Response &response)
{
    cout << "[RequestManager] Log : Starting Restaurant registration..." << endl;
    cout << "[RequestManager] Log : Checking for errors..." << endl;

    //Hibakezelés

    //Nem kaptunk adatot?
    if (request.body.empty()) {
        JSONMessage(response, 400, "[RequestManager] Error : Empty request body");
        return;
    }

    //Nem megfelelő formátum?
    //1. Fő objektum
    QJsonObject jsonObject = QJsonDocument::fromJson(QString::fromStdString(request.body).toUtf8()).object();
    {
        QStringList requiredKeys = { "Email", "Jelszo", "Nev", "Leiras", "Cim", "Nyitvatartas", "Cimke", "Kep", "Szallit" };
        for (auto const& key : requiredKeys) {
            if (!jsonObject.keys().contains(key)) {
                JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
                return;
            }
        }
    }
    //2. Cím
    QJsonObject jsonCim = jsonObject.value("Cim").toObject();
    {
        QStringList requiredKeys = { "Irsz", "Kozterulet", "Hazszam", "Emelet_ajto" };
        for (auto const& key : requiredKeys) {
            if (!jsonCim.keys().contains(key)) {
                JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect object Cim");
                return;
            }
        }
    }
    //3. Nyitvatartás
    QJsonArray jsonNyitva = jsonObject.value("Nyitvatartas").toArray();
    {
        if (jsonNyitva.count() != 7) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Not enough entries in Nyitvatartas");
            return;
        }

        QStringList requiredKeys = { "NapID", "KonyhaNyit", "EtteremNyit", "EtteremZar", "KonyhaZar" };

        for (const auto& val: jsonNyitva){
            if (val == QJsonValue::Null)
                continue;

            QJsonObject nyitvaObject = val.toObject();
            for (const auto& key: requiredKeys) {
                if (!nyitvaObject.keys().contains(key)) {
                    JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect object entry in Nyitvatartas");
                    return;
                }
            }
        }
    }


    //Hibakezelés vége

    cout << "[RequestManager] Log : Forwarding data to DBServer..." << endl;

    QVariantMap data = jsonObject.toVariantMap();
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
