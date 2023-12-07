#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "produit.h"
#include "commande.h"
#include "arduino.h"
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QPalette>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "customcalendar.h"
#include "operation.h"

//integration client

#include "client_log.h"
#include "statistic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateStockIcon(int stockLevel);

    void calculatePaymentStatistics();



private slots:
    void on_pb_ajouter_clicked();

    void on_pb_supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_comboBoxCategory_activated(const QString &arg1);


    void on_checkBox_4_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_stateChanged(int arg1);


    void on_le_id_rech_textChanged(const QString &arg1);

    void afficherStatsParCategorie();

    void afficherAlertes();

    

    
    void on_pushButtonAfficherAlertes_clicked();

    void on_pushButtonAjouterStock_clicked();
    void ajouterEnStock(int idProduit, int quantiteAAjouter);



    void on_pb_valider_clicked();



   // void on_pb_valider_2_clicked();



    void on_recherche_clicked(const QString &arg1);



  //  void on_pushButtonStatistics_clicked();





    //void on_calculatePaymentStatistics_activated(const QString &arg1);








    void on_cherche_cursorPositionChanged(int arg1, int arg2);



    void on_envoyer_clicked();





    void on_pdf_2_clicked();

    void on_historique_2_clicked();



    void on_cherche_2_textChanged(const QString &arg1);

    void on_tri_2_clicked();

    void on_modif_2_clicked();

    void on_supp_3_clicked();

    void on_calendarWidget_clicked(const QDate &date);
    void update_label();   // slot permettant la mise à jour du label état de la lampe 1,
    // ce slot est lancé à chaque réception d'un message de Arduino

    void on_pushButton_6_clicked();   // bouton ON

    void on_pushButton_4_clicked(); // bouton OFF

    void on_pushButton_3_clicked(); // bouton +

    void on_pushButton_5_clicked(); // bouton -




    void on_pushButtonstat_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();


    void on_creationBtn_clicked();
    void on_suppBtn_clicked();
    void on_modifBtn_clicked();
    void on_rechercheBtn_clicked();
    void on_sortBtn_clicked();
    void on_PDFBtn_clicked();
    void on_StatTypeBtn_clicked();
    void on_statEtatBtn_clicked();
    void on_SuiviBtn_clicked();
    void on_AccepteOpBtn_clicked();

    void on_pushButton_10_clicked();
    //integration client
    void on_ajouter_cl_clicked();
    void on_afficher_clicked();
    void on_supprimer_cl_clicked();
    void on_modifier_cl_clicked();
    void on_affiche_tab_activated(const QModelIndex &index);

    void on_chercher_textChanged(const QString &arg1);

    void on_tri_cl_clicked();

    void on_pdf_cl_clicked();

    void on_stat_cl_clicked();

    void on_qrcode_cl_clicked();

    void on_push_id_clicked();

private:
    Ui::MainWindow *ui;
    Produit P;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QStatusBar *statusBar;
        QLabel *stockIconLabel;
        QLabel *notificationLabel;
            int notificationCount;
            void effectuerRechercheParId(const QString &id);
            void genererPDF();
            Arduino arduino;
            QByteArray data;

            Commande C;

            Commande etmp;
             QList<Commande> commandes;
              QNetworkAccessManager *networkManager;
              CustomCalendar cal;
              Arduino A;
              operation Op;
              //integration client
              Statistic * statistic ;
              client_log *lo;
};

#endif // MAINWINDOW_H
