#include "GuiMainWindow.hpp"
#include "ui_GuiMainWindow.h"

#include "Capture.hpp"

#include <QtGui>

GuiMainWindow::GuiMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::GuiMainWindow)
{
	ui->setupUi(this);

    //QPushButton *btn_start_video = new QPushButton;
    connect( ui->btn_start_video, SIGNAL( clicked() ),
             this, SLOT( displayVideo() ) );
}

GuiMainWindow::~GuiMainWindow()
{
	delete ui;
}

//display video output to interface
void GuiMainWindow::displayVideo()
{
    Capture *camera = new Capture;

    while( true )
    {
        QImage frame = camera->grabConvertedFrame();
        displayFrame( frame );
    }
}

void GuiMainWindow::displayFrame( QImage frame )
{
    emit ui->lbl_camera_ouput->setPixmap( QPixmap::fromImage( frame ) );
}
