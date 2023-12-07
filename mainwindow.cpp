
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "produit.h"
#include "arduino.h"
#include <QMessageBox>
#include <QVector>
#include <QList>
#include <QStatusBar>
#include <QLabel>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QMap>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QInputDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QFile>
#include <QTimer>
#include "commande.h"

//integration client

#include "client.h"
#include "client_log.h"
#include "qrcode.h"
#include <vector>
#include <string>
#include <QBuffer>
#include <cstdlib>
#include <ctime>
#include <QSqlError>
#include<QDebug>
#include<QImage>
#include <QPixmap>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include<QTextDocument>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QtPrintSupport/QAbstractPrintDialog>
#include <QtPrintSupport/QPrinterInfo>
#include <QPrintDialog>
#include <QGraphicsView>
#include <QTabWidget>

#include <QSqlQuery>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int ret=arduino.connect_arduino(); // lancer la connexion à arduino
        switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< arduino.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<arduino.getarduino_port_name();
           break;
        case(-1):qDebug() << "arduino is not available";
        }


   ui->setupUi(this);




   QStatusBar *statusBar = QMainWindow::statusBar();
        setStatusBar(statusBar);


        stockIconLabel = new QLabel(this);
        statusBar->addWidget(stockIconLabel);
        setCentralWidget(ui->centralWidget);
        statusBar->setFixedHeight(40);
         ui->mainToolBar->addWidget(statusBar);



        connect(ui->le_id_rech, &QLineEdit::textChanged, this, &MainWindow::on_le_id_rech_textChanged);
        connect(ui->pushButtonGenererPDF, &QPushButton::clicked, this, &MainWindow::genererPDF);

//connect(ui->pushButtonStatsCategorie, &QPushButton::clicked, this, &MainWindow::afficherStatsParCategorie);
        connect(ui->pushButtonStatsCategorie, &QPushButton::clicked, this, &MainWindow::afficherStatsParCategorie);
        connect(ui->pushButtonAfficherAlertes, SIGNAL(clicked()), this, SLOT(afficherAlertes()));
        connect(ui->pushButtonAjouterStock, SIGNAL(clicked()), this, SLOT(on_pushButtonAjouterStock_clicked()));




        int initialStockLevel = 10;
        updateStockIcon(initialStockLevel);


        QLabel *notificationLabel;
            //int notificationCount;
        int tp = Produit::getLowStockProductCount();
        notificationLabel = new QLabel("++"+QString(tp), this);
        ui->mainToolBar->addWidget(notificationLabel);

    // Initialize the system tray icon
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(QIcon(":/icons/icon.png")); // Set the path to your notification icon

        // Initialize the tray menu
        trayMenu = new QMenu(this);
        QAction *showAction = new QAction("Show", this);
       // connect(showAction, &QAction::triggered, this);
        trayMenu->addAction(showAction);

        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();

    Produit* P = new Produit(); // Création d'une instance de Produit
    ui->tab_produit->setModel(P->afficher());
    //ui->notif_stock->setModel(P->afficher());

    //integration client

    client f;
    ui->affiche_tab->setModel(f.afficher());

}

MainWindow::~MainWindow()
{
    delete ui;
    ui->tableView_2->setModel(etmp.afficher());
}

void MainWindow::updateStockIcon(int stockLevel)
{
    // Set the stock icon based on the stock level
    // You can customize this logic based on your requirements
    if (stockLevel > 5)
        stockIconLabel->setPixmap(QPixmap(":/icons/icon.png"));
    else
        stockIconLabel->setPixmap(QPixmap(":/icons/icon.png"));
}

