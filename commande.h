#ifndef COMMANDE_H
#define COMMANDE_H
#include<QString>
#include <QRadioButton>
#include<QSqlQueryModel>
#include<QSqlQuery>
#include<QtSql>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextTableCell>
#include <QFileDialog>
#include <QTableView>
#include <QHeaderView>
#include <QDebug>
#include <QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts>

#include <QTextToSpeech>
#include <QAudioInput>
#include <QBuffer>

class Commande
{
private:

    QString nom,prenom,produit;
    int id;
    int num_telephone;
    int id_produit;
     QString typePaiment;
     QDate date_;
     int prix;



public:
    Commande();
    Commande(int, QString, QString, QString ,  int , int, QString,QDate,int );

    QString getNom();
    QString getPrenom();
    QString getProduit();
    QString getTypePaiment();
    int getId();
    int getNumeroTelephone();
    int getIdproduit();
    QDate getdate();
    int getPrix();


    void setNom(QString n);
    void setPrenom(QString p);
    void setProduit(QString pr);
    void setId(int id);
    void setNumeroTelephone(int num);
    void setIdproduit(int id_produit);
    void setTypePaiment(QString pa);
    void setdate(QDate date);
     void setPrix(int prix);

    bool ajouter(const QString &typePaiment);
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifierCommande();
    QSqlQueryModel *rechercher(QString test);
    QSqlQueryModel* rechercherParNom( QString test);
    QSqlQueryModel* rechercherParPrenom( QString test);
    static QSqlQueryModel *triCommandes();
    QSqlQueryModel * triCommandesParprenom();
    QSqlQueryModel * triCommandesParnom();

    bool exportToPDF(QTableView *tableView);


    void setTypePaiment(const QString &type);

    QChartView* countenligne();
    int countcarte();
    int countcheque();
    QSqlQueryModel* historique() const;
    std::string chatbotFunction(const QString &userMessage);
    QAbstractItemModel* Excel();




private:
    QList<Commande> commandes;
    QTextToSpeech *speech;
};
#endif // COMMANDE_H
