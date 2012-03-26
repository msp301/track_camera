#ifndef GUIMAIN_HPP
#define GUIMAIN_HPP

#include "Capture.hpp"

#include <QThread>

#include <string>

using namespace std;

class GuiMain : public QThread
{
    public:
        GuiMain( string name, VideoBuffer *buffer );
        ~GuiMain();
        void run();
    private:
        void close();

        string window_name;
        VideoBuffer *video_buffer;
};

#endif // GUIMAIN_HPP