void MainWindow::on_pb_ajouter_clicked()
{
    int id = ui->le_id->text().toInt();
    QString nom = ui->le_nom->text();
    float prix = ui->le_prix->text().toFloat();

    QString selectedCategory = ui->comboBoxCategory->currentText();

    int quantite = ui->le_quantite->text().toInt();
    Produit::Category category = Produit::stringToCategory(selectedCategory);
    Produit P(id, nom, prix, category, quantite);
    QString ajout_produit = P.ajouter();
    if (ajout_produit == "NEW_PRODUCT")
    {
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("ajout avec succes.\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
        ui->tab_produit->setModel(P.afficher());
        ui->tab_produit->setModel(P.getSortedModel());

    }
    else if (ajout_produit == "PRODUCT_EXISTS")
    {
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("Le produit existe.\n"
                                             "Modification de quentité avec succes."),
                                 QMessageBox::Cancel);
        ui->tab_produit->setModel(P.afficher());
    }
    else if (P.getCategory() == Produit::Undefined)
    {
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                              QObject::tr("Veuillez spécifier une catégorie avant d'ajouter le produit."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_pb_supprimer_clicked()
{
    Produit P;
    P.setid(ui->lineEdit->text().toInt());

    bool test = P.supprimer(P.getid());

    QMessageBox msgBox;

    if (test)
    {
        msgBox.setText("Suppression avec succès.");
        ui->tab_produit->setModel(P.afficher());
    }
    else
    {
        msgBox.setText("Echec de suppression");
    }
    msgBox.exec();
}

void MainWindow::on_pb_modifier_clicked()
{
    int id = ui->le_id->text().toInt();
    QString nom = ui->le_nom->text();
    float prix = ui->le_prix->text().toFloat();
    QString selectedCategory = ui->comboBoxCategory->currentText();
    int quantite = ui->le_quantite->text().toInt();
    Produit::Category category = Produit::stringToCategory(selectedCategory);

    Produit P(id, nom, prix, category,quantite);
    bool test = P.modifier();
    ui->tab_produit->setModel(P.getSortedModel());
    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("Update effectué\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
        ui->tab_produit->setModel(P.afficher());
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                              QObject::tr("Update non effectué\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_comboBoxCategory_activated(const QString &arg1)
{
    QString selectedCategory = arg1;
        Produit::Category category;
        int tp = Produit::getLowStockProductCount();

        // Convertir la chaîne de caractères en une valeur de l'énumération Category
        if (selectedCategory == "Collier")
        {
            category = Produit::Collier;
        }
        else if (selectedCategory == "Boucles")
        {
            category = Produit::Boucles;
        }
        else if (selectedCategory == "Anneau")
        {
            category = Produit::Anneau;
        }
        else if (selectedCategory == "Bracelet")
        {
            category = Produit::Bracelet;
        }
        else
        {
            // Catégorie non valide
            return;
        }

        QList<Produit::Category> selectedCategories;
        selectedCategories.append(category);

      //  ui->tab_produit->setModel(P->get_FilteredProducts(selectedCategories));

}




void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    QList<Produit::Category> selectedCategories;

    if (ui->checkBox_4->isChecked())
        selectedCategories.append(Produit::Collier);
    if (ui->checkBox_3->isChecked())
        selectedCategories.append(Produit::Boucles);
    if (ui->checkBox_2->isChecked())
        selectedCategories.append(Produit::Anneau);
    if (ui->checkBox->isChecked())
        selectedCategories.append(Produit::Bracelet);

    ui->tab_produit->setModel(P.get_FilteredProducts(selectedCategories));


}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    QList<Produit::Category> selectedCategories;

    if (ui->checkBox_4->isChecked())
        selectedCategories.append(Produit::Collier);
    if (ui->checkBox_3->isChecked())
        selectedCategories.append(Produit::Boucles);
    if (ui->checkBox_2->isChecked())
        selectedCategories.append(Produit::Anneau);
    if (ui->checkBox->isChecked())
        selectedCategories.append(Produit::Bracelet);
    ui->tab_produit->setModel(P.get_FilteredProducts(selectedCategories));
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    QList<Produit::Category> selectedCategories;

    if (ui->checkBox_4->isChecked())
        selectedCategories.append(Produit::Collier);
    if (ui->checkBox_3->isChecked())
        selectedCategories.append(Produit::Boucles);
    if (ui->checkBox_2->isChecked())
        selectedCategories.append(Produit::Anneau);
    if (ui->checkBox->isChecked())
        selectedCategories.append(Produit::Bracelet);
    ui->tab_produit->setModel(P.get_FilteredProducts(selectedCategories));
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    QList<Produit::Category> selectedCategories;

    if (ui->checkBox_4->isChecked())
        selectedCategories.append(Produit::Collier);
    if (ui->checkBox_3->isChecked())
        selectedCategories.append(Produit::Boucles);
    if (ui->checkBox_2->isChecked())
        selectedCategories.append(Produit::Anneau);
    if (ui->checkBox->isChecked())
        selectedCategories.append(Produit::Bracelet);
    ui->tab_produit->setModel(P.get_FilteredProducts(selectedCategories));
}












/*void MainWindow::on_le_id_rech_textChanged(const QString &text)
{
    // Appelé chaque fois que le texte dans le QLineEdit le_id_rech change
    effectuerRechercheParId(text);
}*/


void MainWindow::on_le_id_rech_textChanged(const QString &arg1)
{
    effectuerRechercheParId(arg1);
}



void MainWindow::effectuerRechercheParId(const QString &id)
{
    // Vérifiez si le texte de l'ID est un entier valide
    bool isInt;
    int idInt = id.toInt(&isInt);

    if (isInt)
    {
        // Effectuez la recherche par ID
        Produit produitRecherche = P.rechercherParId(idInt);

        // Mettez à jour le modèle de la QTableView tab_rech avec le résultat de la recherche
        QStandardItemModel *model = new QStandardItemModel(1, 5, this);
        model->setHorizontalHeaderItem(0, new QStandardItem("Identifiant"));
        model->setHorizontalHeaderItem(1, new QStandardItem("Nom"));
        model->setHorizontalHeaderItem(2, new QStandardItem("Prix"));
        model->setHorizontalHeaderItem(3, new QStandardItem("Catégorie"));
        model->setHorizontalHeaderItem(4, new QStandardItem("Quantité"));

        model->setItem(0, 0, new QStandardItem(QString::number(produitRecherche.getid())));
        model->setItem(0, 1, new QStandardItem(produitRecherche.getnom()));
        model->setItem(0, 2, new QStandardItem(QString::number(produitRecherche.getprix())));
        model->setItem(0, 3, new QStandardItem(Produit::getCategoryString(produitRecherche.getCategory())));
        model->setItem(0, 4, new QStandardItem(QString::number(produitRecherche.getquantite())));

        ui->tab_rech->setModel(model);
    }
    else
    {
        // L'ID n'est pas un entier valide, vous pouvez afficher un message d'erreur si nécessaire
    }
}


void MainWindow::genererPDF()
{
    QString nomFichier = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "Fichiers PDF (*.pdf)");

    if (!nomFichier.isEmpty())
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(nomFichier);

        QPainter painter(&printer);
        ui->tab_produit->render(&painter);
    }
}

void MainWindow::afficherStatsParCategorie()
{
    QMap<Produit::Category, int> stats = P.getStatsParCategorie();
    int totalProducts = 0;

    // Calculer le nombre total de produits
    for (auto it = stats.constBegin(); it != stats.constEnd(); ++it)
    {
        totalProducts += it.value();
    }

    // Créer un modèle de données pour le QTableView
    QStandardItemModel *model = new QStandardItemModel(stats.size(), 3, this);
    model->setHorizontalHeaderItem(0, new QStandardItem("Catégorie"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Nombre"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Pourcentage"));

    // Remplir le modèle avec le nombre et le pourcentage
    int row = 0;
    for (auto it = stats.constBegin(); it != stats.constEnd(); ++it)
    {
        QStandardItem *categoryItem = new QStandardItem(Produit::getCategoryString(it.key()));
        QStandardItem *countItem = new QStandardItem(QString::number(it.value()));

        // Calculer le pourcentage
        double percentage = (static_cast<double>(it.value()) / totalProducts) * 100.0;
        QStandardItem *percentageItem = new QStandardItem(QString::number(percentage, 'f', 2) + "%");

        model->setItem(row, 0, categoryItem);
        model->setItem(row, 1, countItem);
        model->setItem(row, 2, percentageItem);

        ++row;
    }

    // Appliquer le modèle au QTableView
    ui->tab_stat->setModel(model);

    // Facultatif : ajuster la largeur des colonnes pour afficher correctement les données
    ui->tab_stat->resizeColumnsToContents();

    // Facultatif : désactiver le tri sur les colonnes
    ui->tab_stat->setSortingEnabled(false);

}

void MainWindow::afficherAlertes()
{
    // Obtenez la liste des produits en alerte depuis la classe Produit
    QList<Produit> alertes = P.getProduitsEnAlerte();

    // Créez un modèle de données pour les alertes
    QStandardItemModel *alertesModel = new QStandardItemModel(alertes.size(), 5, this);

    // Configurez les en-têtes de colonnes
    alertesModel->setHorizontalHeaderLabels(QStringList() << "ID" << "Nom" << "Prix" << "Catégorie" << "Quantité");

    // Remplissez le modèle avec les données des produits en alerte
    for (int i = 0; i < alertes.size(); ++i) {
        alertesModel->setItem(i, 0, new QStandardItem(QString::number(alertes[i].getid())));
        alertesModel->setItem(i, 1, new QStandardItem(alertes[i].getnom()));
        alertesModel->setItem(i, 2, new QStandardItem(QString::number(alertes[i].getprix())));
        alertesModel->setItem(i, 3, new QStandardItem(Produit::getCategoryString(alertes[i].getCategory())));
        alertesModel->setItem(i, 4, new QStandardItem(QString::number(alertes[i].getquantite())));
    }

    // Affichez le modèle dans le QTableView tab_alertes
    ui->tab_alertes->setModel(alertesModel);
}

/*void MainWindow::on_pushButtonAfficherAlertes_clicked()
{
    afficherAlertes();
}*/
// MainWindow.cpp

/*void MainWindow::on_pushButtonAfficherAlertes_clicked()
{

    afficherAlertes();
    int alerteCount = Produit::getLowStockProductCount();

    if (alerteCount < 3) {
        // Son bref pour moins de 3 alertes
        arduino.write_to_arduino("1");
    } else if (alerteCount < 6) {
        // Son plus aigu pour 3 à 5 alertes
        arduino.write_to_arduino("2");
    } else {
        // Son encore plus aigu pour 6 alertes ou plus
        arduino.write_to_arduino("3");
    }


}*/
void MainWindow::on_pushButtonAfficherAlertes_clicked()
{
    afficherAlertes();
    int alerteCount = Produit::getLowStockProductCount();

    if (alerteCount < 3) {
        // Son bref et LED allumée pour moins de 3 alertes
        arduino.write_to_arduino("1");
        //arduino.write_to_arduino("L");  // Envoie une commande pour allumer la LED
    } else if (alerteCount < 6) {
        // Son plus aigu et LED clignotante pour 3 à 5 alertes
        arduino.write_to_arduino("2");
       // arduino.write_to_arduino("C");  // Envoie une commande pour activer le clignotement de la LED
    } else {
        // Son encore plus aigu et LED clignotante rapide pour 6 alertes ou plus
        arduino.write_to_arduino("3");
       // arduino.write_to_arduino("F");  // Envoie une commande pour activer le clignotement rapide de la LED
    }

    // Utilisation de QTimer pour désactiver la LED après un certain temps (par exemple, 5 secondes)
    QTimer::singleShot(5000, this, [this]() {
        arduino.write_to_arduino("0");  // Envoie une commande pour éteindre la LED
    });
}






void MainWindow::ajouterEnStock(int idProduit, int quantiteAAjouter)
{
    bool ajoutReussi = P.ajouterEnStock(idProduit, quantiteAAjouter);

    if (ajoutReussi) {
        QMessageBox::information(this, "Succès", "Produit ajouté en stock avec succès.");
        // Actualisez la vue des alertes après l'ajout en stock
        afficherAlertes();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout en stock.");
    }
}


void MainWindow::on_pushButtonAjouterStock_clicked()
{
    // Récupérez l'ID du produit et la quantité à ajouter depuis la ligne sélectionnée dans le QTableView
        QModelIndexList selectedIndexes = ui->tab_alertes->selectionModel()->selectedRows();

        if (!selectedIndexes.isEmpty()) {
            int idProduit = selectedIndexes.at(0).data(Qt::UserRole).toInt();

            // Affichez une boîte de dialogue pour saisir la quantité à ajouter
            bool ok;
            int quantiteAAjouter = QInputDialog::getInt(this, "Ajouter en stock", "Quantité à ajouter :", 1, 1, 100, 1, &ok);

            if (ok) {
                // Appelez la fonction pour ajouter en stock
                ajouterEnStock(idProduit, quantiteAAjouter);
            }
        } else {
            QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner un produit en alerte pour ajouter en stock.");
        }
}





void MainWindow::on_pb_valider_clicked()
{
    int id = ui->l_id->text().toInt();
    QString nom = ui->le_nom->text();
    QString prenom = ui->le_prenom->text();
    int id_produit = ui->id_prod->text().toInt();
    QString produit = ui->le_produit->text();
    int num_telephone = ui->le_numero->text().toInt();
    QDate date_=ui->dateEdit->date();
    int prix = ui->prix->text().toInt();

    QSqlQuery query;
    QString res=QString::number(id);
    query.prepare("SELECT * FROM GESTION_COMMANDES where PRIX= :prix");
    query.bindValue(":prix",prix);
    query.exec();

    if(query.exec())
    {
        if (prix > 1000)
        {
            // Connect to Arduino and send the product ID
            QByteArray idData;
            idData.append(prix);

            A.write_to_arduino(idData);

            if (A.connect_arduino() == 0)
            {
                A.write_to_arduino(idData);
                // Ne fermez pas la connexion ici
            }
            else
            {
                qDebug() << "Failed to connect to Arduino.";
            }
        }

    }

    // Retrieve the selected typePaiement from your UI element (e.g., comboBox)
    QString typePaiment = ui->calculatePaymentStatistics->currentText();

    Commande C(id, nom, prenom, produit, num_telephone, id_produit,typePaiment,date_,prix);

    // Call the ajouter function with the additional typePaiement parameter
    bool test = C.ajouter(typePaiment);

    if (test)
    {
        ui->tableView_2->setModel(etmp.afficher());

        // Check if the price is greater than 1000
      /*  if (prix > 1000)
        {
            // Connect to Arduino and send the product ID
            QByteArray idData;
            idData.append(QString::number(id_produit));


            if (A.connect_arduino() == 0)
            {
                A.write_to_arduino(idData);
                // Ne fermez pas la connexion ici
            }
            else
            {
                qDebug() << "Failed to connect to Arduino.";
            }
        }*/

        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("ajout effectué\n"
                                             "click Cancel to exit."), QMessageBox::Cancel);

        // Fermez la connexion Arduino ici, après avoir effectué toutes les opérations nécessaires
        A.close_arduino();
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                              QObject::tr("ajout non effectué\n"
                                          "click Cancel to exit."), QMessageBox::Cancel);
    }
}




