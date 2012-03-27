#ifndef GUIMAINWINDOW_HPP
#define GUIMAINWINDOW_HPP

#include "Capture.hpp"
#include "VideoBuffer.hpp"
#include "VideoStream.hpp"

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
        void createConnections();

		Ui::GuiMainWindow *ui;
        Capture *capture;
        VideoBuffer *video_buffer;
        VideoStream *video_stream;

    private slots:
        void displayVideo();
        void displayFrame( QImage frame );
};

#endif // GUIMAINWINDOW_HPP
