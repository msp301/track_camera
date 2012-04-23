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
        QList<QextPortInfo> availablePorts();
        void setPort( QString port_name );
        void sendFaceData( vector<Coordinate> coordinates );
    private:
        QextSerialPort *serial_port;
        QextSerialEnumerator *ports;
};

#endif // STANDCONTROLLER_HPP
