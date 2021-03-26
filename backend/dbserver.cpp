#include "dbserver.h"

#include <iostream>

#include <QSqlResult>

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

QPair<QString, int> DBServer::UserExists(QSqlQuery &query, QString *message, const QStringList &auth, const QStringList &tables)
{
    int id = -1;
    QString keresettTabla;


    if (tables.contains("Etterem")) {

        query.prepare("SELECT EtteremID FROM Etterem WHERE Email='"+ auth.at(0) +"' AND Jelszo='"+ auth.at(1) +"'");
        if (ExecuteQuery(query, message) && query.next()) {
            id = query.value(0).toInt();
            keresettTabla = "Etterem";
        }
    }

    if (id == -1 && tables.contains("RegisztraltVendeg")) {

        query.prepare("SELECT RVendegID FROM RegisztraltVendeg WHERE Email='"+ auth.at(0) +"' AND Jelszo='"+ auth.at(1) +"'");
        if (ExecuteQuery(query, message) && query.next()) {
            id = query.value(0).toInt();
            keresettTabla = "RegisztraltVendeg";
        }

    }
    if (id == -1 && tables.contains("Futar")) {

        query.prepare("SELECT FutarID FROM Futar WHERE Email='"+ auth.at(0) +"' AND Jelszo='"+ auth.at(1) +"'");
        if (ExecuteQuery(query, message) && query.next()) {
            id = query.value(0).toInt();
            keresettTabla = "Futar";
        }
    }

    return QPair(keresettTabla, id);
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
    //Ettől függetlenül még lehetnek null értékek, ezeket mindig ki kell szűrni mielőtt a query
    //elindul.

    //Adatok formázása - később egyszerűbb lesz velük dolgozni
    QVariantMap vendeg;
    vendeg.insert("Email", data.value("Email"));
    vendeg.insert("Jelszo", data.value("Jelszo"));
    vendeg.insert("VNev", data.value("VNev"));
    vendeg.insert("KNev", data.value("KNev"));
    vendeg.insert("Telefonszam", data.value("Telefonszam"));

    QVariantMap cim = data.value("Cim").toMap();

    //----Etterem tábla----

    //QVariantMap-ek query-be dobálásához lesz jó a GenerateStrings és a BindValues függvény
    QString binds, fields;
    GenerateStrings(vendeg, fields, binds);

    QString str = "INSERT INTO RegisztraltVendeg ("+ fields +") VALUES ("+ binds +")";
    query.prepare(str);
    BindValues(query, vendeg);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!ExecuteQuery(query, message))
        return;

    //----Index megtalálása----
    //A legújabb rekord indexe a legmagasabb
    str = "SELECT MAX(RVendegID) FROM RegisztraltVendeg";
    query.prepare(str);
    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!ExecuteQuery(query, message))
        return;

    query.next();
    int vendegID = query.value(0).toInt();
    cout << "Kapott ID: " << vendegID << endl;


   //----Cim tábla---
    GenerateStrings(cim, fields, binds);
    str = "INSERT INTO RegVendegCim (VendegID, "+ fields +") VALUES ("+ QString::number(vendegID) +", "+ binds +")";
    query.prepare(str);
    BindValues(query, cim);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!ExecuteQuery(query, message))
        return;


    //Végül az elindított tranzakciót befejezzük
    db.commit();


    return;

}

void DBServer::queryLogin(const QStringList &auth, QString *message)
{
    auto user = UserExists(query, message, auth);

    if (user.first.length() == 0) {
        *message = "[DBServer] Error : Auth data matches no existing users";
    } else {
        *message = "[DBServer] Log: User found in table '"+ user.first +"'.";
    }

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
        napID << tmp.value("NapID");
        konyhaNyit << tmp.value("KonyhaNyit");
        etteremNyit << tmp.value("EtteremNyit");
        etteremZar << tmp.value("EtteremZar");
        konyhaZar << tmp.value("KonyhaZar");
    }

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