void MainWindow::on_supp_3_clicked()
{
    int id=ui->supp_4->text().toInt();
    bool test=etmp.supprimer(id);
    if(test)
    {
           ui->tableView_2->setModel(etmp.afficher());
         QMessageBox::information(nullptr,QObject::tr("OK"),QObject::tr("Suppression effectuee\n""Click Cancel to exit."),QMessageBox::Cancel);

    }
    else
        QMessageBox::critical(nullptr,QObject::tr("OK"),QObject::tr("Suppression non effectuee\n""Click Cancel to exit."),QMessageBox::Cancel);


}

void MainWindow::on_modif_2_clicked()
{
    int id = ui->l_id->text().toInt();
        QString nom = ui->le_nom->text();
        QString prenom = ui->le_prenom->text();
        int id_produit = ui->id_prod->text().toInt();
        QString produit = ui->le_produit->text();
        int num_telephone = ui->le_numero->text().toInt();
        QDate date_=ui->dateEdit->date();
        int prix = ui->prix->text().toInt();


        // Retrieve the selected typePaiement from your UI element (e.g., comboBox)
        QString typePaiment = ui->calculatePaymentStatistics->currentText();
        Commande C(id, nom, prenom, produit, num_telephone, id_produit,typePaiment,date_,prix);


        bool test=C.modifierCommande();
        if (test)
         {
             QMessageBox::information(nullptr, QObject::tr("ok"),
                                      QObject::tr("Update effectué\n"
                                                  "Click Cancel to exit."), QMessageBox::Cancel);
             ui->tableView_2->setModel(etmp.afficher());
         }
         else
             QMessageBox::critical(nullptr, QObject::tr("not ok"),
                                      QObject::tr("Update non effectué\n"
                                                  "Click Cancel to exit."), QMessageBox::Cancel);
      }


