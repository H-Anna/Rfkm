#include "dbserver.h"

#include <iostream>

#include <QSqlResult>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

using namespace std;

DBServer::DBServer(const QString& driver, QString connectionName, const QString& dbName):
    db(QSqlDatabase::addDatabase(driver, connectionName)),
    dbName(dbName),
    query(db)
{

}

bool DBServer::OpenCon() {
    db.setDatabaseName(dbName);
    if (!db.open())
    {
        cout << "[DBServer] DB Error : " << db.lastError().text().toStdString() << endl;
        return false;
    }
    cout << "[DBServer] Log : Opened DB" << db.lastError().text().toStdString() << endl;
    return true;
}

void DBServer::GenerateStrings(const QVariantMap& map, QString &fields, QString &binds)
{
    fields.clear();
    binds.clear();

    bool flag = false;

    for (auto const& key: map.keys()) {

        if (map.value(key).isNull())
            continue;

        if (flag) {
            binds += ", ";
            fields += ", ";
        } else {
            flag = true;
        }
        binds += ":" + key;
        fields += key;
    }
}

void DBServer::BindValues(QSqlQuery &query, const QVariantMap &map)
{
    for (const auto& key: map.keys()) {
        if (!query.lastQuery().contains(key))
            continue;

        query.bindValue(QString(":" + key), map.value(key));
    }
}

bool DBServer::ExecuteQuery(QSqlQuery &query, QString *message, bool batch)
{
    //Milyen típusú exec függvényt kell indítani?
    bool queryExec;
    if (batch)
        queryExec = query.execBatch();
    else
        queryExec = query.exec();

    //Ha nem sikerül
    if (!queryExec) {
        *message = "[DBServer] DB Error : " + db.lastError().text();

        //Ha meg van engedve a rollback, próbáljuk meg; ha nem sikerül, hibaüzenet
        if (!db.rollback())
            message->append("\n[DBServer] Rollback Failed");
        else
            message->append("\n[DBServer] Rollback Successful");
    }

    return queryExec;
}

void DBServer::queryRegisterRestaurant(const QVariantMap &data, QString *message)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;

    //Biztos ami biztos
    query.clear();

    //Az inputot leellenőrizte a RequestManager, tehát az biztosan jó.
    //Ettől függetlenül még lehetnek null értékek, ezeket mindig ki kell szűrni mielőtt a query
    //elindul.

    //Adatok formázása - később egyszerűbb lesz velük dolgozni
    QVariantMap etterem;
    etterem.insert("Email", data.value("Email"));
    etterem.insert("Jelszo", data.value("Jelszo"));
    etterem.insert("Nev", data.value("Nev"));
    etterem.insert("Leiras", data.value("Leiras"));
    etterem.insert("Kep", data.value("Kep"));
    etterem.insert("Szallitasi_ktsg", data.value("Szallitasi_ktsg"));

    QVariantMap cim = data.value("Cim").toMap();
    QVariantList nyitva = data.value("Nyitvatartas").toList();
    QVariantList szallit = data.value("Szallit").toList();
    QVariantList cimke = data.value("Cimke").toList();

    //----Etterem tábla----

    //QVariantMap-ek query-be dobálásához lesz jó a GenerateStrings és a BindValues függvény
    QString binds, fields;
    GenerateStrings(etterem, fields, binds);

    QString str = "INSERT INTO Etterem ("+ fields +") VALUES ("+ binds +")";
    query.prepare(str);
    BindValues(query, etterem);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    //----Index megtalálása----
    //A legújabb rekord indexe a legmagasabb
    str = "SELECT MAX(EtteremID) FROM Etterem";
    query.prepare(str);
    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!ExecuteQuery(query, message))
        return;

    query.next();
    int etteremID = query.value(0).toInt();
    cout << "Kapott ID: " << etteremID << endl;


   //----Cim tábla---
    GenerateStrings(cim, fields, binds);
    str = "INSERT INTO EtteremCim (EtteremID, "+ fields +") VALUES ("+ QString::number(etteremID) +", "+ binds +")";
    query.prepare(str);
    BindValues(query, cim);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!ExecuteQuery(query, message))
        return;


    //----Nyitva tábla---
    //Ez egy tömb ami azonos objektumokat tartalmaz, egyszerűbb így bind-olni az értékeket



    QVariantList napID, konyhaNyit, etteremNyit, etteremZar, konyhaZar;

    for (auto const& elem: nyitva) {
        if (elem.isNull())
            continue;

        auto tmp = elem.toMap();
        napID << tmp.value("NapID");
        konyhaNyit << tmp.value("KonyhaNyit");
        etteremNyit << tmp.value("EtteremNyit");
        etteremZar << tmp.value("EtteremZar");
        konyhaZar << tmp.value("KonyhaZar");
    }

    //Ha végig null értékek voltak, akkor nem tart nyitva az étterem, tehát nem kell lefuttatni a query-t
    if (!napID.empty()) {

        str = "INSERT INTO EtteremNyitvatartas VALUES ("+ QString::number(etteremID) +", ?, ?, ?, ?, ?)";
        query.prepare(str);

        query.addBindValue(napID);
        query.addBindValue(etteremZar);
        query.addBindValue(konyhaNyit);
        query.addBindValue(konyhaZar);
        query.addBindValue(etteremNyit);

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

        if (!ExecuteQuery(query, message, true))
            return;
    }


    //----Szallit tábla---
    //Ha a Szallit mező null vagy üres tömb, akkor a szallit QVariantList üres lesz
    if (!szallit.empty()) {

        str = "INSERT INTO Szallit VALUES ("+ QString::number(etteremID) + ", ?)";
        query.prepare(str);
        query.addBindValue(szallit);

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
        if (!ExecuteQuery(query, message, true))
            return;

    }


    //----Cimke tábla---
    if (!cimke.empty()) {
        ///TODO
    }

    //Végül az elindított tranzakciót befejezzük
    db.commit();


    return;
}

