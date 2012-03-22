#include <QtGui/QApplication>
#include "GuiMainWindow.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //GuiMainWindow w;
    //w.show();

    cv::VideoCapture capture( 0 ); //create camera interface
    cv::namedWindow( "window", 1 ); //create new UI window ready for display

    //check that camrea is ready before attempting to read
    if( capture.isOpened() )
    {
        //constantly sample video input and display frames to window
        for( ;; )
        {
            cv::Mat frame;
            capture >> frame; //read next frame from video capture

            cv::imshow( "window", frame ); //display frame in window
            if( cv::waitKey( 20 ) >= 0 ) break; //check if user wants to exit
        }
    }
    capture.release(); //disconnect camera before closing program

    return 0;
    //return a.exec();
}
