#include "Application.hpp"
#include "GuiMainWindow.hpp"

int main(int argc, char *argv[])
{
    Application app( argc, argv ); //create new Qt application container
    GuiMainWindow window; //create main window
    window.show(); //display main window

    return app.exec(); //start application
}
