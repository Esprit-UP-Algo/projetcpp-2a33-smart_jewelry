#include "statistic.h"
#include "ui_statistic.h"
#include "qcustomplot.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "client.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QRegExp>
#include <QStandardItemModel>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QTableWidgetSelectionRange>
#include<QPdfWriter>
#include<QPainter>
#include<QTextStream>
#include<QDebug>
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSeries>
#include <QPieSlice>
Statistic::Statistic(QWidget *parent) :
QDialog(parent),
ui(new Ui::Statistic)
{
ui->setupUi(this);
 MakeStat();
}




Statistic::Statistic(QVector<double>* ticks,QVector<QString> *labels)
{
QSqlQuery qry;
int i=0;
qry.exec("SELECT adresse FROM GEST_CLIENT");
while (qry.next())
{
    QString ID = qry.value(0).toString();
    i++;
    *ticks<<i;
    *labels <<ID;
}
}
void Statistic::MakeStat()
{
client cl;
        QBarSet *set0 = new QBarSet("adresse des clients ");

            *set0 << cl.statclient("tunis") << cl.statclient("sousse") << cl.statclient("sfax") << cl.statclient("nabeul");

            QColor color(0x6568F3);
            set0->setColor(color);

            QBarSeries *series = new QBarSeries();
            series->append(set0);




            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle("");
            chart->setAnimationOptions(QChart::SeriesAnimations);
            chart->setBackgroundVisible(false);

            QColor bgColor(0xF4DCD3);
                           chart->setBackgroundBrush(QBrush(QColor(bgColor)));

                           chart->setBackgroundVisible(true);
            QStringList categories;
            categories << "tunis" << "sousse" << "sfax" << "nabeul";
            QBarCategoryAxis *axis = new QBarCategoryAxis();
            axis->append(categories);
            chart->createDefaultAxes();
            chart->setAxisX(axis, series);

            QChartView *chartView = new QChartView(chart);

            chartView->setMinimumWidth(1000);
            chartView->setMinimumHeight(400);
            chartView->setParent(ui->frame_s);
            chart->legend()->setAlignment(Qt::AlignBottom);
            chartView->show();

}


Statistic::~Statistic()
{
delete ui;
}
