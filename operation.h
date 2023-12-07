#ifndef OPERATION_H
#define OPERATION_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QObject>
#include <QMessageBox>


class operation
{
private:
    int numero_operation;
    QString type;
    QString duree;
    QString etat;
    int confirmedItemId;

public:
    operation();
    operation(int numero_operation  , QString type , QString duree, QString etat );
    //Getters
    int getNumero_operation(){return numero_operation;}
    QString getEtat(){return etat;}
    QString getType(){return type;}
    QString getDuree(){return duree;}
    //Setters
    void setNumero_operation(int numero_operation1){numero_operation = numero_operation1;}
    void setEtat(QString etat1){etat = etat1;}
    void setType(QString type1){type = type1;}
    void setDuree(QString duree1){duree = duree1;}



    //fonctionnalites
    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel* rechercherParId(int numero_operation);
    QSqlQueryModel *rechercherParType(const QString &type);
    QSqlQueryModel *rechercherParEtat(const QString &etat);
    QSqlQueryModel *trierParAttribut(const QString &attribut, bool ascending);
    QMap<QString, int> getTypeStatistics();
    QMap<QString, int> getEtatStatistics();
    int countOperationsByEtat(const QString &etat);

    bool isOperationReady(int itemId);
    void setConfirmedItemId(int confirmedItemId);
    void updateWeight(int itemId, float weight);
};

#endif // OPERATION_H