void MainWindow::on_recherche_clicked(const QString& arg1)
{

}





void MainWindow::on_tri_2_clicked()
{
    Commande c;
    QString select=ui->comboBox_4->currentText();
    if(select=="id")
    {
        QSqlQueryModel* sortedCommandesModel = Commande::triCommandes();

        ui->tableView_2->setModel(c.triCommandes());
    }
    else if (select=="nom")
    {
        QSqlQueryModel* sortedCommandesModel = Commande::triCommandes();

        ui->tableView_2->setModel(c.triCommandesParnom());
    }
    else if (select=="prenom")
    {
        QSqlQueryModel* sortedCommandesModel = Commande::triCommandes();

        ui->tableView_2->setModel(c.triCommandesParprenom());
    }



}



void MainWindow::on_pdf_2_clicked()
{
    Commande commande;
       commande.exportToPDF(ui->tableView_2);
}
// Assume you have a list of Commande objects named commandes






/*void MainWindow::on_pushButton_2_clicked()
{
    Commande c;
    QLayoutItem* item;
           while ((item =ui->verticalLayout_2 ->takeAt(0)) != nullptr) {
               delete item->widget();
               delete item;
           }
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    QChartView *chartView1 = c.countenligne();

            // Set up your UI to include the chart view
            ui->verticalLayout_2->addWidget(chartView1);
}*/






void MainWindow::on_historique_2_clicked()
{
    Commande commande;
        QSqlQueryModel* historiqueModel = commande.historique();
        ui->tableView_2->setModel(historiqueModel);
}

void MainWindow::on_cherche_cursorPositionChanged(int arg1, int arg2)
{
    //kjhy
}

