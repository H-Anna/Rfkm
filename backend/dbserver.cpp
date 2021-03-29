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

void DBServer::GenerateUpdateString(const QVariantMap& map, QString &string)
{
    string.clear();

    for (auto const& key: map.keys()) {

        if (map.value(key).isNull())
            continue;

        string += " " + key+" = :" + key;
        if(map.lastKey()!=key){
            string+=", ";
        }
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
        cout << query.executedQuery().toStdString() << endl;
        cout << message->toStdString() << endl;

        //Ha meg van engedve a rollback, próbáljuk meg; ha nem sikerül, hibaüzenet
        if (!db.rollback())
            message->append("\n[DBServer] Rollback Failed");
        else
            message->append("\n[DBServer] Rollback Successful");
    }

    return queryExec;
}

QPair<QString, int> DBServer::UserExists(QSqlQuery &query, QString *message, const QStringList &auth)
{

    QString str = QString("SELECT ID, Tablanev FROM (") +
            "SELECT FutarID AS ID, Email, Jelszo, 'Futar' AS Tablanev FROM Futar" +
            " UNION " +
            "SELECT EtteremID AS ID, Email, Jelszo, 'Etterem' AS Tablanev FROM Etterem" +
            " UNION " +
            "SELECT RVendegID AS ID, Email, Jelszo, 'RegisztraltVendeg' AS Tablanev FROM RegisztraltVendeg" +
            ") WHERE Email='"+ auth.at(0) +"' AND Jelszo='"+ auth.at(1) +"'";

    query.prepare(str);
    if (!ExecuteQuery(query, message)) {
        return QPair<QString, int>();
    }
    query.next();
    int id = query.value(0).toInt();
    QString keresettTabla = query.value(1).toString();

    if (id <= 0)
        return QPair<QString, int>();

    return QPair<QString, int>(keresettTabla, id);
}


bool DBServer::insertFoodOpenHours(const QVariantMap &idoszak, QString *message, QString &fields, QString &binds){
    GenerateStrings(idoszak, fields, binds);

    QString str = "INSERT INTO EtelIdoszak ("+ fields +") VALUES ("+ binds +")";
    query.prepare(str);
    BindValues(query, idoszak);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message)){
        return false;
    }
    return true;
}

bool DBServer::insertFoodTags(const QVariantList &cimke, QString *message, int etelID){
    QString str = "INSERT INTO EtelCimke VALUES ("+ QString::number(etelID) +", ?)";
    query.prepare(str);
    query.addBindValue(cimke);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message, true)){
        return false;
    }
    return true;
}

bool DBServer::insertRestaurantDelivers(const QVariantList &szallit, QString *message, int etteremID){
    QString str = "INSERT INTO EtteremSzallit VALUES ("+ QString::number(etteremID) + ", ?)";
    query.prepare(str);
    query.addBindValue(szallit);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message, true)){
        return false;
    }
    return true;
}

bool DBServer::insertRestaurantTags(const QVariantList &cimke, QString *message, int etteremID){
    QString str = "INSERT INTO EtteremCimke VALUES ("+ QString::number(etteremID) +", ?)";
    query.prepare(str);
    query.addBindValue(cimke);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message, true)){
        return false;
    }
    return true;
}

bool DBServer::insertRestaurantOpenHours(const QVariantList &nyitva, QString *message, int etteremID){
    QVariantList napID, konyhaNyit, etteremNyit, etteremZar, konyhaZar;

    for (auto const& elem: nyitva) {
        if (elem.isNull())
            continue;

        auto tmp = elem.toMap();

        if (tmp.value("KonyhaNyit").toUInt() == 0 && tmp.value("KonyhaZar").toUInt() == 0
                && tmp.value("EtteremNyit").toUInt() == 0 && tmp.value("EtteremZar").toUInt() == 0) {
            continue;
        }

        napID << tmp.value("NapID");
        konyhaNyit << tmp.value("KonyhaNyit");
        etteremNyit << tmp.value("EtteremNyit");
        etteremZar << tmp.value("EtteremZar");
        konyhaZar << tmp.value("KonyhaZar");
    }

    //Ha végig null értékek voltak, akkor nem tart nyitva az étterem, tehát nem kell lefuttatni a query-t
    if (!napID.empty()) {

        QString str = "INSERT INTO EtteremNyitvatartas VALUES ("+ QString::number(etteremID) +", ?, ?, ?, ?, ?)";
        query.prepare(str);

        query.addBindValue(napID);
        query.addBindValue(etteremZar);
        query.addBindValue(konyhaNyit);
        query.addBindValue(konyhaZar);
        query.addBindValue(etteremNyit);

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

        if (!ExecuteQuery(query, message, true))
            return false;
    }
    return true;
}

