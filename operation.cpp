#include "operation.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

operation::operation(int numero_operation ,  QString type , QString duree,QString etat )
{
    this->numero_operation = numero_operation;
    this->etat = etat;
    this->type = type;
    this->duree = duree;

}
operation::operation(){
    numero_operation=0;
    etat="";
    type="";
    duree="";
    confirmedItemId = -1;
}

bool operation::ajouter(){
    QSqlQuery query;
    QString numOperation = QString::number(numero_operation);

    query.prepare("insert into SERVICE_ET_OPERATION (NUMERO_OPERATION, TYPE, DUREE, ETAT) "
                              "values (:numero_operation, :type, :duree, :etat)");
    query.bindValue(":numero_operation",numOperation);
    query.bindValue(":type",type);
    query.bindValue(":duree",duree);
    query.bindValue(":etat",etat);
    return query.exec();

}
QSqlQueryModel * operation::afficher(){
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("select * from SERVICE_ET_OPERATION");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("type"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("duree"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("etat"));
    return model;
}

bool operation::supprimer(int numero_operation){
    QSqlQuery query;
    QString numero_operation1 = QString::number(numero_operation);
    query.prepare("Delete from SERVICE_ET_OPERATION where NUMERO_OPERATION = :id");
    query.bindValue(":id",numero_operation1);
    return query.exec();
}
bool operation::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE SERVICE_ET_OPERATION SET NUMERO_OPERATION = :numero_operation, TYPE = :type, DUREE = :duree, ETAT = :etat WHERE NUMERO_OPERATION = :id");
    query.bindValue(":numero_operation", numero_operation);
    query.bindValue(":id", numero_operation);
    query.bindValue(":type", type);
    query.bindValue(":duree", duree);
    query.bindValue(":etat", etat);
    bool success = query.exec();
    return success;
}

QSqlQueryModel *operation::rechercherParId(int numero_operation) {
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT * FROM SERVICE_ET_OPERATION WHERE NUMERO_OPERATION = :numero_operation");
    query.bindValue(":numero_operation", numero_operation);

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "SQL Error (rechercherParId):" << query.lastError().text();
    }

    qDebug() << "SQL Query (rechercherParId):" << query.lastQuery();

    return model;
}

QSqlQueryModel *operation::rechercherParType(const QString &type) {
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT * FROM SERVICE_ET_OPERATION WHERE UPPER(TYPE) LIKE UPPER(:type)");
    query.bindValue(":type", "%" + type + "%");

    if (query.exec()) {
        model->setQuery(query);
    }

    return model;
}
QSqlQueryModel *operation::rechercherParEtat(const QString &etat) {
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT * FROM SERVICE_ET_OPERATION WHERE UPPER(ETAT) LIKE UPPER(:etat)");
    query.bindValue(":etat", "%" + etat + "%");

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "SQL Error (rechercherParEtat):" << query.lastError().text();
    }
    qDebug() << "SQL Query (rechercherParEtat):" << query.lastQuery();
    qDebug() << "SQL Error (rechercherParEtat):" << query.lastError().text();

    return model;
}
QSqlQueryModel *operation::trierParAttribut(const QString &attribut, bool ascending) {
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    QString sortOrder = (ascending) ? "ASC" : "DESC";

    qDebug() << "Attribut:" << attribut;  // Print the attribute for debugging
    qDebug() << "Sort Order:" << sortOrder;  // Print the sort order for debugging

    if (attribut.toLower() == "numero_operation" || attribut.toLower() == "duree" || attribut.toLower() == "etat") {
        query.prepare("SELECT * FROM SERVICE_ET_OPERATION ORDER BY " + attribut.toLower() + " " + sortOrder);
    } else {
        query.prepare("SELECT * FROM SERVICE_ET_OPERATION");
    }

    if (query.exec()) {
        model->setQuery(query);
        qDebug() << "Query executed successfully.";
    } else {
        qDebug() << "SQL Error (trierParAttribut):" << query.lastError().text();
    }

    qDebug() << "SQL Query (trierParAttribut):" << query.lastQuery();  // Print the generated query

    return model;
}
QMap<QString, int> operation::getTypeStatistics() {
    QMap<QString, int> statistics;

    QSqlQuery query;
    query.prepare("SELECT TYPE, COUNT(*) FROM SERVICE_ET_OPERATION GROUP BY TYPE");
    if (query.exec()) {
        while (query.next()) {
            QString type = query.value(0).toString();
            int count = query.value(1).toInt();
            statistics[type] = count;
        }
    }

    return statistics;
}
QMap<QString, int> operation::getEtatStatistics() {
    QMap<QString, int> statistics;

    QSqlQuery query;
    query.prepare("SELECT ETAT, COUNT(*) FROM SERVICE_ET_OPERATION GROUP BY ETAT");
    if (query.exec()) {
        while (query.next()) {
            QString etat = query.value(0).toString();
            int count = query.value(1).toInt();
            statistics[etat] = count;
        }
    }

    return statistics;
}
int operation::countOperationsByEtat(const QString &etat) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM SERVICE_ET_OPERATION WHERE ETAT = :etat");
    query.bindValue(":etat", etat);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    return 0;  // Return 0 in case of an error or no results
}
