#ifndef CUSTOMCALENDAR_H
#define CUSTOMCALENDAR_H
#include <QWidget>
#include <QCalendarWidget>
#include <QMap>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QListView>
#include <QStringListModel>
class CustomCalendar : public QWidget
{
public:
    CustomCalendar();


    QSqlQueryModel* chercher_commande( QString searchCriteria);
    QSqlQueryModel* chercher_commande(const QDate& searchDate);
//QStringListModel* chercher_commande(QString searchCriteria);


        QCalendarWidget *calendarWidget;
        //CustomCalendar(QWidget *parent = nullptr);

};

#endif // CUSTOMCALENDAR_H
