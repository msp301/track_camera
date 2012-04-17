#include "StandController.hpp"

#include <QDebug>

StandController::StandController()
{
    serial_port = new QextSerialPort; //create serial port object
    ports = new QextSerialEnumerator; //create access to all available ports
}

QList<QextPortInfo> StandController::availablePorts()
{
    QList<QextPortInfo> port_list = ports->getPorts(); //get serial port list

    foreach( QextPortInfo port, port_list )
    {
        qDebug() << "Port Name = " << port.physName;
    }

    return port_list;
}
