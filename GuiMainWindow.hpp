#ifndef GUIMAINWINDOW_HPP
#define GUIMAINWINDOW_HPP

#include "Capture.hpp"
#include "VideoBuffer.hpp"
#include "VideoStream.hpp"
#include "DisplayStream.hpp"
#include "FaceTracking.hpp"

#include <QMainWindow>

#include <opencv2/core/core.hpp>

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
        DisplayStream *video_display;
        FaceTracking *face_tracking;

    public slots:
        void displayVideo();
        void displayFrame( cv::Mat frame );
};

#endif // GUIMAINWINDOW_HPP
