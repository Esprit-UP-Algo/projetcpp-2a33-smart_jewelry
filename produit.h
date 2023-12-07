#ifndef PRODUIT_H
#define PRODUIT_H
#include "QSqlQueryModel"
#include <QString>
#include <QSqlTableModel>

class Produit
{
public:
    enum Category
    {
        Collier,
        Boucles,
        Bracelet,
        Anneau,
        Undefined
    };
    Produit();
    Produit(int,QString,float, Category,int);
    int getid();
    QString getnom();
    float getprix();
    Category getCategory();
    int getquantite();
    void setid(int);
    void setnom(QString);
    void setprix(float);
    void setCategory(Category);
    void setquantite(int);
    static Category stringToCategory(const QString categoryString);
    static QString getCategoryString(Category category);
    QSqlQueryModel *get_FilteredProducts(const QList<Produit::Category> selectedCategories);
    //bool ajouter();
    QString ajouter();
        QSqlQueryModel* afficher();
        bool supprimer(int);
        bool modifier();
        QSqlTableModel* getSortedModel();
        QString CategorytoString(Category );
        static int getLowStockProductCount()  ;
        Produit rechercherParId(int id);
        QMap<Category, int> getStatsParCategorie() const;
        QList<Produit> getProduitsEnAlerte();
         bool ajouterEnStock(int idProduit, int quantiteAAjouter);
        
private:
    int id;
    QString nom;
    float prix;
    Category category;
    int quantite;
};

#endif // PRODUIT_H
