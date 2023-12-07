#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "operation.h"
#include <QMessageBox>
#include <QObject>
#include <QIntValidator>
#include <QRegExp>
#include <QPainter>
#include <QPdfWriter>
#include <QDebug>
#include <QFileDialog>
#include <QValidator>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QSqlRecord>
#include <QMetaObject>
#include <QCoreApplication>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableViewAffichage->setModel(Op.afficher());
}

MainWindow::~MainWindow()
{
    delete ui;
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
