#include "VideoDevice.hpp"

#include <QDebug>

VideoDevice::VideoDevice()
{
}

//retrieve list of connected camera devices
vector<const char*> VideoDevice::getDeviceList()
{
    vector<const char*> devices;

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
const char* VideoDevice::getDeviceName( const char *device_path )
{
    const char *device_name;

    struct udev *udev = udev_new(); //create new udev object
    struct udev_device *camera;

    //retrieve camera details from identified device
    camera = udev_device_new_from_syspath( udev, device_path );

    //get device's 'name' attribute
    device_name = udev_device_get_sysattr_value( camera, "name" );
    qDebug() << "VideoDevice: Device Name = " << device_name;

    return device_name;
}

//retrieve '/dev' file location of connected camera
const char* VideoDevice::getDeviceFile( const char *device_path )
{
    const char *file_name;

    struct udev *udev = udev_new(); //create new udev object
    struct udev_device *camera;

    //retrieve camera details from identified device
    camera = udev_device_new_from_syspath( udev, device_path );

    //get 'DEVNAME' property from device
    file_name = udev_device_get_property_value( camera, "DEVNAME" );
    qDebug() << "VideoDevice: Device File = " << file_name;

    return file_name;
}