bool DBServer::insertUserAddress(const QVariantMap &cim, QString *message, QString &fields, QString &binds){
    GenerateStrings(cim, fields, binds);
    QString str = "INSERT INTO VendegCim ("+ fields +") VALUES ("+ binds +")";
    query.prepare(str);
    BindValues(query, cim);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message)){
        return false;
    }
    return true;
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

    QStringList auth = {data.value("Email").toString(), data.value("Jelszo").toString()};
    if (UserExists(query, message, auth).first.length() != 0) {
        *message = "[DBServer] Error : User with credentials already exists";
        return;
    }

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
    etterem.insert("Irsz", cim.value("Irsz"));
    etterem.insert("Kozterulet", cim.value("Kozterulet"));
    etterem.insert("Hazszam", cim.value("Hazszam"));
    etterem.insert("Emelet_ajto", cim.value("Emelet_ajto"));

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
    query.prepare("SELECT MAX(EtteremID) FROM Etterem");
    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!ExecuteQuery(query, message))
        return;

    query.next();
    int etteremID = query.value(0).toInt();
    cout << "Kapott ID: " << etteremID << endl;


    //----Nyitva tábla---
    //Ez egy tömb ami azonos objektumokat tartalmaz, egyszerűbb így bind-olni az értékeket
    if(!insertRestaurantOpenHours(nyitva, message, etteremID)){
        return;
    }


    //----Szallit tábla---
    //Ha a Szallit mező null vagy üres tömb, akkor a szallit QVariantList üres lesz
    if (!szallit.empty()) {
        if (!insertRestaurantDelivers(szallit, message, etteremID))
            return;
    }

    //----Cimke tábla---
    if (!cimke.empty()) {
        if (!insertRestaurantTags(cimke, message, etteremID))
            return;
    }

    //Végül az elindított tranzakciót befejezzük
    db.commit();

    return;
}

void DBServer::queryRegisterUser(const QVariantMap &data, QString *message)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;

    //Biztos ami biztos
    query.clear();

    QStringList auth = {data.value("Email").toString(), data.value("Jelszo").toString()};
    if (UserExists(query, message, auth).first.length() != 0) {
        *message = "[DBServer] Error : User with credentials already exists";
        return;
    }

    //Az inputot leellenőrizte a RequestManager, tehát az biztosan jó.

    //Adatok formázása - később egyszerűbb lesz velük dolgozni
    QVariantMap vendeg;
    vendeg.insert("Email", data.value("Email"));
    vendeg.insert("Jelszo", data.value("Jelszo"));
    vendeg.insert("VNev", data.value("VNev"));
    vendeg.insert("KNev", data.value("KNev"));

    //----Vendeg tábla----
    QString str = QString("INSERT INTO Vendeg (Telefonszam) VALUES ('") + data.value("Telefonszam").toString() +"')";
    query.prepare(str);
    if (!ExecuteQuery(query, message))
        return;

    query.prepare("SELECT MAX(VendegID) FROM Vendeg");
    if (!ExecuteQuery(query, message))
        return;
    query.next();
    vendeg.insert("RVendegID",query.value(0).toUInt());

    //----RegisztraltVendeg tábla---
    QString binds, fields;
    GenerateStrings(vendeg, fields, binds);
    str = "INSERT INTO RegisztraltVendeg ("+ fields +") VALUES ("+ binds +")";
    query.prepare(str);
    BindValues(query, vendeg);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!ExecuteQuery(query, message))
        return;

   //----VendegCim tábla---
    QVariantMap cim = data.value("Cim").toMap();
    cim.insert("VendegID", vendeg.value("RVendegID"));

    if (!insertUserAddress(cim, message, fields, binds))
        return;

    //Végül az elindított tranzakciót befejezzük
    db.commit();

    return;
}

void DBServer::queryLogin(const QStringList &auth, QString *message, QString *queryMsg)
{
    auto user = UserExists(query, message, auth);

    if (user.first.length() == 0) {
        *message = "[DBServer] Error : Auth data matches no existing users";
        return;
    }

    QJsonObject queryResult;
    queryResult.insert("Tipus", user.first);
    queryResult.insert("ID", user.second);

    *queryMsg = QJsonDocument(queryResult).toJson(QJsonDocument::Compact).toStdString().c_str();
}