void DBServer::queryCreateFood(const QVariantMap &data, QString *message)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;

    //Biztos ami biztos
    query.clear();

    //Az inputot leellenőrizte a RequestManager, tehát az biztosan jó.
    //Ettől függetlenül még lehetnek null értékek, ezeket mindig ki kell szűrni mielőtt a query
    //elindul.

    QVariantMap akcio = data.value("Akcio").toMap();
    QVariantList cimke = data.value("Cimke").toList();
    QVariantMap idoszak = data.value("Idoszak").toMap();

    QVariantMap etel;
    QString binds, fields;
    int akcioID;
    //----Akcio tábla----

    if(!akcio.value("Nev").isNull()){
        QString str = "SELECT AkcioID FROM Akcio WHERE Nev = :Nev  AND Ertek = :Ertek";
        query.prepare(str);
        BindValues(query, akcio);

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
        if (!ExecuteQuery(query, message))
            return;

        if(query.next()){
            akcioID = query.value(0).toInt();
            etel.insert("AkcioID", akcioID);
            cout << "Kapott AkcioID: " << akcioID << endl;
        }
    }

    //Adatok formázása - később egyszerűbb lesz velük dolgozni
    etel.insert("Nev", data.value("Nev"));
    etel.insert("Ar", data.value("Ar"));
    etel.insert("Kep", data.value("Kep"));
    etel.insert("Leiras", data.value("Leiras"));
    etel.insert("EtteremID", data.value("EtteremID"));

    //----Etel tábla----

    //QVariantMap-ek query-be dobálásához lesz jó a GenerateStrings és a BindValues függvény
    GenerateStrings(etel, fields, binds);

    QString str = "INSERT INTO Etel ("+ fields +") VALUES ("+ binds +")";
    query.prepare(str);
    BindValues(query, etel);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    //----Index megtalálása----
    //A legújabb rekord indexe a legmagasabb
    str = "SELECT MAX(EtelID) FROM Etel";
    query.prepare(str);
    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!ExecuteQuery(query, message))
        return;

    query.next();
    int etelID = query.value(0).toInt();
    cout << "Kapott ID: " << etelID << endl;

    //----Idoszak tábla---
    if (!idoszak.empty()) {
        idoszak.insert("EtelID", etelID);
        GenerateStrings(idoszak, fields, binds);

        QString str = "INSERT INTO EtelIdoszak ("+ fields +") VALUES ("+ binds +")";
        query.prepare(str);
        BindValues(query, idoszak);

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

        if (!ExecuteQuery(query, message))
            return;
    }

    //----Cimke tábla---
    QVariantList cimkeID;
    if (!cimke.empty()) {
        for (auto const& elem: cimke) {
            QString str = "SELECT CimkeID FROM Cimke WHERE Nev = :Nev";
            query.prepare(str);
            query.bindValue(QString(":Nev"), elem);

            cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

            if (!ExecuteQuery(query, message))
                return;

            if(query.next()){
                cimkeID << query.value(0).toInt();
                cout << "Kapott CimkeID: " << query.value(0).toInt() << endl;
            }
        }

        if (!cimkeID.empty()) {
            str = "INSERT INTO EtelCimkeTulajdonos VALUES ("+ QString::number(etelID) +", ?)";
            query.prepare(str);
            query.addBindValue(cimkeID);

            cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

            if (!ExecuteQuery(query, message, true))
                return;
        }

    }

    //Végül az elindított tranzakciót befejezzük
    db.commit();
    return;
}

