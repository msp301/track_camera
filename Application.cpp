#include "Application.hpp"

#include <QDebug>

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
    catch( int e )
    {
        qDebug() << "Error: Cannot run notify of event";
        return false;
    }
}
