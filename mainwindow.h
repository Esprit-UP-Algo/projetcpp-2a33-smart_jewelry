#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "operation.h"
#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
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
//    void on_pushButton_12_clicked();

private:
    Ui::MainWindow *ui;
    operation Op ;
};
#endif // MAINWINDOW_H