void DBServer::queryListFood(const QVariantMap &data, QString *message, QString *QueryMessage)
{
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;
    query.clear();

    QString binds, fields;

    //----Etel tábla----

    //SELECT * FROM  Etel LEFT JOIN Akcio ON Etel.AkcioID=Akcio.AkcioID LEFT JOIN EtelIdoszak as ei ON Etel.EtelID=ei.EtelIDs
    QString str = "SELECT * FROM Etel WHERE EtteremID =:EtteremID";
    query.prepare(str);
    query.bindValue(QString(":EtteremID"), data.value("EtteremID"));

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    QJsonArray jsonArray;
    while (query.next())
    {
        QJsonObject jsonObject;

       jsonObject.insert("EtelID", query.value(0).toDouble());
       jsonObject.insert("Akcio", query.value(1).toDouble());
       jsonObject.insert("Nev", query.value(2).toString());
       jsonObject.insert("Ar", query.value(3).toDouble());
       jsonObject.insert("Kep", query.value(4).toString());
       jsonObject.insert("Leiras", query.value(5).toString());

       jsonArray.push_back(jsonObject);
    }

    int idx=0;
    for (auto const& elem: jsonArray) {
        QJsonObject object=elem.toObject();

        //--Akcio--
        int akcioValue=object.value("Akcio").toDouble();
        QJsonObject akcioObject;
        //Ha van kedvezménye az ételnek, akkor nem nulla lesz az érték
        if(akcioValue!=0){
            str = "SELECT * FROM Akcio WHERE AkcioID = :AkcioID";
            query.prepare(str);
            query.bindValue(QString(":AkcioID"), akcioValue);

            cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

            if (!ExecuteQuery(query, message))
                return;

            if(query.next()){
                akcioObject.insert("Nev", query.value(0).toString());
                akcioObject.insert("Ertek", query.value(1).toDouble());
            }
        }
        else{
            akcioObject.insert("Nev", QJsonValue::Null);
            akcioObject.insert("Ertek", QJsonValue::Null);
        }

        //--Idoszak--
        int etelID=object.value("EtelID").toDouble();
        QJsonObject idoszakObject;
        str = "SELECT * FROM EtelIdoszak WHERE EtelID = :EtelID";
        query.prepare(str);
        query.bindValue(QString(":EtelID"), etelID);

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

        if (!ExecuteQuery(query, message))
            return;

        if(query.next()){
            idoszakObject.insert("Kezdes", query.value(1).toDouble());
            idoszakObject.insert("Befejezes", query.value(2).toDouble());
        }
        else{
            idoszakObject.insert("Kezdes", QJsonValue::Null);
            idoszakObject.insert("Befejezes", QJsonValue::Null);
        }

        //--Cimke--
        QJsonArray cimkeArray;
        str = "SELECT Nev FROM  EtelCimkeTulajdonos AS ect JOIN Cimke ON ect.CimkeID=Cimke.CimkeID WHERE EtelID=:EtelID";
        query.prepare(str);
        query.bindValue(QString(":EtelID"), etelID);

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

        if (!ExecuteQuery(query, message))
            return;

        while(query.next()){
            cimkeArray.push_back(query.value(0).toString());
        }

        //Újra belerakjuk a tömbbe
        object.insert("Akcio", akcioObject);
        object.insert("Idoszak", idoszakObject);
        object.insert("Cimke", cimkeArray);
        jsonArray.replace(idx, object);
        idx++;
    }

    QJsonObject finalJsonObject;
    finalJsonObject.insert("Etelek", jsonArray);
    QString strFromObj = QJsonDocument(finalJsonObject).toJson(QJsonDocument::Compact).toStdString().c_str();
    *QueryMessage=strFromObj;


    db.commit();
    return;
}

