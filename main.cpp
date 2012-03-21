#include <QtGui/QApplication>
#include "GuiMainWindow.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GuiMainWindow w;
	w.show();
	
	return a.exec();
}
