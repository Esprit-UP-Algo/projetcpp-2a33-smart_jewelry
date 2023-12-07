#include "client.h"
#include <QDate>
#include <QSqlQuery>
#include <QtDebug>
#include <QSqlQueryModel>
#include <QSqlError>
#include <string>
#include <QPdfWriter>
#include <QPainter>
//constrecteur pardefaut
client::client()
{
     ID=0;
     NOM="";
     PRENOM="";
     NUMERO="";
     ADRESSE="";
}
//constrecteur parametre
client::client(int ID,QString NOM,QString PRENOM,QString NUMERO,QString ADRESSE)
{
     this->ID=ID;
     this->NOM=NOM;
       this->PRENOM=PRENOM;
     this->NUMERO=NUMERO;
    this->ADRESSE=ADRESSE;

}

void client::setID(int ID){this->ID=ID;}
void client::setNOM(QString NOM){this->NOM=NOM;}
void client::setPRENOM(QString PRENOM){this->PRENOM=PRENOM;}
void client::setNUMERO( QString NUMERO){this->NUMERO=NUMERO;}
void client::setADRESSE( QString ADRESSE){this->ADRESSE=ADRESSE;}

bool client::ajouter()
{
    QSqlQuery query;
    QString ID_string=QString::number(ID);


         query.prepare("INSERT INTO GEST_CLIENT (ID,NOM,PRENOM,NUMERO,ADRESSE) "
                       "VALUES (:ID,:NOM,:PRENOM,:NUMERO,:ADRESSE)");
         query.bindValue(":ID",ID_string);
         query.bindValue(":NOM",NOM);
         query.bindValue(":PRENOM",PRENOM);
         query.bindValue(":NUMERO",NUMERO);
         query.bindValue(":ADRESSE",ADRESSE);


         return query.exec();

}
QSqlQueryModel* client::afficher()
{
    QSqlQueryModel* model=new QSqlQueryModel();


          model->setQuery("SELECT* FROM GEST_CLIENT");
          model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
          model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
          model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
          model->setHeaderData(3, Qt::Horizontal, QObject::tr("NUMERO"));
          model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));





    return model;
};
bool client::supprimer(int ID){
    QSqlQuery query;
    QString res = QString::number(ID);
    query.prepare("DELETE FROM GEST_CLIENT WHERE ID=:ID");
    query.bindValue(":ID", res);
    if (!query.exec()) {
        return false;  // Failed to execute the query
    }
    if (query.numRowsAffected() == 0) {
        return false;  // No rows were affected
    }
    return true;
}


bool client::modifier(int ID,QString NOM,QString PRENOM,QString NUMERO,QString ADRESSE)
{
    QSqlQuery query;
       query.prepare("SELECT COUNT(*) FROM GEST_CLIENT WHERE ID = :ID");
         query.bindValue(":ID", ID);
         query.exec();
         query.next();
         int count = query.value(0).toInt();
         if (count == 0) {
             return false;
         }

         QString ID_string = QString::number(ID);
         query.prepare("UPDATE GEST_CLIENT SET NOM = COALESCE(:NOM, NOM), PRENOM = COALESCE(:PRENOM, PRENOM),NUMERO = COALESCE(:NUMERO, NUMERO),ADRESSE = COALESCE(:ADRESSE, ADRESSE) WHERE ID = :ID");
         query.bindValue(":ID", ID_string);
         query.bindValue(":NOM", NOM.isEmpty() ? QVariant(QVariant::String) : NOM);
         query.bindValue(":PRENOM", PRENOM.isEmpty() ? QVariant(QVariant::String) : PRENOM);
         query.bindValue(":NUMERO", NUMERO.isEmpty() ? QVariant(QVariant::String) : NUMERO);
         query.bindValue(":ADRESSE", ADRESSE.isEmpty() ? QVariant(QVariant::String) : ADRESSE);


         return query.exec();
}
void client::recherche(QTableView * affiche_tab ,int ID)
{
    QSqlQueryModel *model= new QSqlQueryModel();
    QString ID_string=QString::number(ID);



    QSqlQuery *query=new QSqlQuery;
    query->prepare("select * from GEST_CLIENT where ID like '%"+ID_string+"%';");


    query->exec();
    model->setQuery(*query);
    affiche_tab->setModel(model);
    affiche_tab->show();




}

QSqlQueryModel* client::trier()
{
    QSqlQueryModel * model=new QSqlQueryModel();
    model->setQuery("select * from GEST_CLIENT ORDER BY ID");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NUMERO"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));

    return model;
}
void  client::telechargerPDF(){

                     QPdfWriter pdf("C:\\Users\\ASUS\\Documents\\gestion_client\\import.pdf");
                     QPainter painter(&pdf);
                    int i = 4000;
                         painter.setPen(Qt::blue);
                         painter.setFont(QFont("Arial", 30));
                         painter.drawText(1100,1200,"GESTION client");
                         painter.setPen(Qt::black);
                         painter.setFont(QFont("Arial",14));
                         painter.drawRect(100,100,7300,2600);
                         painter.drawRect(0,3000,9600,500);
                         painter.setFont(QFont("Arial",8));
                         painter.drawText(300,3300,"ID");
                         painter.drawText(1300,3300,"NOM");
                         painter.drawText(2200,3300,"PRENOM");
                         painter.drawText(3200,3300,"NUMERO");
                         painter.drawText(4200,3300,"ADRESSE");



                         QSqlQuery query;
                         query.prepare("select * from GEST_CLIENT");
                         query.exec();
                         while (query.next())
                         {
                             painter.drawText(300,i,query.value(0).toString());
                             painter.drawText(1300,i,query.value(1).toString());
                             painter.drawText(2300,i,query.value(2).toString());
                             painter.drawText(3300,i,query.value(3).toString());
                             painter.drawText(4300,i,query.value(4).toString());



                            i = i + 500;
                         }}
bool  client::doesIDExist(int id) {


        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM GEST_CLIENT WHERE id = :id");
        query.bindValue(":id", id);

        if (query.exec() && query.next()) {
            return query.value(0).toInt() > 0; // If count is greater than 0, the ID exists
        }

        // Handle the case where the query execution fails or doesn't return a result
        return false;

}
QString client::getadresse_b(int id) {
    QSqlQuery query;
    query.prepare("SELECT adresse FROM GEST_CLIENT WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        QString adresse = query.value("adresse").toString();
        return adresse;
    }

    // Return an appropriate value when no data is found
    // You might want to throw an exception, log an error, or return a specific default value
    return QString(); // Empty QString as the default value
}
int client::statclient(QString adresse)
{
    QSqlQuery query;
    query.prepare("select count(*) from GEST_CLIENT where adresse=:adresse ");
    query.bindValue(":adresse",adresse);
    query.exec();
    query.next();
    return query.value(0).toInt();

}
