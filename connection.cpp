#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
/*
db.setDatabaseName("Source_Projet2A");
db.setUserName("system");//inserer nom de l'utilisateur
db.setPassword("yahya");//inserer mot de passe de cet utilisateur
*/
db.setDatabaseName("mariem");
db.setUserName("mariem");//inserer nom de l'utilisateur
db.setPassword("0000");//inserer mot de passe de cet utilisateur
if (db.open())
test=true;
return  test;

}
void Connection::closeConnection(){db.close();}
