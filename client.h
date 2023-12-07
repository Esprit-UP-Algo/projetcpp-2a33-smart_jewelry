#ifndef CLIENT_H
#define CLIENT_H
#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QTableView>

class client
{
public:
    client();
    client(int,QString,QString,QString,QString);
    int getID(){return ID ;}
    QString getNOM(){return NOM ;}
         QString getPRENOM(){return PRENOM ;}
         QString getNUMERO(){return NUMERO ;}
         QString getADRESSE(){return ADRESSE ;}

         void setID(int);
         void setNOM(QString);
         void setPRENOM(QString);
         void setNUMERO(QString);
         void setADRESSE(QString);

         bool ajouter();
         QSqlQueryModel* afficher();
         bool supprimer(int);
         bool modifier(int,QString,QString,QString,QString);

         void recherche(QTableView * affiche_tab, int);

         QSqlQueryModel * trier();
         void telechargerPDF();
         bool  doesIDExist(int id);
         QString getadresse_b(int id);

         int statclient(QString);


private:
         int ID;
         QString NOM,PRENOM,NUMERO,ADRESSE;

};

#endif // CLIENT_H
