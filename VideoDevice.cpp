#include "VideoDevice.hpp"

#include <QDebug>

VideoDevice::VideoDevice()
{
}

//retrieve list of connected camera devices
vector<string> VideoDevice::getDeviceList()
{
    vector<string> devices;

    struct udev_list_entry *device_entries, *device;
    struct udev_enumerate *enumerate;

    struct udev *udev = udev_new(); //create new udev object
    enumerate = udev_enumerate_new( udev ); //create device enumerator

    //add device subsystem filter to find 'video4linux' compatible devices
    udev_enumerate_add_match_subsystem( enumerate, "video4linux" );
    udev_enumerate_scan_devices( enumerate ); //scan through devices
    device_entries = udev_enumerate_get_list_entry( enumerate );

    //retrieve device names from list
    udev_list_entry_foreach( device, device_entries )
    {
        const char *device_path;

        device_path = udev_list_entry_get_name( device ); //locate camera device
        qDebug() << "VideoDevice: Device Path = " << device_path;

        devices.push_back( device_path ); //add device to devices list
    }

    return devices;
}

//retrieve product name of connected camera
string VideoDevice::getDeviceName( string device_path )
{
    const char *device_name;

    struct udev *udev = udev_new(); //create new udev object
    struct udev_device *camera;

    //retrieve camera details from identified device
    camera = udev_device_new_from_syspath( udev, device_path.c_str() );

    //get device's 'name' attribute
    device_name = udev_device_get_sysattr_value( camera, "name" );
    qDebug() << "VideoDevice: Device Name = " << device_name;

    string name = device_name; //convert to std::string
    return name;
}

//retrieve '/dev' file location of connected camera
string VideoDevice::getDeviceFile( string device_path )
{
    string file_name;

    struct udev *udev = udev_new(); //create new udev object
    struct udev_device *camera;

    //retrieve camera details from identified device
    camera = udev_device_new_from_syspath( udev, device_path.c_str() );

    //get 'DEVNAME' property from device
    file_name = udev_device_get_property_value( camera, "DEVNAME" );
    qDebug() << "VideoDevice: Device File = " << file_name.c_str();

    return file_name;
}

//retrieve number of device, based on its location in '/dev'
int VideoDevice::getDeviceNumber( string device_path )
{
    int device_num;
    string file_name = getDeviceFile( device_path.c_str() );
    size_t position = 0; //initialise starting position

    //find position of device number in device's file name
    position = file_name.find_first_of( "0123456789", position );
    device_num = file_name.at( position ) - 48; //convert to integer

    qDebug() << "Retrieved Device Number = " << device_num;

    return device_num;
}

//retrieve a device's system path using its device file path
string VideoDevice::deviceFileToPath( string device_file )
{
    string device_path;
    vector<const char*> device_paths;
    struct udev_list_entry *device, *devices;
    struct udev_enumerate *enumerate;

    struct udev *udev = udev_new(); //create new udev object
    enumerate = udev_enumerate_new( udev ); //create device enumerator

    //add device 'DEVNAME' property filter to find device with given file path
    udev_enumerate_add_match_property( enumerate, "DEVNAME", device_file.c_str() );
    udev_enumerate_scan_devices( enumerate ); //find matching devices
    devices = udev_enumerate_get_list_entry( enumerate );

    //move device path results to a vector where data can be validated
    udev_list_entry_foreach( device, devices )
    {
        const char *path;

        path = udev_list_entry_get_name( device ); //locate camera device
        device_paths.push_back( path ); //add device to devices list
    }

    //ensure only one device result has been returned
    if( device_paths.size() != 1 )
    {
        int size = device_paths.size();
        qDebug() << "VideoDevice: Error: Device file produced"
                    << "invalid number of results" << size;
    }
    else
    {
        device_path = device_paths.front(); //set device path ready for return
    }

    return device_path;
}
