#ifndef STANDCONTROLLER_HPP
#define STANDCONTROLLER_HPP

#include <qextserialport.h>
#include <qextserialenumerator.h>

class StandController
{
    public:
        StandController();
        QList<QextPortInfo> availablePorts();
        void setPort( QString port_name );
    private:
        QextSerialPort *serial_port;
        QextSerialEnumerator *ports;
};

#endif // STANDCONTROLLER_HPP
