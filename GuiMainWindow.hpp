#ifndef GUIMAINWINDOW_HPP
#define GUIMAINWINDOW_HPP

#include "Capture.hpp"
#include "VideoBuffer.hpp"
#include "VideoDevice.hpp"
#include "VideoStream.hpp"
#include "DisplayStream.hpp"
#include "FaceTracking.hpp"
#include "StandController.hpp"

#include <QtGui>
#include <QTimer>

#include <opencv2/core/core.hpp>

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

        Capture *capture;
        VideoBuffer *video_buffer;
        VideoDevice *video_device;
        VideoStream *video_stream;
        DisplayStream *video_display;
        FaceTracking *face_tracking;
        StandController *stand;
        QTimer *timer_video_display;
        QTimer *timer_face_tracking;

        QMainWindow *main_window;
        QWidget *content;
        QVBoxLayout *v_layout;
        QHBoxLayout *h_layout;
        QMenu *menu_camera;
        QMenu *menu_edit;
        QComboBox *cmb_camera_select;
        QComboBox *cmb_device_select;
        QLabel *lbl_camera_output;

        QAction *action_start;
        QAction *action_quit;
        QAction *action_show_faces;

    public slots:
        void displayVideo();
        void displayFrame( cv::Mat frame );
        void toggleDisplayFaces();
        void setDevice( QString device_name );
        void setCamera( QString camera_name );
};

#endif // GUIMAINWINDOW_HPP
