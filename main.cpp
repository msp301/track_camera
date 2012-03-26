#include <QtGui/QApplication>
#include "GuiMainWindow.hpp"
#include "Capture.hpp"
#include "GuiMain.hpp"
#include "FaceTracking.hpp"

#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
    VideoBuffer *buffer = new VideoBuffer; //create new video buffer
    Capture *camera = new Capture( buffer ); //create new camera object
    camera->start(); //start capturing video stream from camera

    GuiMain *window = new GuiMain( "window", buffer );
    window->start(); //start window

    //FaceTracking *tracker = new FaceTracking( camera );
    //tracker->start();

    while( true )
    {
        //char key;
        //std::cin >> key;
        //if( key  == 'q' ) break;
    }

    delete camera; //disconnect camera before closing program
    delete window;

    return 0;
}
