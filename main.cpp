#include <QtGui/QApplication>
#include "GuiMainWindow.hpp"

#include "StandController.hpp"

int main(int argc, char *argv[])
{
    QApplication app( argc, argv ); //create new Qt application container
    GuiMainWindow window; //create main window
    //window.show(); //display main window

    StandController *stand = new StandController;
    stand->availablePorts();

    return app.exec(); //start application
}
