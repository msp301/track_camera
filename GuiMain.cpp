#include "GuiMain.hpp"

#include <opencv2/highgui/highgui.hpp>

#include <iostream>

GuiMain::GuiMain( string name, Capture *camera )
{
    input = camera;
    window_name = name /*"Track Camera"*/;
    cv::namedWindow( window_name, 1 );

    //typedef void ( *CLOSE ) ( void );
    //CLOSE callbackClose = close();
    //cv::createButton( "btn_close", ( this->close() ) );
}

GuiMain::~GuiMain()
{
    delete input; //clean pointer to Capture instance
    cv::destroyWindow( window_name );
}

void GuiMain::run()
{
    while( /*int i=0; i<50; i++*/ true )
    {
        //std::cout << "i = " << i << endl;
        cv::Mat frame = input->grabFrame();
        cv::imshow( window_name, frame );
        cv::waitKey( 20 );
    }
    //cv::destroyAllWindows();
    this->quit();
}

void GuiMain::close()
{
    this->quit();
}