void DBServer::queryListRestaurant(const QVariantMap &data, QString *message, QString *QueryMessage)
{
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;
    query.clear();

    QString binds, fields;

    //----Etterem tábla----

    QString str = "SELECT * FROM Etterem JOIN Szallit ON Etterem.EtteremID=Szallit.EtteremID JOIN EtteremCim ON EtteremCim.EtteremID=Etterem.EtteremID WHERE TeruletID=:Iranyitoszam";
    query.prepare(str);
    query.bindValue(QString(":Iranyitoszam"), data.value("Iranyitoszam"));

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    QJsonArray jsonArray;
    while (query.next())
    {
        QJsonObject jsonObject;

       jsonObject.insert("EtteremID", query.value(0).toDouble());
       jsonObject.insert("Nev", query.value(3).toString());
       jsonObject.insert("Leiras", query.value(4).toString());
       jsonObject.insert("Kep", query.value(5).toString());
       jsonObject.insert("Szallitasi_ktsg", query.value(6).toDouble());
       QJsonObject cimObject;
       cimObject.insert("Irsz", query.value(10).toDouble());
       cimObject.insert("Kozterulet", query.value(11).toString());
       cimObject.insert("Hazszam", query.value(12).toString());
       cimObject.insert("Emelet_ajto", query.value(13).toString());
       jsonObject.insert("Cim", cimObject);

       jsonArray.push_back(jsonObject);
    }

    int idx=0;
    for (auto const& elem: jsonArray) {
        QJsonObject object=elem.toObject();

        //--Nyitvatartas--
        QJsonArray nyitvaArray;
        str = "SELECT * FROM EtteremNyitvatartas WHERE EtteremID = :EtteremID";
        query.prepare(str);
        query.bindValue(QString(":EtteremID"), object.value("EtteremID").toDouble());

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

        if (!ExecuteQuery(query, message))
            return;

        while(query.next()){
            QJsonObject nyitvaObject;
            nyitvaObject.insert("NapID", query.value(1).toDouble());
            nyitvaObject.insert("EtteremZar", query.value(2).toDouble());
            nyitvaObject.insert("KonyhaNyit", query.value(3).toDouble());
            nyitvaObject.insert("KonyhaZar", query.value(4).toDouble());
            nyitvaObject.insert("EtteremNyit", query.value(5).toDouble());
            nyitvaArray.push_back(nyitvaObject);
        }

        //--Cimke--
        QJsonArray cimkeArray;
        str = "SELECT Nev FROM EtteremCimkeTulajdonos AS ect JOIN Cimke ON ect.CimkeID=Cimke.CimkeID WHERE EtteremID=:EtteremID";
        query.prepare(str);
        query.bindValue(QString(":EtteremID"), object.value("EtteremID").toDouble());

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

        if (!ExecuteQuery(query, message))
            return;

        while(query.next()){
            cimkeArray.push_back(query.value(0).toString());
        }

        //Újra belerakjuk a tömbbe
        object.insert("Nyitvatartas", nyitvaArray);
        object.insert("Cimke", cimkeArray);
        jsonArray.replace(idx, object);
        idx++;
    }

    QJsonObject finalJsonObject;
    finalJsonObject.insert("Ettermek", jsonArray);
    QString strFromObj = QJsonDocument(finalJsonObject).toJson(QJsonDocument::Compact).toStdString().c_str();
    *QueryMessage=strFromObj;


    db.commit();
    return;
}