void MainWindow::on_cherche_2_textChanged(const QString &arg1)
{
    Commande C;
    QString select=ui->comboBox_3->currentText();
    if(select=="id")
    {
        QString IDC=ui->cherche_2->text();
               ui->tableView_2->setModel(C. rechercher(IDC));
              ui->tableView_2->clearSelection();
    }
    else if (select=="nom")
    {
        QString IDC=ui->cherche_2->text();
               ui->tableView_2->setModel(C. rechercherParNom(IDC));
              ui->tableView_2->clearSelection();
    }
    else if (select=="prenom")
    {
        QString IDC=ui->cherche_2->text();
               ui->tableView_2->setModel(C. rechercherParPrenom(IDC));
              ui->tableView_2->clearSelection();
    }

}

void MainWindow::on_envoyer_clicked()
{
    /*Commande c;
       QString userMessage = ui->envoyer->text();
       std::string botResponse = c.chatbotFunction(userMessage);
          ui->label_chatbot_response->setText(QString::fromStdString(botResponse));*/
    Commande c;
        QString userMessage = ui->envoyer->text();
        std::string botResponse = c.chatbotFunction(userMessage);
        ui->label_chatbot_response->setText(QString::fromStdString(botResponse));
}



/*void MainWindow::on_exel_2_clicked()
{
    // Assuming ui->tab_emp is a QTableView in your UI file

        // Set the model for the QTableView
        ui->tableView_2->setModel(C.Excel());

        QString filters("CSV files (*.csv);;All files (*)");
        QString defaultFilter("CSV files (*.csv)");
        QString fileName = QFileDialog::getSaveFileName(this, "Save file", QCoreApplication::applicationDirPath(),
                                    filters, &defaultFilter);
        QFile file(fileName);

        QAbstractItemModel *model = ui->tableView_2->model(); // Use ui->tab_emp here

        if (file.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream data(&file);
            QStringList strList;

            // Write headers
            for (int i = 0; i < model->columnCount(); i++) {
                if (model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().length() > 0)
                    strList.append("\"" + model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() + "\"");
                else
                    strList.append("");
            }
            data << strList.join(";") << "\n";

            // Write data
            for (int i = 0; i < model->rowCount(); i++) {
                strList.clear();
                for (int j = 0; j < model->columnCount(); j++) {
                    if (model->data(model->index(i, j)).toString().length() > 0)
                        strList.append("\"" + model->data(model->index(i, j)).toString() + "\"");
                    else
                        strList.append("");
                }
                data << strList.join(";") + "\n";
            }

            file.close();
            QMessageBox::information(this, "Exporter To Excel", "Exporter En Excel Avec Succès");
        }
        else {
            QMessageBox::warning(this, "Error", "Unable to open the file for writing");
        }


}*/


/*void MainWindow::on_exel_2_clicked()
{
    QTableView *table = ui->tableView_2;

        QString fileName = QFileDialog::getSaveFileName(this, "Save as Excel", QDir::homePath(), "Excel Files (*.xlsx)");

        if (fileName.isEmpty())
            return;

       // QXlsx::Document xlsx;

        int numRows = table->model()->rowCount();
        int numCols = table->model()->columnCount();

        // Ajoutez les en-têtes des colonnes
        for (int col = 0; col < numCols; ++col)
        {
            xlsx.write(1, col + 1, table->model()->headerData(col, Qt::Horizontal).toString());
        }

        // Ajoutez les données du QTableView
        for (int row = 0; row < numRows; ++row)
        {
            for (int col = 0; col < numCols; ++col)
            {
                xlsx.write(row + 2, col + 1, table->model()->data(table->model()->index(row, col)).toString());
            }
        }

        // Enregistrez le fichier Excel
        xlsx.saveAs(fileName);

        QMessageBox::information(this, "Success", "Data has been successfully exported to Excel.");
}
*/





/*void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    qDebug() << date.toString("dd/MM/yyyy");



        //QString Search=ui->calendarWidget->selectedDate()->text();
        ui->listView->setModel(cal.chercher_commande(date.toString("dd/MM/yyyy")));
        //ui->listView->setModel(cal.chercher_commande(date.toString("dd/MM/yyyy")));
}*/
/*void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    qDebug() << date.toString("dd/MM/yyyy");

    ui->listView->setModel(cal.chercher_commande(date));
}*/
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    qDebug() << "Clicked Date: " << date.toString("dd/MM/yyyy");

    // Use your search_date in the query
    QSqlQueryModel *resultModel = cal.chercher_commande(date);

    if (resultModel)
    {
        qDebug() << "Query Successful. Rows: " << resultModel->rowCount();

        // Debug output for each row
        for (int row = 0; row < resultModel->rowCount(); ++row)
        {
            QStringList rowData;
            for (int col = 0; col < resultModel->columnCount(); ++col)
            {
                rowData << resultModel->data(resultModel->index(row, col)).toString();
            }
            qDebug() << "Row " << row << ": " << rowData.join(", ");
        }

        ui->listView->setModel(resultModel);
    }
    else
    {
        qDebug() << "Query Failed!";
    }
}
/*void MainWindow::update_label()
{
    data=A.read_from_arduino();

    if(data=="1")

        ui->label_3->setText("ON"); // si les données reçues de arduino via la liaison série sont égales à 1
    // alors afficher ON

    else if (data=="0")

        ui->label_3->setText("OFF");   // si les données reçues de arduino via la liaison série sont égales à 0
     //alors afficher ON
}*/
void MainWindow::update_label()
{
    QByteArray dataFromArduino = A.read_from_arduino();

    // Print the received data in hexadecimal format for debugging purposes
    qDebug() << "Data from Arduino (hex): " << dataFromArduino.toHex();

    // Check if the received data is valid
    if (dataFromArduino.size() > 0 && dataFromArduino.size() <= 1000)
    {
        if (dataFromArduino == "1")
        {
            ui->label_3->setText("ON"); // if the received data is equal to 1
        }
        else if (dataFromArduino == "0")
        {
            ui->label_3->setText("OFF"); // if the received data is equal to 0
        }
        else
        {
            qDebug() << "Invalid data received: " << dataFromArduino;
        }
    }
    else
    {
        qDebug() << "Invalid data size received: " << dataFromArduino.size();
    }
}

