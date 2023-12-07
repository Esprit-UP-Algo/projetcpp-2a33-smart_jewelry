#include "commande.h"
#include<QString>
#include <QRadioButton>
#include<QSqlQueryModel>
#include<QSqlQuery>
#include<QtDebug>
#include<QSqlDatabase>
#include<iostream>
#include<QMessageBox>
#include<QLabel>
#include<QVBoxLayout>
#include<QDialog>
#include<QtDebug>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextTableCell>
#include <QPrinter>
#include <QPrintDialog>
//#include <QAudioInput>
#include <QBuffer>
#include <QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts>
//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
//#include <QNetworkReply>
#include <QUrlQuery>





Commande::Commande()
{


}
Commande::Commande(int id, QString nom, QString prenom, QString produit, int num_telephone, int id_produit, QString typePaiment, QDate date_, int prix)
{
    this->nom =nom;
    this->prenom =prenom;
    this->produit =produit;
    this->id =id;
    this->num_telephone =num_telephone;
    this->id_produit =id_produit;
    this->typePaiment=typePaiment;
    this->date_=date_;
    this->prix=prix;

}

QString Commande::getNom(){return nom;}
QString Commande::getPrenom(){return prenom;}
QString Commande::getProduit(){return produit;}
int Commande::getId(){return id;}
int Commande::getNumeroTelephone(){return num_telephone;}
int Commande::getIdproduit(){return id_produit;}
QString Commande::getTypePaiment(){return typePaiment;}
QDate Commande::getdate(){return date_;}
int Commande::getPrix(){return prix;}
bool Commande::ajouter(const QString &typePaiment)
{

    QSqlQuery query;
    QString id_string=QString::number(id);
    QString num=QString::number(num_telephone);
    QString id_produits=QString::number(id_produit);
    QString prix_string=QString::number(prix);


    query.prepare("INSERT INTO GESTION_COMMANDES(ID,NOM,PRENOM,PRODUIT,NUM_TELEPHONE,ID_PRODUIT,TYPEPAIMENT,DATE_,PRIX)"
                  "values(:ID,:NOM,:PRENOM,:PRODUIT,:NUM_TELEPHONE,:ID_PRODUIT,:TYPEPAIMENT,:date_,:PRIX)");

    query.bindValue(":ID",id);
    query.bindValue(":NOM",nom);
    query.bindValue(":PRENOM",prenom);
    query.bindValue(":PRODUIT",produit);
    query.bindValue(":NUM_TELEPHONE",num);
    query.bindValue(":ID_PRODUIT",id_produit);
    query.bindValue(":TYPEPAIMENT", typePaiment);
    query.bindValue(":date_", date_);
    query.bindValue(":PRIX", prix);
    if (query.exec()){
        qDebug()<<"ajout effectué avec succés";
        return true;
    }else {
        qDebug()<<"erreur lors de l'ajout:"<< query.lastError().text();
        return false;
    }

}
QSqlQueryModel * Commande::afficher()
{
    QSqlQueryModel * model=new QSqlQueryModel();
           model->setQuery("select * from GESTION_COMMANDES");
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("nom"));
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("prenom"));
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("produit"));
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("num_telephone"));
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("id_produit"));
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("typePaiment"));
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("date_"));
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("prix"));
           return model;
       }
