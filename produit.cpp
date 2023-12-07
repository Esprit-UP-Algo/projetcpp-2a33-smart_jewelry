#include "produit.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QList>
#include <iostream>
#include <QSqlError>
using namespace std;
Produit::Produit()
{
  id = 0;
  nom = "";
  prix = 0;
  category=Undefined;
  quantite=0;

}


Produit::Produit(int id, QString nom, float prix, Category category,int quantite)
{
  this->id = id;
  this->nom = nom;
  this->prix = prix;
  this->category = category;
  this->quantite= quantite;
}
int Produit::getid() { return id; }
QString Produit::getnom() { return nom; }
float Produit::getprix() { return prix; }
Produit::Category Produit::getCategory() { return category; }
int Produit::getquantite() { return quantite; }
void Produit::setid(int id) { this->id = id; }
void Produit::setnom(QString nom) { this->nom = nom; }
void Produit::setprix(float prix) { this->prix = prix; }
void Produit::setCategory(Category category) { this->category = category; }
void Produit::setquantite(int quantite) { this->quantite = quantite; }


QString Produit::ajouter()
{
  if (category == Category::Undefined)
  {
    qDebug() << "Veuillez choisir une categorie de produit.";
    return "false";
  }

  QSqlQuery query;
  QString result;
  query.prepare("SELECT * FROM GESTION_DE_PRODUITS WHERE ID_PRODUIT = :id AND NOM = :nom");
  query.bindValue(":id", id);
  query.bindValue(":nom", nom);

  query.exec();

  if (query.next())
  {
    int currentQuantity = query.value("QUANTITE").toInt();
    int newQuantity = currentQuantity + quantite;

    query.prepare("UPDATE GESTION_DE_PRODUITS SET QUANTITE = :newQuantity WHERE ID_PRODUIT =:id AND NOM = :nom");
    query.bindValue(":newQuantity", newQuantity);
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.exec();
    result = "PRODUCT_EXISTS";
  }
  else // New produit, add
  {
    QString id_string = QString::number(id);
    QString prix_string = QString::number(prix);
    QString category_string = getCategoryString(category);
    QString quantite_string = QString::number(quantite);


    query.prepare("INSERT INTO GESTION_DE_PRODUITS (ID_PRODUIT, NOM, PRIX,  CATEGORY, QUANTITE) "
                  "VALUES (:id_produit, :nom, :prix, :category, :quantite)");
    query.bindValue(":id_produit", id_string);
    query.bindValue(":nom", nom);
    query.bindValue(":prix", prix_string);
    query.bindValue(":category", category_string);
    query.bindValue(":quantite", quantite_string);
    query.exec();
    result = "NEW_PRODUCT";
  }
  return result;
}



QString Produit::getCategoryString(Category category)
{
  switch (category)
  {
  case Category::Collier:
    return "Collier";
  case Category::Boucles:
    return "Boucles";
  case Category::Bracelet:
    return "Bracelet";
  case Category::Anneau:
    return "Anneau";
  default:
    return "Undefined";
  }
}
Produit::Category Produit::stringToCategory(const QString categoryString)


{
  if (categoryString == "Collier")
  {
    return Collier;
  }
  else if (categoryString == "Boucles")
  {
    return Boucles;
  }
  else if (categoryString == "Bracelet")
  {
    return Bracelet;
  }
  else if (categoryString == "Anneau")
  {
    return Anneau;
  }
  else
  {
    return Undefined;
  }
}

bool Produit::supprimer(int id)
{
  QSqlQuery query;
  query.prepare("Delete FROM GESTION_DE_PRODUITS where ID_PRODUIT=:id");
  query.bindValue(0, id);

  return query.exec();
}

QSqlQueryModel *Produit::afficher()
{
  QSqlQueryModel *model = new QSqlQueryModel();

  model->setQuery("SELECT* FROM GESTION_DE_PRODUITS ");
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
  model->setHeaderData(6, Qt::Horizontal, QObject::tr("prix"));
  model->setHeaderData(7, Qt::Horizontal, QObject::tr("category"));
  model->setHeaderData(8, Qt::Horizontal, QObject::tr("quantite"));
//ui->table_employe->setModel(emp.getSortedModel());
  return model;
}



QString Produit::CategorytoString(Category category)
{
  QString categoryString[4]={"Collier","Boucles","Anneau","Bracelet"} ;

 return categoryString[category];
}


QSqlQueryModel *Produit::get_FilteredProducts(const QList<Produit::Category> selectedCategories)
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QString query = "SELECT * FROM GESTION_DE_PRODUITS WHERE ";
    QStringList categoryConditions;

    for (const Produit::Category category : selectedCategories)
    {


        categoryConditions << QString("category = '%1'").arg(Produit::CategorytoString( category));


    }

    query += categoryConditions.join(" OR ");

    std::cout << query.toStdString() << std::endl;
    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("prix"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("category"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("quantite"));

    return model;
}





