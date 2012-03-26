#ifndef GUIMAIN_HPP
#define GUIMAIN_HPP

#include "Capture.hpp"

#include <QThread>

#include <string>

using namespace std;

class GuiMain : public QThread
{
    public:
        GuiMain( string name, Capture *camera );
        ~GuiMain();
        void run();
        void displayVideo( Capture *camera );
    private:
        void close();

        string window_name;
        Capture *input;
};

#endif // GUIMAIN_HPP
