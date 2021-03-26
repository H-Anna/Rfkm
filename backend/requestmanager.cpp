#include "requestmanager.h"

#include "Base64.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <QRegularExpression>

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

    QJsonValue a = QJsonValue::Null;

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
        QList<int> napIDNums;

        for (const auto& val: jsonNyitva){
            if (val == QJsonValue::Null)
                continue;

            QJsonObject nyitvaObject = val.toObject();
            for (const auto& key: requiredKeys) {
                if (!nyitvaObject.keys().contains(key)) {
                    JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect object entry in Nyitvatartas");
                    return;
                }
                int idnum = nyitvaObject.value("NapID").toInt();
                if (napIDNums.contains(idnum)) {
                    JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Duplicate NapID in Nyitvatartas objects");
                    return;
                } else {
                    napIDNums.append(idnum);
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

void RequestManager::RegisterUser(const httplib::Request &request, Response &response)
{
    cout << "[RequestManager] Log : Starting User registration..." << endl;
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
        QStringList requiredKeys = { "Email", "Jelszo", "VNev", "KNev", "Cim", "Telefonszam" };
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
    //3. Telefonszám
    QJsonValue jsonTel = jsonObject.value("Telefonszam");
    {
        QRegularExpression re(R"([+]\d{1,2}\d{1,2}\d{7})");
        QRegularExpression re2(R"([06]\d{1,2}\d{7,8})");

        if (!re.match(jsonTel.toString()).hasMatch() || !re2.match(jsonTel.toString()).hasMatch()) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect format for Telefonszam");
            return;
        }
    }

    //Hibakezelés vége

    cout << "[RequestManager] Log : Forwarding data to DBServer..." << endl;

    QVariantMap data = jsonObject.toVariantMap();
    //Ha minden oké, továbbadjuk az adatbázisnak az adatokat
    QString msg("OK");
    database.queryRegisterUser(data, &msg);

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

void RequestManager::Login(const httplib::Request &request, Response &response)
{
    cout << "[RequestManager] Log : Starting Login process..." << endl;
    cout << "[RequestManager] Log : Checking for errors..." << endl;

    //Hibakezelés

    //Nem kaptunk adatot?
    if (request.headers.empty()) {
        JSONMessage(response, 400, "[RequestManager] Error : Empty request headers");
        return;
    }

    //Nincs egy bizonyos header (Authorization)
    if (!request.has_header("Authorization") || request.get_header_value("Authorization").length() == 0) {
        JSONMessage(response, 400, "[RequestManager] Error : No Authorization in request header");
        return;
    }

    //Hibakezelés vége
    cout << "[RequestManager] Log : Decoding..." << endl;

    //A header a bejelentkezési adatokat Base64 kódolva tartalmazza.
    //Az Authorization header tartalma: "Basic (kód)"
    //Ezért a string elejétől (a szóközig) meg kell szabadulni.
    QString value = QString::fromStdString(request.get_header_value("Authorization"));
    QString base = value.right(value.length() - value.indexOf(" ") - 1);
    auto a = macaron::Base64::Decode(base.toStdString());
    auto fullauth = QString::fromStdString(string(a.begin(), a.end()));
    auto auth = fullauth.split(":");

    cout << "[RequestManager] Log : Forwarding data to DBServer..." << endl;

    //QVariantMap data = jsonObject.toVariantMap();
    //Ha minden oké, továbbadjuk az adatbázisnak az adatokat
    QString msg("OK");
    database.queryLogin(auth, &msg);

    int status = 200;

    //Ellenőrizzük a query eredményét
    if (msg != "OK") {
        cout << "[RequestManager] Error occured in DB" << endl;
        status = 500;
    }

    JSONMessage(response, status, msg);

    cout << "[RequestManager] Log : Response sent." << endl;
    return;
}

void RequestManager::SetOpenHours(const httplib::Request &request, Response &response)
{
    cout << "[RequestManager] Log : Starting User registration..." << endl;
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
    cout << "[RequestManager] DebugLog: " << jsonObject.value("EtteremID").toString().toStdString() << endl;
    {
        QStringList requiredKeys = { "EtteremID", "Nyitvatartas" };
        for (auto const& key : requiredKeys) {
            if (!jsonObject.keys().contains(key)) {
                JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
                return;
            }
        }
    }
    //2. Nyitvatartás
    QJsonArray jsonNyitva = jsonObject.value("Nyitvatartas").toArray();
    {
        if (jsonNyitva.count() != 7) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Not enough entries in Nyitvatartas");
            return;
        }

        QStringList requiredKeys = { "NapID", "KonyhaNyit", "EtteremNyit", "EtteremZar", "KonyhaZar" };

        QList<int> napIDNums;
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

            int idnum = nyitvaObject.value("NapID").toInt();
            if (napIDNums.contains(idnum)) {
                JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Duplicate NapID in Nyitvatartas objects");
                return;
            } else {
                napIDNums.append(idnum);
            }
        }
    }

    //Hibakezelés vége

    cout << "[RequestManager] Log : Forwarding data to DBServer..." << endl;

    QVariantMap data = jsonObject.toVariantMap();
    //Ha minden oké, továbbadjuk az adatbázisnak az adatokat
    QString msg("OK");
    database.querySetOpenHours(data, &msg);

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

void RequestManager::CreateFood(const httplib::Request &request, Response &response)
{
    cout << "[RequestManager] Log : Starting  New Food Creation..." << endl;
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
        QStringList requiredKeys = { "Nev", "Ar", "Kep", "Leiras", "EtteremID", "Cimke"};
        for (auto const& key : requiredKeys) {
            if (!jsonObject.keys().contains(key)) {
                JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
                return;
            }
        }
    }
    //2. Idoszak
    QJsonObject jsonIdoszak = jsonObject.value("Idoszak").toObject();
    {
        QStringList requiredKeys = { "Kezdes", "Befejezes" };
        for (auto const& key : requiredKeys) {
            if (!jsonIdoszak.keys().contains(key)) {
                JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect object Idoszak");
                return;
            }
        }
    }
    //3. Akcio
    QJsonObject jsonAkcio = jsonObject.value("Akcio").toObject();
    {
        QStringList requiredKeys = { "Nev", "Ertek" };
        for (auto const& key : requiredKeys) {
            if (!jsonAkcio.keys().contains(key)) {
                JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect object Akcio");
                return;
            }
        }
    }

    //Hibakezelés vége

    cout << "[RequestManager] Log : Forwarding data to DBServer..." << endl;

    QVariantMap data = jsonObject.toVariantMap();
    //Ha minden oké, továbbadjuk az adatbázisnak az adatokat
    QString msg("OK");
    database.queryCreateFood(data, &msg);

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

void RequestManager::ListFood(const httplib::Request &request, Response &response)
{
    cout << "[RequestManager] Log : Starting Food Listing..." << endl;
    cout << "[RequestManager] Log : Forwarding data to DBServer..." << endl;

    QVariantMap data;
    QString id = QString::fromStdString(request.matches[1]);
    data.insert("EtteremID", id);

    QString msg("OK");
    QString queryMsg("");
    database.queryListFood(data, &msg, &queryMsg);

    int status = 200;

    //Ellenőrizzük a query eredményét
    if (msg != "OK") {
        cout << "[RequestManager] Error occured during DB write" << endl;
        status = 500;
        JSONMessage(response, status, msg);
    }
    else{
        response.set_content(queryMsg.toStdString(), "application/json");
        response.status = status;
    }

    cout << "[RequestManager] Log : Response sent." << endl;
    return;
}

void RequestManager::ListRestaurant(const httplib::Request &request, Response &response)
{
    cout << "[RequestManager] Log : Starting Restaurant Listing..." << endl;
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
        QStringList requiredKeys = { "Iranyitoszam"};
        for (auto const& key : requiredKeys) {
            if (!jsonObject.keys().contains(key)) {
                JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
                return;
            }
        }
    }

    //Hibakezelés vége

    cout << "[RequestManager] Log : Forwarding data to DBServer..." << endl;

    QVariantMap data = jsonObject.toVariantMap();
    //Ha minden oké, továbbadjuk az adatbázisnak az adatokat
    QString msg("OK");
    QString queryMsg("");
    database.queryListRestaurant(data, &msg, &queryMsg);

    int status = 200;

    //Ellenőrizzük a query eredményét
    if (msg != "OK") {
        cout << "[RequestManager] Error occured during DB write" << endl;
        status = 500;
        JSONMessage(response, status, msg);
    }
    else{
        response.set_content(queryMsg.toStdString(), "application/json");
        response.status = status;
    }

    cout << "[RequestManager] Log : Response sent." << endl;
    return;
}
