#ifndef VIDEODEVICE_HPP
#define VIDEODEVICE_HPP

#include <libudev.h>
#include <vector>
#include <string>

using namespace std;

class VideoDevice
{
    public:
        VideoDevice();
        vector<string> getDeviceList();
        string getDeviceName( string device_path );
        string getDeviceFile( string device_path );
        int getDeviceNumber( string device_path );
        string deviceFileToPath( string device_file );
};

#endif // VIDEODEVICE_HPP