void DBServer::queryCreateFood(const QVariantMap &data, QString *message)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;
    query.clear();

    //Az inputot leellenőrizte a RequestManager, tehát az biztosan jó.
    //Ettől függetlenül még lehetnek null értékek, ezeket mindig ki kell szűrni mielőtt a query
    //elindul.

    QVariantList cimke = data.value("Cimke").toList();
    QVariantMap idoszak = data.value("Idoszak").toMap();
    QVariantMap etel;
    QString binds, fields;

    //Adatok formázása - később egyszerűbb lesz velük dolgozni
    etel.insert("Nev", data.value("Nev"));
    etel.insert("Ar", data.value("Ar"));
    etel.insert("Kep", data.value("Kep"));
    etel.insert("Leiras", data.value("Leiras"));
    etel.insert("AkcioID", data.value("AkcioID"));
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

    if (!idoszak.value("Kezdes").isNull()) {
        idoszak.insert("EtelID", etelID);

        if (!insertFoodOpenHours(idoszak, message, fields, binds)){
            return;
        }
    }

    //----Cimke tábla---
    if (!cimke.empty()) {
        if (!insertFoodTags(cimke, message, etelID))
            return;
    }

    //Végül az elindított tranzakciót befejezzük
    db.commit();
    return;
}

void DBServer::queryCreateFoodTag(const QVariantMap &data, QString *message)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;

    query.clear();

    QVariantMap cimke;
    cimke.insert("Nev", data.value("Nev"));
    cimke.insert("TipusID", 2);

    //----Cimke tábla----

    QString binds, fields;
    GenerateStrings(cimke, fields, binds);

    QString str = "INSERT INTO Cimke ("+ fields +") VALUES ("+ binds +")";
    query.prepare(str);
    BindValues(query, cimke);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

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

    //----Etel tábla----

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
        str = "SELECT Nev FROM EtelCimke AS ect JOIN Cimke ON ect.CimkeID=Cimke.CimkeID WHERE EtelID=:EtelID";
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

    //----EtteremCimke tábla----

    QVariantList etteremCimke = data.value("EtteremCimke").toList();
    QVariantList checkEtteremCimke;
    if(!etteremCimke.isEmpty()){
        QString str = "SELECT EtteremID FROM EtteremCimke WHERE CimkeID IN ( "+etteremCimke.at(0).toString();
        for (int idx=1; idx<etteremCimke.length(); idx++){
            str.append(", "+etteremCimke.at(idx).toString()+" ");
        }
        str.append(") GROUP BY EtteremID HAVING COUNT(DISTINCT CimkeID)=:id");

        query.prepare(str);
        query.bindValue(QString(":id"), etteremCimke.length());

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

        if (!ExecuteQuery(query, message))
            return;

        while (query.next())
        {
            checkEtteremCimke.push_back(query.value(0).toDouble());
        }
    }

    //----EtelCimke tábla----

    QVariantList etelCimke = data.value("EtelCimke").toList();
    QVariantList checkEtelCimke;
    if(!etelCimke.isEmpty()){
        QString str = "SELECT EtteremID FROM Etel JOIN EtelCimke AS ec ON Etel.EtelID=ec.EtelID WHERE CimkeID IN ( "+etelCimke.at(0).toString();
        for (int idx=1; idx<etelCimke.length(); idx++){
            str.append(", "+etelCimke.at(idx).toString()+" ");
        }
        str.append(") GROUP BY EtteremID HAVING COUNT(DISTINCT CimkeID)=:id");

        query.prepare(str);
        query.bindValue(QString(":id"), etelCimke.length());

        cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

        if (!ExecuteQuery(query, message))
            return;

        while (query.next())
        {
            checkEtelCimke.push_back(query.value(0).toDouble());
        }
    }

    //----Etterem tábla----

    QVariantMap etterem;
    etterem.insert("Irsz", data.value("Irsz"));
    etterem.insert("Nev", "%"+data.value("Nev").toString()+"%");

    QString str = "SELECT * FROM Etterem JOIN EtteremSzallit as esz ON Etterem.EtteremID=esz.EtteremID";
    if(!data.value("Irsz").isNull()){
        str.append(" WHERE TeruletID = :Irsz");
        if(!data.value("Nev").isNull()){
            str.append(" AND UPPER(Etterem.Nev) LIKE UPPER(:Nev)");
        }
    }
    else if(!data.value("Nev").isNull()){
        str.append(" WHERE UPPER(Etterem.Nev) LIKE UPPER(:Nev)");
    }

    query.prepare(str);
    BindValues(query, etterem);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    QJsonArray jsonArray;
    while (query.next())
    {
        int etteremID=query.value(0).toDouble();
        //csak akkor adja vissza az adott éttermet, ha az étterem cimkéi és az étel cimkéi is benne vannak az adott halmazokban
        if((!etteremCimke.isEmpty() && !checkEtteremCimke.contains(etteremID)) ||
            (!etelCimke.isEmpty() && !checkEtelCimke.contains(etteremID))){
            continue;
        }
        QJsonObject jsonObject;

        jsonObject.insert("EtteremID", etteremID);
        jsonObject.insert("Nev", query.value(3).toString());
        jsonObject.insert("Leiras", query.value(4).toString());
        jsonObject.insert("Kep", query.value(5).toString());
        jsonObject.insert("Szallitasi_ktsg", query.value(6).toDouble());
        QJsonObject cimObject;
        cimObject.insert("Irsz", query.value(7).toDouble());
        cimObject.insert("Kozterulet", query.value(8).toString());
        cimObject.insert("Hazszam", query.value(9).toString());
        cimObject.insert("Emelet_ajto", query.value(10).toString());
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
            nyitvaObject.insert("KonyhaNyit", query.value(2).toDouble());
            nyitvaObject.insert("KonyhaZar", query.value(3).toDouble());
            nyitvaObject.insert("EtteremNyit", query.value(4).toDouble());
            nyitvaObject.insert("EtteremZar", query.value(5).toDouble());
            nyitvaArray.push_back(nyitvaObject);
        }

        //--Cimke--
        QJsonArray cimkeArray;
        str = "SELECT Nev FROM EtteremCimke AS ec JOIN Cimke ON ec.CimkeID=Cimke.CimkeID WHERE EtteremID=:EtteremID";
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

