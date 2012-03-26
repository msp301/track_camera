#ifndef GUIMAINWINDOW_HPP
#define GUIMAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
	class GuiMainWindow;
}

class GuiMainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit GuiMainWindow(QWidget *parent = 0);
		~GuiMainWindow();
		
	private:
		Ui::GuiMainWindow *ui;

    private slots:
        void displayVideo();
        void displayFrame( QImage frame );
};

#endif // GUIMAINWINDOW_HPP
