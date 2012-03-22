#include <QtGui/QApplication>
#include "GuiMainWindow.hpp"
#include "Capture.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //GuiMainWindow w;
    //w.show();

    cv::namedWindow( "window", 1 ); //create new UI window ready for display

    Capture *camera = new Capture; //create new camera object

    //check that camera is ready before attempting to read
    if( camera->isReady() )
    {
        //constantly sample video input and display frames to window
        for( ;; )
        {
            cv::Mat frame;
            frame = camera->grabFrame(); //read next frame from camera

            cv::imshow( "window", frame ); //display frame in window
            if( cv::waitKey( 20 ) >= 0 ) break; //check if user wants to exit
        }
    }
    delete camera; //disconnect camera before closing program

    return 0;
    //return a.exec();
}