void DBServer::queryListRestaurantTag(QString *message, QString *QueryMessage)
{
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;
    query.clear();

    QString binds, fields;
    QJsonArray jsonArray;

    //----Cimke tábla----

    QString str = "SELECT * FROM Cimke JOIN CimkeTipus AS ct ON Cimke.TipusID=ct.TipusID WHERE ct.TipusID=3 OR ct.TipusID=4";
    query.prepare(str);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    while (query.next())
    {
        QJsonObject jsonObject;

        jsonObject.insert("CimkeID", query.value(0).toDouble());
        jsonObject.insert("Nev", query.value(1).toString());
        jsonObject.insert("Tipus", query.value(4).toString());

        jsonArray.push_back(jsonObject);
    }

    QJsonObject finalJsonObject;
    finalJsonObject.insert("EtteremCimke", jsonArray);
    QString strFromObj = QJsonDocument(finalJsonObject).toJson(QJsonDocument::Compact).toStdString().c_str();
    *QueryMessage=strFromObj;

    db.commit();
    return;
}

void DBServer::queryListFoodTag(QString *message, QString *QueryMessage)
{
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;
    query.clear();

    QString binds, fields;
    QJsonArray jsonArray;

    //----Cimke tábla----

    QString str = "SELECT * FROM Cimke JOIN CimkeTipus AS ct ON Cimke.TipusID=ct.TipusID WHERE ct.TipusID=1 OR ct.TipusID=2";
    query.prepare(str);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    while (query.next())
    {
        QJsonObject jsonObject;

        jsonObject.insert("CimkeID", query.value(0).toDouble());
        jsonObject.insert("Nev", query.value(1).toString());
        jsonObject.insert("Tipus", query.value(4).toString());

        jsonArray.push_back(jsonObject);
    }

    QJsonObject finalJsonObject;
    finalJsonObject.insert("EtelCimke", jsonArray);
    QString strFromObj = QJsonDocument(finalJsonObject).toJson(QJsonDocument::Compact).toStdString().c_str();
    *QueryMessage=strFromObj;

    db.commit();
    return;
}


