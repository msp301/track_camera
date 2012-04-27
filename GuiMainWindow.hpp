#ifndef GUIMAINWINDOW_HPP
#define GUIMAINWINDOW_HPP

#include "Capture.hpp"
#include "VideoBuffer.hpp"
#include "VideoStream.hpp"
#include "DisplayStream.hpp"
#include "FaceTracking.hpp"
#include "StandController.hpp"

#include <QMainWindow>
#include <QTimer>

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
        void setupGui();
        void setupTimers();
        void createConnections();
        void createMenuConnections();

		Ui::GuiMainWindow *ui;
        Capture *capture;
        VideoBuffer *video_buffer;
        VideoStream *video_stream;
        DisplayStream *video_display;
        FaceTracking *face_tracking;
        StandController *stand;
        QTimer *timer_video_display;

    public slots:
        void displayVideo();
        void displayFrame( cv::Mat frame );
        void toggleDisplayFaces();
        void setDevice( QString device_name );
};

#endif // GUIMAINWINDOW_HPP
