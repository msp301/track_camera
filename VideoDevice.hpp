#ifndef VIDEODEVICE_HPP
#define VIDEODEVICE_HPP

#include <libudev.h>
#include <vector>

using namespace std;

class VideoDevice
{
    public:
        VideoDevice();
        vector<const char*> getDeviceList();
        const char* getDeviceName( const char *device_path );
        const char* getDeviceFile( const char *device_path );
};

#endif // VIDEODEVICE_HPP