void MainWindow::on_pushButton_6_clicked()   // implémentation du slot bouton on
{
     A.write_to_arduino("1"); //envoyer 1 à arduino
}

void MainWindow::on_pushButton_4_clicked()  // implémentation du slot bouton off
{

     A.write_to_arduino("0");  //envoyer 0 à arduino
}

void MainWindow::on_pushButton_3_clicked()  // implémentation du slot bouton +
{
    A.write_to_arduino("2");   //envoyer 2 à arduino
}

void MainWindow::on_pushButton_5_clicked() // implémentation du slot bouton -
{
    A.write_to_arduino("3");  //envoyer 3 à arduino
}







void MainWindow::on_pushButtonstat_clicked()
{
    Commande c;
    QLayoutItem* item;
           while ((item =ui->verticalLayout ->takeAt(0)) != nullptr) {
               delete item->widget();
               delete item;
           }
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    QChartView *chartView1 = c.countenligne();

            // Set up your UI to include the chart view
            ui->verticalLayout->addWidget(chartView1);

}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}








void MainWindow::on_creationBtn_clicked(){
    int numero_operation = ui->IdSpinBox->text().toInt();
    QString type = ui->typeComboBox->currentText();
    QString duree = ui->dureeSpinBox->text();
    QString etat = ui->EtatlineEdit_1->text();

    operation Op(numero_operation,type,duree,etat);
    bool test = Op.ajouter();
    if(test){
        QMessageBox::information(nullptr, QObject::tr("Ajouter effectué"),
                                 QObject::tr("Ajouter effectué\nclick cancel to exit"),
                                 QMessageBox::Cancel);
        ui->tableViewAffichage->setModel(Op.afficher());
    }
    else
        QMessageBox::critical(nullptr,QObject::tr("not ok"),QObject::tr("Ajout non effectué.\n"
                                                                        "Click cancel to exit."),QMessageBox::Cancel);

}
void MainWindow::on_suppBtn_clicked(){
    int numero_operation = ui->suppSpinBox->text().toInt();
    bool test = Op.supprimer(numero_operation);
    if(test){
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("suppression effectué\nclick cancel to exit"),
                                 QMessageBox::Cancel);
    }
    else{
        QMessageBox::critical(nullptr,QObject::tr("not ok"),QObject::tr("suppression non effectué.\n"
                                                                        "Click cancel to exit."),QMessageBox::Cancel);
    }
    ui->tableViewAffichage->setModel(Op.afficher());
}

void MainWindow::on_modifBtn_clicked()
{
    int numero_operation=ui->modifSpinBox->text().toInt();
    QString type=ui->ModifTypeComboBox->currentText();
    QString duree=ui->modifDureeLineEdit->text();
    QString etat=ui->modifEtatLineEdit->text();
    operation Op(numero_operation,type,duree,etat);
    bool test=Op.modifier();
   QMessageBox msgBox;
    if(test )
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("modification effectué"),
                                 QMessageBox::Cancel);
        ui->tableViewAffichage->setModel(Op.afficher());
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("OK"),
                                 QObject::tr("modification non effectué"),
                                 QMessageBox::Cancel);
}
void MainWindow::on_rechercheBtn_clicked()
{
    QString inputText = ui->RechlineEdit->text().simplified(); // Remove extra spaces

    // Create an operation instance
    operation Op;

    // Perform the search operation
    QSqlQueryModel *resultModel = nullptr;

    // Search by ID
    bool isNumeric;
    int searchId = inputText.toInt(&isNumeric);
    if (isNumeric) {
        resultModel = Op.rechercherParId(searchId);
    } else {
        // Search by type
        resultModel = Op.rechercherParType(inputText);
        if (!resultModel) {
            // If not found by type, try searching by etat
            resultModel = Op.rechercherParEtat(inputText);
        }
    }

    // Debug statements
    qDebug() << "Search Text:" << inputText;
    qDebug() << "Is Numeric:" << isNumeric;

    // Handle the search result
    if (resultModel) {
        // Operation found, update your QTableView
        ui->tableViewAffichage->setModel(resultModel);
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Search completed"),
                                 QMessageBox::Cancel);
    } else {
        // Operation not found or an error occurred
        QMessageBox::warning(nullptr, QObject::tr("Not Found"),
                             QObject::tr("Operation not available."),
                             QMessageBox::Cancel);
        // Optionally update the table view with all operations if nothing is found
        ui->tableViewAffichage->setModel(Op.afficher());
    }
}