void DBServer::queryUpdateFood(const QVariantMap &data, QString *message, QString *QueryMessage)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;

    query.clear();

    //Az inputot leellenőrizte a RequestManager, tehát az biztosan jó.
    //Ettől függetlenül még lehetnek null értékek, ezeket mindig ki kell szűrni mielőtt a query
    //elindul.

    QVariantList cimke = data.value("Cimke").toList();
    QVariantMap idoszak = data.value("Idoszak").toMap();
    QVariantMap etel;
    QString string, binds, fields;

    //Adatok formázása - később egyszerűbb lesz velük dolgozni
    etel.insert("Nev", data.value("Nev"));
    etel.insert("Ar", data.value("Ar"));
    etel.insert("Kep", data.value("Kep"));
    etel.insert("Leiras", data.value("Leiras"));
    etel.insert("AkcioID", data.value("AkcioID"));

    //----Etel tábla----

    GenerateUpdateString(etel, string);

    QString str = "UPDATE Etel SET"+string+" WHERE EtelID = :EtelID;";
    etel.insert("EtelID", data.value("EtelID"));
    query.prepare(str);
    BindValues(query, etel);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    //----Idoszak tábla---

    str = "DELETE FROM EtelIdoszak WHERE EtelID=:EtelID;";
    query.prepare(str);
    query.bindValue(QString(":EtelID"), data.value("EtelID"));

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    if (!idoszak.empty()) {
        idoszak.insert("EtelID",data.value("EtelID"));

        if (!insertFoodOpenHours(idoszak, message, fields, binds)){
            return;
        }
    }

    //----Cimke tábla---

    str = "DELETE FROM EtelCimke WHERE EtelID=:EtelID;";
    query.prepare(str);
    query.bindValue(QString(":EtelID"), data.value("EtelID"));

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    if (!cimke.empty()) {
        if (!insertFoodTags(cimke, message, data.value("EtelID").toInt()))
            return;
    }

    //Végül az elindított tranzakciót befejezzük
    db.commit();
    return;
}

void DBServer::queryUpdateRestaurantBasics(const QVariantMap &data, QString *message)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;

    query.clear();

    //Adatok formázása - később egyszerűbb lesz velük dolgozni
    QVariantMap etterem;
    etterem.insert("Email", data.value("Email"));
    etterem.insert("Jelszo", data.value("Jelszo"));
    etterem.insert("Nev", data.value("Nev"));
    etterem.insert("Leiras", data.value("Leiras"));
    etterem.insert("Kep", data.value("Kep"));
    etterem.insert("Szallitasi_ktsg", data.value("Szallitasi_ktsg"));

    QVariantList szallit = data.value("Szallit").toList();
    QVariantList cimke = data.value("Cimke").toList();
    QString string;

    //----Etterem tábla----

    GenerateUpdateString(etterem, string);

    QString str = "UPDATE Etterem SET"+string+" WHERE EtteremID = :EtteremID;";
    etterem.insert("EtteremID", data.value("EtteremID"));
    query.prepare(str);
    BindValues(query, etterem);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;


    //----Szallit tábla---

    str = "DELETE FROM EtteremSzallit WHERE EtteremID=:EtteremID;";
    query.prepare(str);
    query.bindValue(QString(":EtteremID"), data.value("EtteremID"));

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    if (!szallit.empty()) {
        if (!insertRestaurantDelivers(szallit, message, data.value("EtteremID").toInt()))
            return;
    }

    //----Cimke tábla---

    str = "DELETE FROM EtteremCimke WHERE EtteremID=:EtteremID;";
    query.prepare(str);
    query.bindValue(QString(":EtteremID"), data.value("EtteremID"));

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    if (!cimke.empty()) {
        if (!insertRestaurantTags(cimke, message, data.value("EtteremID").toInt()))
            return;
    }

    //Végül az elindított tranzakciót befejezzük
    db.commit();

    return;
}

void DBServer::queryUpdateRestaurantAddress(const QVariantMap &data, QString *message)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;

    query.clear();

    //Adatok formázása - később egyszerűbb lesz velük dolgozni
    QVariantMap etterem;
    etterem.insert("Irsz", data.value("Irsz"));
    etterem.insert("Kozterulet", data.value("Kozterulet"));
    etterem.insert("Hazszam", data.value("Hazszam"));
    etterem.insert("Emelet_ajto", data.value("Emelet_ajto"));

    QString string;

    //----Etterem tábla----

    GenerateUpdateString(etterem, string);

    QString str = "UPDATE Etterem SET"+string+" WHERE EtteremID = :EtteremID;";
    etterem.insert("EtteremID", data.value("EtteremID"));
    query.prepare(str);
    BindValues(query, etterem);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    //Végül az elindított tranzakciót befejezzük
    db.commit();

    return;
}

