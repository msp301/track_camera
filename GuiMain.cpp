#include "GuiMain.hpp"

#include <opencv2/highgui/highgui.hpp>

#include <iostream>

GuiMain::GuiMain( string name, Capture *camera )
{
    input = camera;
    window_name = name /*"Track Camera"*/;
    cv::namedWindow( window_name, 1 );
}

GuiMain::~GuiMain()
{
    delete input; //clean pointer to Capture instance
    cv::destroyWindow( window_name );
}

void GuiMain::run()
{
    while( true )
    {
        cv::Mat frame = input->grabFrame();
        cv::imshow( window_name, frame );
        cv::waitKey( 20 );
    }
}

void GuiMain::close()
{
    this->quit();
}
