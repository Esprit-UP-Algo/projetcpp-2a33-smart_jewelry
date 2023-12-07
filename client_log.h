#ifndef CLIENT_LOG_H
#define CLIENT_LOG_H

#include <QWidget>
#include <arduino.h>
namespace Ui {
class client_log;
}

class client_log : public QWidget
{
    Q_OBJECT

public:
    explicit client_log(QWidget *parent = nullptr);
    ~client_log();

private slots:
    void on_push_id_clicked();

private:
    Ui::client_log *ui;

    Arduino A;
     QByteArray data;

};

#endif // CLIENT_LOG_H
