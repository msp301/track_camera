#include "Application.hpp"

#include <opencv2/core/core.hpp>

Application::Application( int argc, char *argv[] ) :
    QApplication( argc, argv )
{
}

//reimplementation of QApplication::notify to correctly catch non-Qt errors
bool Application::notify( QObject *receiver, QEvent *event )
{
    try
    {
        //attempt to execute notify as normal
        return QApplication::notify( receiver, event );
    }
    catch( cv::Exception exception )
    {
        //catch errors produced by OpenCV
        //cv::Exception uses std::exception to output error to console
        return false;
    }
}
