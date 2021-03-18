#include "dbserver.h"

#include <iostream>

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

void DBServer::queryRegisterRestaurant(const QVariantMap &data, QString *message)
{
    query.clear();

    QString str = "INSERT INTO Etterem (Email, Jelszo, Nev, Leiras, Cim) VALUES (:Email, :Jelszo, :Nev, :Leiras, :Cim)";
    cout << "[DBServer] Query : " << str.toStdString() << endl;
    query.prepare(str);

    for (const auto& key: data.keys()) {
        cout << "[DBServer] Log : Binding " << QString(":" + key).toStdString() << " to " << data.value(key).toString().toStdString() << endl;
        query.bindValue(QString(":" + key), data.value(key));
    }

    if (!query.exec()) {
        message = new QString("[DBServer] DB Error : " + db.lastError().text());
        return;
    }

    db.commit();
    return;

}