void MainWindow::on_sortBtn_clicked()
{
    QString attribut = ui->choixComboBox->currentText();
    bool ascending = true; // Check the checkbox for ascending, uncheck for descending

    // Create an operation instance
    operation Op;

    // Perform the sort operation
    QSqlQueryModel *sortedModel = Op.trierParAttribut(attribut, ascending);

    // Handle the sort result
    if (sortedModel) {
        // Update your QTableView
        ui->tableViewAffichage->setModel(sortedModel);
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Sort completed"),
                                 QMessageBox::Cancel);
    } else {
        // Sort not completed or an error occurred
        QMessageBox::warning(nullptr, QObject::tr("Not Found"),
                             QObject::tr("Sort not available."),
                             QMessageBox::Cancel);
        // Optionally update the table view with all operations if sorting is not successful
        ui->tableViewAffichage->setModel(sortedModel);
    }
}
void MainWindow::on_PDFBtn_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tableViewAffichage->model()->rowCount();
    const int columnCount = ui->tableViewAffichage->model()->columnCount();

    // Styling for the table
    out << "<html><head><style>"
        << "table { width: 100%; border-collapse: collapse; margin-top: 20px; font-family: Arial, sans-serif; }"
        << "th, td { border: 10px solid black !important; padding: 15px; text-align: center; font-size: 14px; }"
        << "th { background-color: white; color: black; }"
        << "tr:nth-child(even) { background-color: #f2f2f2; }"
        << "tr:hover { background-color: #D3D3D3; }"
        << "</style></head><body>";

    // Add table headers
    out << "<table><tr>";
    for (int column = 0; column < columnCount; column++)
        if (!ui->tableViewAffichage->isColumnHidden(column))
            out << "<th>" << ui->tableViewAffichage->model()->headerData(column, Qt::Horizontal).toString() << "</th>";
    out << "</tr>";

    // Add table rows
    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->tableViewAffichage->isColumnHidden(column)) {
                QString data = ui->tableViewAffichage->model()->data(ui->tableViewAffichage->model()->index(row, column)).toString().simplified();
                out << "<td>" << ((!data.isEmpty()) ? data : "&nbsp;") << "</td>";
            }
        }
        out << "</tr>";
    }

    out << "</table></body></html>";

    // Save the PDF
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save as PDF", QString(), "*.pdf");
    if (fileName.isEmpty()) return;

    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.setPageSize(printer.pageRect().size());
    doc.print(&printer);
}
void MainWindow::on_StatTypeBtn_clicked() {
    // Assuming you have a QTableView named tableView for displaying statistics
    QMap<QString, int> statistics = Op.getTypeStatistics();

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setColumnCount(1);
    model->setHorizontalHeaderLabels(QStringList() << "Stat par type");

    int row = 0;
    for (const QString &type : statistics.keys()) {
        QStandardItem *item = new QStandardItem(QString("%1: %2").arg(type, QString::number(statistics.value(type))));
        model->setItem(row, 0, item);
        row++;
    }

    ui->StatOpTableView->setModel(model);
    ui->StatOpTableView->horizontalHeader()->setStyleSheet("color: gold");
    ui->StatOpTableView->verticalHeader()->setVisible(false);
}
void MainWindow::on_statEtatBtn_clicked() {
    // Assuming you have a QTableView named tableView for displaying statistics
    QMap<QString, int> statistics = Op.getEtatStatistics();

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setColumnCount(1);
    model->setHorizontalHeaderLabels(QStringList() << "Stat par etat");

    int row = 0;
    for (const QString &etat : statistics.keys()) {
        QStandardItem *item = new QStandardItem(QString("%1: %2").arg(etat, QString::number(statistics.value(etat))));
        model->setItem(row, 0, item);
        row++;
    }

    ui->StatOpTableView->setModel(model);
    ui->StatOpTableView->horizontalHeader()->setStyleSheet("color: gold");
    ui->StatOpTableView->verticalHeader()->setVisible(false);
}
void MainWindow::on_SuiviBtn_clicked() {
    // Get the numero_operation from the QLineEdit
    int numero_operation = ui->suiviLineEdit->text().toInt();

    // Create an operation instance
    operation Op;

    // Search for the operation by ID
    QSqlQueryModel *resultModel = Op.rechercherParId(numero_operation);

    if (!resultModel) {
        // An error occurred during the search
        QMessageBox::warning(this, "Error", "An error occurred during the search.", QMessageBox::Ok);
        return;
    }

    if (resultModel->rowCount() == 0) {
        // Operation not found, display an error message
        QMessageBox::warning(this, "Error", "Operation not found.", QMessageBox::Ok);
        return;
    }

    // Get the etat and duree values
    QString etat = resultModel->record(0).value("ETAT").toString();
    QString duree = resultModel->record(0).value("DUREE").toString();

    // Display the LED based on etat
    if (etat == "en attente") {
        ui->stackedWidget->setCurrentIndex(0); // Index of the red LED
    } else if (etat == "en cours") {
        ui->stackedWidget->setCurrentIndex(1); // Index of the yellow LED
    } else if (etat == "pret") {
        ui->stackedWidget->setCurrentIndex(2); // Index of the green LED
    }
    ui->stackedWidget->setStyleSheet("border-radius: 20px;");

    // Display the duree in a QLabel or similar
    ui->OpLabel_16->setText("Duree: " + duree + "jrs restant");
    ui->OpLabel_21->setText("Operation "+ etat);
}
void MainWindow::on_AccepteOpBtn_clicked() {
        // Create an operation instance
        operation Op;

        // Count the number of operations with etat "en attente"
        int enAttenteCount = Op.countOperationsByEtat("en attente");

        // Calculate the number of available places
        int availablePlaces = 10 - enAttenteCount;

        // Update QLabel texts and styles
        if (availablePlaces > 0) {
            ui->OpLabel_25->setText(QString("Vous pouvez ajouter au maximum %1 opérations.").arg(availablePlaces));
            ui->OpLabel_25->setStyleSheet("color: green;");
        } else {
            ui->OpLabel_25->setText("Vous ne pouvez pas ajouter une autre opération. Aucune place disponible.");
            ui->OpLabel_25->setStyleSheet("color: red;");
        }

        // Show the number of available places in another label
        ui->OpLabel_24->setText(QString("vous avez déjà %1 operations en attentes").arg(enAttenteCount));
    }

void MainWindow::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);

}
//integration client

