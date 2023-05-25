
#ifndef LUXHANDLER_H
#define LUXHANDLER_H

#include <QThread>
#include <QTextEdit>
#include <QLCDNumber>
#include <QSerialPort>
#include "logger.h"
#include "qcombobox.h"


class LuxHandler
{
public:
    LuxHandler();
    void openConnection();
    void closeConnection();
    bool isOpen();
    bool tryConnection(QSerialPortInfo port);
    void setCom(QString com);
    void setCombo(QComboBox *combo);

private slots:
    void NewData();

signals:
    void gotNewData(QByteArray data);

private:
    QSerialPort _serialLux;
    Logger *l;
    bool connected;
};

#endif // LUXHANDLER_H
