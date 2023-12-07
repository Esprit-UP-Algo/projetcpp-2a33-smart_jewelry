#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Connection c;
    QFile styleFile( "C:/Users/Salim/Downloads/Atelier_Connexion modifff/Atelier_Connexion/style.qss" );

    styleFile.open( QFile::ReadOnly );

    // Apply the loaded stylesheet
    QString style = QLatin1String( styleFile.readAll() );
    a.setStyleSheet( style );

    bool test=c.createconnect();
    if(test)
    {w.show();
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);



    return a.exec();
}