void MainWindow::on_ajouter_cl_clicked(){
    QString ID_string = ui->ID->text();
      bool ok;
      int ID = ID_string.toInt(&ok);

      if (!ok) {

          QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer ID.");
          return;
      }
      QString NOM = ui->NOM->text();


      QString PRENOM = ui->PRENOM->text();

      QString NUMERO = ui->NUMERO->text();
      QString ADRESSE = ui->ADRESSE->text();

      client tr(ID,NOM,PRENOM,NUMERO,ADRESSE);
      bool test = tr.ajouter();

      QMessageBox msgBox;
      if (test) {
          msgBox.setText("ajout avec succes");
          msgBox.exec();
          ui->ID->clear();
          ui->NOM->clear();
          ui->PRENOM->clear();
          ui->NUMERO->clear();
          ui->ADRESSE->clear();

      } else {
          msgBox.setText("echec");
          msgBox.exec();
      }

      ui->affiche_tab->setModel(tr.afficher());
};
void MainWindow::on_supprimer_cl_clicked() {
    client f;
    int ID= ui->ID->text().toInt();
    bool test = f.supprimer(ID);
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Ok"),
                                 QObject::tr("Suppression effectuée.\n"
                                             "Click Ok to exit."), QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not ok"),
                              QObject::tr("Échec de la suppression.\n"
                                          "Le fournisseur n'existe pas dans la base de données."),
                              QMessageBox::Cancel);
    }
    ui->affiche_tab->setModel(f.afficher());
}


void MainWindow::on_modifier_cl_clicked()
{

        int ID=ui->ID->text().toInt();
           QString NOM = ui->NOM->text();
           QString PRENOM = ui->PRENOM->text();
           QString NUMERO = ui->NUMERO->text();
           QString ADRESSE = ui->ADRESSE->text();


              client f(ID,NOM,PRENOM,NUMERO,ADRESSE);

                 bool test=f.modifier(ID,NOM,PRENOM,NUMERO,ADRESSE);

                 if(test)

               {ui->affiche_tab->setModel(f.afficher());

               QMessageBox::information(nullptr, QObject::tr("Modifier avec succées "),

                                 QObject::tr("invite modifiée.\n"

                                             "Click ok to exit."), QMessageBox::Ok);



               }

                 else

                     QMessageBox::critical(nullptr, QObject::tr("Modifier a echoué"),

                                 QObject::tr("echec d'ajout !.\n"

                                             "Click Cancel to exit."), QMessageBox::Cancel);


}


void MainWindow::on_afficher_clicked()
{
    client f;
    ui->affiche_tab->setModel(f.afficher());
}


void MainWindow::on_affiche_tab_activated(const QModelIndex &index)
{
    QString val=ui->affiche_tab->model()->data(index).toString();
    QSqlQuery qry;
    qry.prepare("select * from GEST_CLIENT where ID='"+val+"'");
    if(qry.exec())
    {
     while(qry.next())
     {ui->ID->setText(qry.value(0).toString());
      ui->NOM->setText(qry.value(1).toString());
      ui->PRENOM->setText(qry.value(2).toString());
      ui->NUMERO->setText(qry.value(3).toString());
      ui->ADRESSE->setText(qry.value(4).toString());

     }
    }
    else
    {
     QMessageBox::critical(this,tr("error::"),qry.lastError().text());
    }
}


void MainWindow::on_chercher_textChanged(const QString &arg1)
{
    client f;
        int ID= ui->chercher->text().toInt();

      f.recherche(ui->affiche_tab,ID);
        if (ui->chercher->text().isEmpty())
        {
            ui->affiche_tab->setModel(f.afficher());
        }
}

void MainWindow::on_tri_cl_clicked()
{
    client f;
         ui->affiche_tab->setModel(f.trier());
}

void MainWindow::on_pdf_cl_clicked()
{
    client f;
        f.telechargerPDF();

                 QMessageBox::information(nullptr,QObject::tr("OK"),
                            QObject::tr("Téléchargement terminé"), QMessageBox::Cancel);

}

void MainWindow::on_stat_cl_clicked()
{
    statistic = new class Statistic(this);

        statistic->setWindowTitle("Statistiques");
        statistic->show();
}

void MainWindow::on_qrcode_cl_clicked()
{



        QString text ="Client details :"+ ui->affiche_tab->model()->data(ui->affiche_tab->model()->index(ui->affiche_tab->currentIndex().row(),0)).toString()
                                +" "+ui->affiche_tab->model()->data(ui->affiche_tab->model()->index(ui->affiche_tab->currentIndex().row(),1)).toString()
                                +" "+ui->affiche_tab->model()->data(ui->affiche_tab->model()->index(ui->affiche_tab->currentIndex().row(),2)).toString()
                                +" "+ui->affiche_tab->model()->data(ui->affiche_tab->model()->index(ui->affiche_tab->currentIndex().row(),3)).toString()
                                +" "+ui->affiche_tab->model()->data(ui->affiche_tab->model()->index(ui->affiche_tab->currentIndex().row(),4)).toString()
                                +" "+ui->affiche_tab->model()->data(ui->affiche_tab->model()->index(ui->affiche_tab->currentIndex().row(),5)).toString()
                                +" "+ui->affiche_tab->model()->data(ui->affiche_tab->model()->index(ui->affiche_tab->currentIndex().row(),6)).toString();
                        //text="user data";
                        using namespace qrcodegen;
                          // Create the QR Code object
                          QrCode qr = QrCode::encodeText( text.toUtf8().data(), QrCode::Ecc::MEDIUM );
                          qint32 sz = qr.getSize();
                          QImage im(sz,sz, QImage::Format_RGB32);
                            QRgb black = qRgb(  0,  0,  0);
                            QRgb white = qRgb(255,255,255);
                          for (int y = 0; y < sz; y++)
                            for (int x = 0; x < sz; x++)
                              im.setPixel(x,y,qr.getModule(x, y) ? black : white );
                          ui->qr_code->setPixmap( QPixmap::fromImage(im.scaled(256,256,Qt::KeepAspectRatio,Qt::FastTransformation),Qt::MonoOnly) );


}

void MainWindow::on_push_id_clicked()
{
    lo = new class client_log (this);
    lo->setWindowTitle("client");
    lo->show();
}
