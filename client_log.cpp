#include "client_log.h"
#include "ui_client_log.h"
#include "client.h"
#include "arduino.h"
#include <QMessageBox>
client_log::client_log(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client_log)
{
    ui->setupUi(this);
    int ret=A.connect_arduino(); // lancer la connexion à arduino
     switch(ret){
     case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
         break;
     case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
        break;
     case(-1):qDebug() << "arduino is not available";
     }
      QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(on_push_id_clicked())); // permet de lancer
      //le slot update_label suite à la reception du signal readyRead (reception des données).
}

client_log::~client_log()
{
    delete ui;
}

void client_log::on_push_id_clicked()
{
    int id=ui->line_idd->text().toInt();
    //contrat_ar ca;
       client c;
    data=A.read_from_arduino();
   int d =data.toInt();
   qDebug() << d;


    if(c.doesIDExist(id) && d==1)
    {
      QMessageBox::information(nullptr, QObject::tr("valide"),
                   QObject::tr("cin et carte correct:accés valide .\n"
                                 "Click Cancel to exit."), QMessageBox::Cancel);

         QString adresse = c.getadresse_b(id);
      //QString adresseString = QString::number(adresse);
         // Send the temperature value to Arduino
         // Send the temperature value to Arduino
          QByteArray dataToSend = adresse.toUtf8();

           qDebug() << dataToSend;
           A.write_to_arduino(dataToSend);

  }


}
