#include "StandController.hpp"

StandController::StandController()
{
    serial_port = new QextSerialPort( ); //create serial port object
    ports = new QextSerialEnumerator; //create access to all available ports
}

//access list of available supported devices
QList<QextPortInfo> StandController::availablePorts()
{
    QList<QextPortInfo> port_list;

    //check all serial ports for available supported devices
    foreach( QextPortInfo port, ports->getPorts() )
    {
        //Arduino devices are identified by "/dev/ttyACM" + device number
        if( !port.physName.contains( "ACM" ) ) continue; //skip non-arduino devices

        port_list.push_back( port ); //add identified port to port list
    }

    return port_list; //return available port list
}
