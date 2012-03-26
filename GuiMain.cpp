#include "GuiMain.hpp"

#include <opencv2/highgui/highgui.hpp>

GuiMain::GuiMain( string name, VideoBuffer *buffer )
{
    video_buffer = buffer;
    window_name = name /*"Track Camera"*/;
    cv::namedWindow( window_name, 1 );
}

GuiMain::~GuiMain()
{
    delete video_buffer; //clean pointer to Capture instance
    cv::destroyWindow( window_name );
}

void GuiMain::run()
{
    while( true )
    {
        //cv::Mat frame = input->grabFrame();
        cv::Mat frame = video_buffer->read();
        cv::imshow( window_name, frame );
        cv::waitKey( 20 );
    }
}

void GuiMain::close()
{
    this->quit();
}
