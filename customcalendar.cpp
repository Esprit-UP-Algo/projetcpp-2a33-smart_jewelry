#include "customcalendar.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QListView>
#include <QStringListModel>
#include<QDebug>
#include <QSqlError>
CustomCalendar::CustomCalendar()
{

}
/*QSqlQueryModel* CustomCalendar::chercher_commande( QString searchCriteria)
 {

     QSqlQueryModel* model = new QSqlQueryModel();
     QSqlQuery query;

     // Construct the SQL query to search only in the "DATE_" column
     QString queryString = "SELECT * FROM GESTION_COMMANDES WHERE date_ LIKE '%" + searchCriteria + "%'";

     model->setQuery(queryString);

     // Set column headers using column names

        model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("nom"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("prenom"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("produit"));
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("num_telephone"));
        model->setHeaderData(5,Qt::Horizontal,QObject::tr("id_produit"));
        model->setHeaderData(6,Qt::Horizontal,QObject::tr("typePaiment"));
        model->setHeaderData(7,Qt::Horizontal,QObject::tr("date_"));
     return model;
 }*/
QSqlQueryModel* CustomCalendar::chercher_commande(const QDate& searchDate)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Convert the QDate to a string in the format "dd/MM/yyyy"
    QString searchCriteria = searchDate.toString("dd/MM/yyyy");

    // Construct the SQL query to search only in the "DATE_" column
    QString queryString = "SELECT * FROM GESTION_COMMANDES WHERE DATE_ LIKE '%" + searchCriteria + "%'";
    qDebug() << "SQL Query: " << queryString;

        if (!query.exec(queryString))
        {
            qDebug() << "Query Error: " << query.lastError().text();
        }
    model->setQuery(queryString);

    // Set column headers using column names
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("produit"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("num_telephone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("id_produit"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("typePaiment"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("date_"));

    return model;
}

