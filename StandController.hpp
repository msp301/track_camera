#ifndef STANDCONTROLLER_HPP
#define STANDCONTROLLER_HPP

#include "Coordinate.hpp"

#include <qextserialport.h>
#include <qextserialenumerator.h>

#include <vector>

using namespace std;

class StandController
{
    public:
        StandController();
        ~StandController();
        QList<QextPortInfo> availablePorts();
        void setPort( QString port_name );
        void sendFaceData( Coordinate coordinate );
    private:
        QextSerialPort *serial_port;
        QextSerialEnumerator *ports;
};

#endif // STANDCONTROLLER_HPP
