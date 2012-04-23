#include "StandController.hpp"

#include <QDebug>

StandController::StandController()
{
    serial_port = new QextSerialPort; //create serial port object
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

//set the port to use to connect to hardware controller
void StandController::setPort( QString port_name )
{
    //ensure given port name is valid
    foreach( QextPortInfo port, availablePorts() )
    {
        //check given port name is a valid device port
        if( port.physName == port_name )
        {
            serial_port->setPortName( port_name ); //set port name
            qDebug() << "Set Port Name = " << port_name;
        }
    }
}

//send face position data to hardware controller
void StandController::sendFaceData( vector<Coordinate> coordinates )
{

}
