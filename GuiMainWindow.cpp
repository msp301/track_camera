#include "GuiMainWindow.hpp"
#include "ui_GuiMainWindow.h"

#include "Capture.hpp"

#include <QtGui>

GuiMainWindow::GuiMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::GuiMainWindow)
{
	ui->setupUi(this);

    video_buffer = new VideoBuffer; //create video buffer for captured frames
    video_stream = new VideoStream( video_buffer ); //create video stream handler

    createConnections(); //setup UI callback connections

    //QPushButton *btn_start_video = new QPushButton;
}

GuiMainWindow::~GuiMainWindow()
{
	delete ui;
}

//create all required connections between GUI elements
void GuiMainWindow::createConnections()
{
    connect( ui->btn_start_video, SIGNAL( clicked() ),
             this, SLOT( displayVideo() ) );
}

//display video output to interface
void GuiMainWindow::displayVideo()
{
    video_stream->start(); //start video stream
}

void GuiMainWindow::displayFrame( QImage frame )
{
    emit ui->lbl_camera_ouput->setPixmap( QPixmap::fromImage( frame ) );
}
