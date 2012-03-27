#include "GuiMain.hpp"

#include <opencv2/highgui/highgui.hpp>

#include <iostream>

GuiMain::GuiMain( string name, VideoBuffer *buffer )
{
    video_buffer = buffer;
    window_name = name /*"Track Camera"*/;
    cv::namedWindow( window_name, 1 );
    cout << "GuiMain: Created window instance" << endl;
}

GuiMain::~GuiMain()
{
    delete video_buffer; //clean pointer to Capture instance
    cv::destroyWindow( window_name );
}

void GuiMain::run()
{
    cout << "GuiMain: Started GUI thread" << endl;
    while( true )
    {
        //cv::Mat frame = input->grabFrame();
        cv::Mat frame = video_buffer->read();

        //ensure frame given frame is defined before continuing to display
        if( !frame.empty() )
        {
            cv::imshow( window_name, frame );
        }
        cv::waitKey( 20 );
    }
}

void GuiMain::close()
{
    this->quit();
}
