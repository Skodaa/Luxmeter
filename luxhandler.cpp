
#include "luxhandler.h"
#include "thread"
#include <QTextEdit>
#include "logger.h"
#include <iostream>
#include <QSerialPortInfo>
#include <QString>

LuxHandler::LuxHandler(QObject *parent)
    : QObject{parent}
{

    this->l = new Logger();

    // setting the serial port parameters
    _serialLux.setBaudRate(QSerialPort::Baud115200);
    _serialLux.setDataBits(QSerialPort::Data8);
    _serialLux.setParity(QSerialPort::NoParity);
    _serialLux.setStopBits(QSerialPort::OneStop);
    _serialLux.setFlowControl(QSerialPort::NoFlowControl);

    // we will link the fact of receiving a data througt the port with a function
    connect(&_serialLux,&QSerialPort::readyRead, this, &LuxHandler::newData);

}

LuxHandler::~LuxHandler()
{
    // closing the connection when the class is closed
    closeConnection();
}

/**
 * @brief lux_thread::tryConnection
 * @param port : the port we try to connect to
 * @return boolean indicating if the connection was successful
 */
bool LuxHandler::tryConnection(QSerialPortInfo port)
{
    std::string data;
    // setting the port name for the serial connection
    _serialLux.setPortName(port.portName());
    // trying to open the connection
    if(!_serialLux.open(QIODevice::ReadWrite))
    {
        // if the connection failed logging an error and returning that this is not the good port
        this->l->write_debug(port.portName().toStdString());
        this->l->write_error("Connexion impossible");
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief lux_thread::newData
 * Function called every time a new data is received throught com port
 */
void LuxHandler::newData()
{
    // wait for 1 second before emiting a signal with the data received
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    emit gotNewData(_serialLux.readAll());
}

/**
 * @brief lux_thread::closeConnection
 * Function to close the communication with the COM port
 */
void LuxHandler::closeConnection()
{
    _serialLux.close();
}

/**
 * @brief lux_thread::openConnection
 * Function to open a connection with the COM port
 */
void LuxHandler::openConnection()
{
    // trying to connect and logging the result
    if(!_serialLux.open(QIODevice::ReadOnly))
    {
        this->l->write_error("Connexion impossible");
    }
    else
    {
        this->l->write_log("connexion etabli");
    }
}

/**
 * @brief lux_thread::isOpen
 * Function to check if the connection is open
 * @return a boolean depending if the connection is open or not
 */
bool LuxHandler::isOpen()
{
    return _serialLux.isOpen();
}

/**
 * @brief lux_thread::setCom
 * Function to set the com port of the connection
 * @param com
 */
void LuxHandler::setCom(QString com)
{
    _serialLux.setPortName(com);
}

/**
 * @brief lux_thread::setCombo
 * Function to set the combo box containing all the COM port name
 * @param combo : the combobox we want to set the port name in
 */
void LuxHandler::setCombo(QComboBox *combo)
{

    // A list of all the ports available
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    // adding every port available in the combobox
    for(const QSerialPortInfo &port : ports)
    {
        combo->addItem(port.portName());
    }
}
