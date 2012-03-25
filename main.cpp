#include <QtGui/QApplication>
#include "GuiMainWindow.hpp"
#include "Capture.hpp"
#include "GuiMain.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //GuiMainWindow w;
    //w.show();

    //cv::namedWindow( "window", 1 ); //create new UI window ready for display

    Capture *camera = new Capture; //create new camera object
    GuiMain *window = new GuiMain( "window", camera );
    window->start(); //start window

    //window->displayVideo( camera );

    //cv::imshow( "window", frame ); //display frame in window

    while( true );

    //while( true )
    //{
    //    cv::Mat frame = camera->grabFrame();
    //    imshow( "window", frame );
    //    if( cv::waitKey( 20 ) >= 0 ) break; //check if user wants to exit
    //}

    delete camera; //disconnect camera before closing program
    delete window;

    return 0;
    //return a.exec();
}