void DBServer::querySetOpenHours(const QVariantMap &data, QString *message)
{
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;

    query.clear();

    //Ellenőrizzük hogy van-e ilyen ID-jű rekord
    int etteremID = data.value("EtteremID").toUInt();
    QVariantList nyitva = data.value("Nyitvatartas").toList();

    QVariantList napID, konyhaNyit, etteremNyit, etteremZar, konyhaZar;

    for (auto const& elem: nyitva) {
        if (elem.isNull())
            continue;

        auto tmp = elem.toMap();

        if (tmp.value("KonyhaNyit").toUInt() == 0 && tmp.value("KonyhaZar").toUInt() == 0
                && tmp.value("EtteremNyit").toUInt() == 0 && tmp.value("EtteremZar").toUInt() == 0) {
            continue;
        }

        napID << tmp.value("NapID");
        konyhaNyit << tmp.value("KonyhaNyit");
        etteremNyit << tmp.value("EtteremNyit");
        etteremZar << tmp.value("EtteremZar");
        konyhaZar << tmp.value("KonyhaZar");
    }


    //Ha már létezik ilyen ID-vel étterem a táblában, azokat a rekordokat töröljük
    query.prepare("DELETE FROM EtteremNyitvatartas WHERE EtteremID="+QString::number(etteremID));
    if (!ExecuteQuery(query, message))
        return;

    //Ha végig null értékek voltak, akkor nem tart nyitva az étterem, tehát nem kell lefuttatni a query-t
    if (!napID.empty()) {

        query.clear();
        QString str = "INSERT INTO EtteremNyitvatartas VALUES ("+ QString::number(etteremID) +", ?, ?, ?, ?, ?)";
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

    db.commit();
}

void DBServer::queryUpdateUser(const QVariantMap &data, QString *message)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;

    query.clear();

    QVariantMap vendeg;
    vendeg.insert("VNev", data.value("VNev"));
    vendeg.insert("KNev", data.value("KNev"));
    QString fields, binds, string;

    //----Vendeg tábla----

    QString str = QString("UPDATE Vendeg SET Telefonszam = ") + data.value("Telefonszam").toString() +" WHERE VendegID=:VendegID";
    query.prepare(str);
    query.bindValue(QString(":VendegID"), data.value("VendegID"));
    if (!ExecuteQuery(query, message))
        return;

    //----RegisztraltVendeg tábla---

    GenerateUpdateString(vendeg, string);

    str = "UPDATE RegisztraltVendeg SET"+string+" WHERE RVendegID = :RVendegID;";
    vendeg.insert("RVendegID", data.value("VendegID"));
    query.prepare(str);
    BindValues(query, vendeg);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

   //----VendegCim tábla---

    str = "DELETE FROM VendegCim WHERE VendegID=:VendegID;";
    query.prepare(str);
    query.bindValue(QString(":VendegID"), data.value("VendegID"));

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    QVariantMap cim = data.value("Cim").toMap();
    cim.insert("VendegID", vendeg.value("RVendegID"));

    if (!insertUserAddress(cim, message, fields, binds))
        return;

    //Végül az elindított tranzakciót befejezzük
    db.commit();


    return;
}


void DBServer::queryDeleteFood(const QVariantMap &data, QString *message)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;
    query.clear();

    //----Etel tábla----

    QString str = "DELETE FROM Etel WHERE EtelID=:EtelID;";
    query.prepare(str);
    query.bindValue(QString(":EtelID"), data.value("EtelID"));

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    //Végül az elindított tranzakciót befejezzük
    db.commit();
    return;
}

void DBServer::queryDeleteFoodTag(const QVariantMap &data, QString *message)
{
    //Kötelezően indítunk egy tranzakciót. Ha ez nincs, akkor hiányosan kerülhetnek be az adatok
    if (!db.transaction()) {
        *message = "[DBServer] Can't open transaction - stopped";
        return;
    }
    cout << "[DBServer] Transaction started" << endl;
    query.clear();

    //----Cimke tábla----

    QString str = "DELETE FROM Cimke WHERE CimkeID=:CimkeID;";
    query.prepare(str);
    query.bindValue(QString(":CimkeID"), data.value("CimkeID"));

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    //Végül az elindított tranzakciót befejezzük
    db.commit();
    return;
}





