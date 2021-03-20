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

void DBServer::queryRegisterRestaurant(const QVariantMap &data, QString *message)
{
    query.clear();

    //Az input biztosan jó.
    //Ellenőrizzük hogy van-e már ilyen adat...
    QString str = "SELECT COUNT(*) FROM Etterem WHERE Email = :Email";
    query.prepare(str);
    query.bindValue(":Email", data.value("Email"));

    if (!query.exec()) {
        message = new QString("[DBServer] DB Error : " + db.lastError().text());
        return;
    }

    if (query.result()->handle().toInt() > 0) {
        message = new QString("[DBServer] Error : Data Email already exists");
        return;
    }

    //Új adat hozzáadása
    str.clear();
    str = "INSERT INTO Etterem (Email, Jelszo, Nev, Leiras, Cim) VALUES (:Email, :Jelszo, :Nev, :Leiras, :Cim)";
    cout << "[DBServer] Query : " << str.toStdString() << endl;
    query.prepare(str);

    for (const auto& key: data.keys()) {
        //cout << "[DBServer] Log : Binding " << QString(":" + key).toStdString() << " to " << data.value(key).toString().toStdString() << endl;
        query.bindValue(QString(":" + key), data.value(key));
    }

    if (!query.exec()) {
        message = new QString("[DBServer] DB Error : " + db.lastError().text());
        return;
    }

    db.commit();
    return;

}

void DBServer::queryRegisterNewRestaurant(const QVariantMap &data, QString *message)
{
    query.clear();

    //Az input biztosan jó.
    //Új adat hozzáadása
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

    //Létezik-e már

    //Etterem tábla
    QString binds, fields;
    GenerateStrings(etterem, fields, binds);

    QString str = "INSERT INTO Etterem ("+ fields +") VALUES ("+ binds +")";
    query.prepare(str);
    BindValues(query, etterem);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!query.exec()) {
        cout << "[DBServer] Error while processing query" << endl;
        *message = "[DBServer] DB Error : " + db.lastError().text();
        return;
    }
    cout << "[DBServer] Executed query : " << query.executedQuery().toStdString() << endl;

    db.commit();
    //query.clear();

    query.finish();

    str = "SELECT MAX(EtteremID) FROM Etterem";
    query.prepare(str);
    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!query.exec()) {
        *message = "[DBServer] DB Error : " + db.lastError().text();
        return;
    }

    int etteremID = query.result()->handle().toInt();
    cout << "Kapott ID: " << etteremID << endl;

    //query.clear();

    //Cim tabla
    GenerateStrings(cim, fields, binds);
    str = "INSERT INTO EtteremCim (EtteremID, "+ fields +") VALUES ("+ QString::number(etteremID) +", "+ binds +")";
    query.prepare(str);
    BindValues(query, cim);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!query.exec()) {
        *message = "[DBServer] DB Error : " + db.lastError().text();
        return;
    }

    //query.clear();


    //Nyitva tabla
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

    str = "INSERT INTO EtteremNyitvatartas VALUES ("+ QString::number(etteremID) +", ?, ?, ?, ?, ?)";
    query.prepare(str);

    query.addBindValue(napID);
    query.addBindValue(etteremZar);
    query.addBindValue(konyhaNyit);
    query.addBindValue(konyhaZar);
    query.addBindValue(etteremNyit);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;

    if (!query.execBatch()) {
        *message = "[DBServer] DB Error : " + db.lastError().text();
        return;
    }

    /*
    for (auto const& elem: nyitva) {
        if (elem.isNull())
            continue;

        QVariantMap tmp = elem.toMap();

        QString binds, fields;
        GenerateStrings(tmp, fields, binds);

        QString str = "INSERT INTO EtteremNyitvatartas VALUES ("+ QString::number(etteremID) +", "+ binds +")";
        cout << "[DBServer] Query : " << str.toStdString() << endl;
        query.prepare(str);
        BindValues(query, tmp);

        if (!query.exec()) {
            message = new QString("[DBServer] DB Error : " + db.lastError().text());
            return;
    }
    */


    //Szallit tabla

    str = "INSERT INTO Szallit VALUES ("+ QString::number(etteremID) + ", ?)";
    query.prepare(str);
    query.addBindValue(szallit);

    cout << "[DBServer] Query : " << query.lastQuery().toStdString() << endl;
    if (!query.execBatch()) {
        *message = "[DBServer] DB Error : " + db.lastError().text();
        return;
    }

    //Cimke tabla
    {
        ///TODO

    }

    //db.commit();
    return;
}