bool Produit::modifier()
{
    QSqlQuery query;
    QString id_string = QString::number(id);
    QString prix_string = QString::number(prix);
    QString quantite_string = QString::number(quantite);

    query.prepare("UPDATE GESTION_DE_PRODUITS SET NOM=:nom, PRIX=:prix, CATEGORY=:category,QUANTITE=:quantite  where ID_PRODUIT=:id ");

    query.bindValue(":id", id_string);
    query.bindValue(":nom", nom);
    query.bindValue(":prix", prix_string);
    query.bindValue(":category", CategorytoString(category));
    query.bindValue(":quantite", quantite_string);


    return query.exec();
}

QSqlTableModel* Produit::getSortedModel()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("GESTION_DE_PRODUITS");
    model->setSort(0, Qt::AscendingOrder);
    model->select();
    return model;}
/*int Produit::getLowStockProductCount()
{
    cout<<"++++++++++++++++++++++++++++++++";
    QSqlQuery query;

    query.prepare("SELECT* FROM GESTION_DE_PRODUITS WHERE  ");

    cout<<"--------------";
query.exec();
    if(query.next())
    {

cout<<"--------------";

        cout<<"***********"<<query.result();
        return query.value(0).toInt();

    }

    //return 0; // Return 0 in case of an error or no results
}*/

int Produit::getLowStockProductCount()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM GESTION_DE_PRODUITS WHERE QUANTITE < :seuil");
    query.bindValue(":seuil", 5); 
    query.exec();

    if (query.next())
    {
        return query.value(0).toInt();
    }

    return 0;
}
/*int Produit::getLowStockProductCount()
{
    qDebug() << "Fetching low stock product count...";

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM GESTION_DE_PRODUITS WHERE STOCK < SEUIL_STOCK");
    query.exec();

    QSqlError error = query.lastError();
    if (error.isValid()) {
        qWarning() << "Error executing query:" << error.text();
        return -1; // Gestion de l'erreur
    }

    if (query.next()) {
        int lowStockCount = query.value(0).toInt();
        qDebug() << "Low stock product count: " << lowStockCount;
        return lowStockCount;
    }

    // Si la requête n'a pas réussi à récupérer les données
    qWarning() << "Error fetching low stock product count: No results";
    return -1;
}/*


/*int Produit::getLowStockProductCount()
{
  Produit produitInstance;  // Créez une instance de Produit
  QList<Produit> alertes = produitInstance.getProduitsEnAlerte();
  return alertes.size();
}*/




Produit Produit::rechercherParId(int id )
{
    QSqlQuery query;
    query.prepare("SELECT * FROM GESTION_DE_PRODUITS WHERE ID_PRODUIT = :id ");
    query.bindValue(":id", id);
    query.exec();

    Produit produit;

    if (query.next())
    {
        produit.setid(query.value("ID_PRODUIT").toInt());
        produit.setnom(query.value("NOM").toString());
        produit.setprix(query.value("PRIX").toFloat());
        produit.setCategory(stringToCategory(query.value("CATEGORY").toString()));
        produit.setquantite(query.value("QUANTITE").toInt());
    }

    return produit;
}


// Dans produit.cpp
QMap<Produit::Category, int> Produit::getStatsParCategorie() const
{
    QMap<Produit::Category, int> stats;

    QSqlQuery query;
    query.prepare("SELECT CATEGORY, COUNT(*) FROM GESTION_DE_PRODUITS GROUP BY CATEGORY");
    query.exec();

    while (query.next())
    {
        Produit::Category category = Produit::stringToCategory(query.value(0).toString());
        int count = query.value(1).toInt();

        stats.insert(category, count);
    }

    return stats;
}


QList<Produit> Produit::getProduitsEnAlerte()
{
    QList<Produit> alertes;

    // Exemple : récupérer les produits avec une quantité inférieure à 5
    QSqlQuery query;
    query.prepare("SELECT * FROM GESTION_DE_PRODUITS WHERE QUANTITE < 5");
    query.exec();

    while (query.next()) {
        int id = query.value("ID_PRODUIT").toInt();
        QString nom = query.value("NOM").toString();
        float prix = query.value("PRIX").toFloat();
        Category category = stringToCategory(query.value("CATEGORY").toString());
        int quantite = query.value("QUANTITE").toInt();

        Produit produit(id, nom, prix, category, quantite);
        alertes.append(produit);
    }

    return alertes;
}

bool Produit::ajouterEnStock(int idProduit, int quantiteAAjouter)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM GESTION_DE_PRODUITS WHERE ID_PRODUIT = :id");
    query.bindValue(":id", idProduit);
    query.exec();

    if (query.next()) {
        int currentQuantity = query.value("QUANTITE").toInt();
        int newQuantity = currentQuantity + quantiteAAjouter;

        query.prepare("UPDATE GESTION_DE_PRODUITS SET QUANTITE = :newQuantity WHERE ID_PRODUIT = :id");
        query.bindValue(":newQuantity", newQuantity);
        query.bindValue(":id", idProduit);

        // Assurez-vous d'exécuter la requête pour effectuer la mise à jour
        if (query.exec()) {
            return true; // La mise à jour s'est bien déroulée
        }
    }

    return false;
}
