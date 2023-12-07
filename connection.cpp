#include "connection.h"
#include <QSqlError>
#include <QDebug>
Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A");
db.setUserName("system");//inserer nom de l'utilisateur
db.setPassword("yahya");//inserer mot de passe de cet utilisateur

if (db.open()) {
        qDebug() << "Connected to the database!";
        test = true;
    } else {
        qDebug() << "Error opening the database: " << db.lastError().text();
    }


        return  test;
}
void Connection::closeConnection(){db.close();}
