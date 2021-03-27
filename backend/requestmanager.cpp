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

bool RequestManager::CheckJSON(const QJsonObject &toCheck, const QStringList &requiredKeys)
{
    for (auto const& key : requiredKeys) {
        if (!toCheck.keys().contains(key)) {
            return false;
        }
    }
    return true;
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
        if (!CheckJSON(jsonObject, requiredKeys)) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
            return;
        }
    }
    //2. Cím
    QJsonObject jsonCim = jsonObject.value("Cim").toObject();
    {
        QStringList requiredKeys = { "Irsz", "Kozterulet", "Hazszam", "Emelet_ajto" };
        if (!CheckJSON(jsonCim, requiredKeys)) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect object Cim");
            return;
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

        for (auto val: jsonNyitva){
            if (val == QJsonValue::Null)
                continue;

            QJsonObject nyitvaObject = val.toObject();
            if (!CheckJSON(nyitvaObject, requiredKeys)) {
                JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect object Cim");
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
        if (!CheckJSON(jsonObject, requiredKeys)) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
            return;
        }
    }
    //2. Cím
    QJsonObject jsonCim = jsonObject.value("Cim").toObject();
    {
        QStringList requiredKeys = { "Irsz", "Kozterulet", "Hazszam", "Emelet_ajto" };
        if (!CheckJSON(jsonCim, requiredKeys)) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
            return;
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
    if (request.body.empty()) {
        JSONMessage(response, 400, "[RequestManager] Error : Empty request headers");
        return;
    }

    //Nem megfelelő formátum?
    QJsonObject jsonObject = QJsonDocument::fromJson(QString::fromStdString(request.body).toUtf8()).object();
    {
        QStringList requiredKeys = { "Email", "Jelszo" };
        if (!CheckJSON(jsonObject, requiredKeys)) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
            return;
        }
    }


    //Hibakezelés vége

    cout << "[RequestManager] Log : Forwarding data to DBServer..." << endl;

    //Ha minden oké, továbbadjuk az adatbázisnak az adatokat

    QStringList auth({jsonObject.value("Email").toString(), jsonObject.value("Jelszo").toString()});
    QString msg("OK");
    QString queryMsg;
    database.queryLogin(auth, &msg, &queryMsg);

    int status = 200;

    //Ellenőrizzük a query eredményét
    if (msg != "OK") {

        cout << "[RequestManager] Error occured during DB write" << endl;
        status = 500;
        JSONMessage(response, status, msg);

    } else {
        response.set_content(queryMsg.toStdString(), "application/json");
        response.status = status;
    }

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
        if (!CheckJSON(jsonObject, requiredKeys)) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
            return;
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
            if (!CheckJSON(nyitvaObject, requiredKeys)) {
                JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
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
        if (!CheckJSON(jsonObject, requiredKeys)) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
            return;
        }
    }
    //2. Idoszak
    QJsonObject jsonIdoszak = jsonObject.value("Idoszak").toObject();
    {
        QStringList requiredKeys = { "Kezdes", "Befejezes" };
        if (!CheckJSON(jsonIdoszak, requiredKeys)) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
            return;
        }
    }
    //3. Akcio
    QJsonObject jsonAkcio = jsonObject.value("Akcio").toObject();
    {
        QStringList requiredKeys = { "Nev", "Ertek" };
        if (!CheckJSON(jsonAkcio, requiredKeys)) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
            return;
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
        QStringList requiredKeys = { "Iranyitoszam" };
        if (!CheckJSON(jsonObject, requiredKeys)) {
            JSONMessage(response, 400, "[RequestManager] Error : Bad JSON - Incorrect keys in main JSON object");
            return;
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