bool Commande::supprimer(int id)
{
        QSqlQuery query;
        QString res=QString::number(id);
        query.prepare("Delete from GESTION_COMMANDES where ID= :ID");
        query.bindValue(":ID",res);

        return query.exec();
}
bool Commande::modifierCommande()
{
        QSqlQuery query;

        QString id_string=QString::number((id));
        QString id_produit_string=QString::number((id_produit));
          QString num_telephone_string=QString::number((num_telephone));
          QString prix_string=QString::number(prix);



               query.prepare("UPDATE GESTION_COMMANDES SET   ID_PRODUIT=:id_produit,NOM=:nom,PRENOM=:prenom,PRODUIT=produit ,NUM_TELEPHONE=num_telephone,TYPEPAIMENT=typePaiment,DATE_=date_,PRIX=prix WHERE ID=:id");

               query.bindValue(":id", id_string);
               query.bindValue(":nom", nom);
               query.bindValue(":prenom", prenom);
               query.bindValue(":produit", produit);
               query.bindValue(":id_produit",id_produit_string );
               query.bindValue(":num", num_telephone_string);
               query.bindValue(":TYPEPAIMENT", typePaiment);
               query.bindValue(":date_", date_);
               query.bindValue(":prix", prix);

         return query.exec();

}
QSqlQueryModel* Commande::rechercher(QString test)
{
    QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery query;
        query.prepare("SELECT *  FROM GESTION_COMMANDES WHERE ID LIKE :test");
        query.bindValue(":test", "%" + test + "%");
        if (query.exec()) {
            model->setQuery(query);
            model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("nom"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("prenom"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("produit"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("num_telephone"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("id_produit"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("typePaiment"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("date_"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("prix"));

        } else {
            qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        }
        return model;
}
QSqlQueryModel* Commande::rechercherParNom(QString test)
{
    QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery query;
        query.prepare("SELECT *  FROM GESTION_COMMANDES WHERE NOM LIKE :test");
        query.bindValue(":test", "%" + test + "%");
        if (query.exec()) {
            model->setQuery(query);
            model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("nom"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("prenom"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("produit"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("num_telephone"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("id_produit"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("typePaiment"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("date_"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("prix"));

        } else {
            qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        }
        return model;
}
QSqlQueryModel* Commande::rechercherParPrenom(QString test)
{
    QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery query;
        query.prepare("SELECT *  FROM GESTION_COMMANDES WHERE PRENOM LIKE :test");
        query.bindValue(":test", "%" + test + "%");
        if (query.exec()) {
            model->setQuery(query);
            model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("nom"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("prenom"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("produit"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("num_telephone"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("id_produit"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("typePaiment"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("date_"));
               model->setHeaderData(0,Qt::Horizontal,QObject::tr("prix"));

        } else {
            qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        }
        return model;
}


QSqlQueryModel* Commande::triCommandes()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM GESTION_COMMANDES ORDER BY ID ASC");

    if (model->rowCount() == 0) {

        qDebug() << "No commands found.";
    }

    return model;
}
QSqlQueryModel* Commande::triCommandesParnom()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM GESTION_COMMANDES ORDER BY NOM ASC");

    if (model->rowCount() == 0) {

        qDebug() << "No commands found.";
    }

    return model;
}
QSqlQueryModel* Commande::triCommandesParprenom()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM GESTION_COMMANDES ORDER BY PRENOM ASC");

    if (model->rowCount() == 0) {

        qDebug() << "No commands found.";
    }

    return model;
}

bool Commande::exportToPDF(QTableView *tableView)
{
    // Create a QTextDocument
    QTextDocument doc;

    // Create a QTextCursor to append content to the document
    QTextCursor cursor(&doc);

    // Set up the QTextTable with the same number of rows and columns as the table view
    int rows = tableView->model()->rowCount();
    int columns = tableView->model()->columnCount();
    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    QTextTable *table = cursor.insertTable(rows + 1, columns, tableFormat);

    // Set headers in the first row
    for (int col = 0; col < columns; ++col) {
        QString header = tableView->model()->headerData(col, Qt::Horizontal).toString();
        table->cellAt(0, col).firstCursorPosition().insertText(header);
    }

    // Fill the table with data from the table view
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            QString data = tableView->model()->index(row, col).data().toString();
            table->cellAt(row + 1, col).firstCursorPosition().insertText(data);
        }
    }

    // Ask the user for a file name and location
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save PDF", "commande.pdf", "PDF Files (*.pdf)");

    if (fileName.isEmpty()) {
        // The user canceled the save operation
        return false;
    }

    // Set up the QPrinter
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // Print the document to the printer
    doc.print(&printer);

    return true;
}






QChartView* Commande::countenligne()
{
    int type1= 0;
     int type2=0;
     int type3=0;



       QSqlQuery query;
        query.exec("SELECT TYPEPAIMENT FROM GESTION_COMMANDES");


     while(query.next())
     {
          QString type= query.value(0).toString();
             if(type=="cheque")
             {
                 type1++;
             }
             else if(type=="enligne")
             {
                 type2++;
             }
             else
             {
                 type3++;
             }


     }
      QBarSet *set1 = new QBarSet("1");
         QBarSet *set2 = new QBarSet("2");
         QBarSet *set3 = new QBarSet("3");


         *set1 <<type1 ;
         *set2 <<NULL << type2;
         *set3 << NULL<< NULL<< type3;


         QBarSeries *series = new QBarSeries();

         series->append(set1);
         series->append(set2);
         series->append(set3);


       QChart *chart = new QChart();
         chart->addSeries(series);

         chart->setTitle("Statistique type");

         QStringList cate;
         cate << "enligne" << "carte" << "cheque";

       QBarCategoryAxis *axis = new QBarCategoryAxis();
         axis->append(cate);
         chart->createDefaultAxes();
         chart->setAxisX(axis,series);
         QChartView *chartView = new QChartView(chart);
      chartView->chart()->setAnimationOptions(QChart::AllAnimations);
         return chartView;
}

QSqlQueryModel* Commande::historique() const
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM GESTION_COMMANDES ORDER BY DATE_ DESC");
    if (query.exec()) {
        model->setQuery(query);
    }
    return model;
}

std::string Commande::chatbotFunction(const QString &userMessage) {
    QString lowerMessage = userMessage.toLower();

    if (lowerMessage.contains("bonjour")) {
        return "Salut! Comment puis-je vous aider aujourd'hui?";
    } else if (lowerMessage.contains("comment ça va?")) {
        return "Je suis un programme, donc je n'ai pas de sentiments, mais merci de demander!";
    } else if (lowerMessage.contains("quel est ton nom?")) {
        return "Je suis un chatbot créé par OpenAI.";
    } else if (lowerMessage.contains("au revoir")) {
        return "Au revoir! N'hésitez pas à revenir si vous avez d'autres questions.";
    } else {
        return "Désolé, je ne comprends pas. Pouvez-vous reformuler votre question?";
    }
}
/*QAbstractItemModel* Commande::Excel()
{
    QStandardItemModel* model = new QStandardItemModel();

    // Ajouter les en-têtes
    model->setHorizontalHeaderLabels({"id", "nom", "prenom", "produit", "num_telephone", "id_produit", "typepaiment","date_"});

    // Remplir le modèle avec des données
    for (int row = 0; row < 5; ++row) {
        QList<QStandardItem*> rowData;

        // Exemple : Ajouter des données fictives pour chaque colonne
        QStandardItem* idItem = new QStandardItem(QString::number(row + 1));  // ID_V
        QStandardItem* nomItem = new QStandardItem("01-01-2000");  // Date (exemple)
        QStandardItem* prenomItem = new QStandardItem;  // Quantité (exemple)
        QStandardItem* produitItem = new QStandardItem("mauvais");  // Type (exemple)
        QStandardItem* num_telephoneItem = new QStandardItem(QString::number(50.0 + row));  // Prix (exemple)
        QStandardItem* id_produitItem = new QStandardItem(QString::number((10 + row) * (50.0 + row)));  // Montant (exemple)
        QStandardItem* typePaimentItem = new QStandardItem;  // ID_Col (exemple)
        QStandardItem* date_Item = new QStandardItem(QString::number(row + 1));

        // Ajouter les éléments à la liste rowData
        rowData << idItem << nomItem << prenomItem << produitItem << num_telephoneItem << id_produitItem << typePaimentItem<< date_Item;

        // Ajouter la ligne au modèle
        model->appendRow(rowData);
    }

    return model;
}*/
QAbstractItemModel* Commande::Excel()
{
    QStandardItemModel* model = new QStandardItemModel();

    // Ajouter les en-têtes
    model->setHorizontalHeaderLabels({"ID", "Nom", "Prenom", "Produit", "NumTelephone", "IDProduit", "TypePaiement"});

    // Remplir le modèle avec des données
    for (int row = 0; row < commandes.size(); ++row) {
        qDebug() << "Row: " << row;
        QList<QStandardItem*> rowData;

        // Exemple : Ajouter des données fictives pour chaque colonne
        QStandardItem* idItem = new QStandardItem(QString::number(commandes[row].getId()));
        QStandardItem* nomItem = new QStandardItem(commandes[row].getNom());
        QStandardItem* prenomItem = new QStandardItem(commandes[row].getPrenom());
        QStandardItem* produitItem = new QStandardItem(commandes[row].getProduit());
        QStandardItem* numTelephoneItem = new QStandardItem(QString::number(commandes[row].getNumeroTelephone()));
        QStandardItem* idProduitItem = new QStandardItem(QString::number(commandes[row].getIdproduit()));
        QStandardItem* typePaiementItem = new QStandardItem(commandes[row].getTypePaiment());

        // Ajouter les éléments à la liste rowData
        rowData << idItem << nomItem << prenomItem << produitItem << numTelephoneItem << idProduitItem << typePaiementItem;

        // Ajouter la ligne au modèle
        model->appendRow(rowData);
    }

    return model;
}


















