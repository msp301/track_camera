#-------------------------------------------------
#
# Project created by QtCreator 2012-03-21T22:36:38
#
#-------------------------------------------------

include( libs/qextserialport/src/qextserialport.pri )

QT       += core gui

TARGET = track_camera
TEMPLATE = app

LIBS += -ludev \
	-L/usr/local/lib \
	-lopencv_core \
	-lopencv_imgproc \
	-lopencv_highgui \
	-lopencv_ml \
	-lopencv_video \
	-lopencv_features2d \
	-lopencv_calib3d \
	-lopencv_objdetect \
	-lopencv_contrib \
	-lopencv_legacy \
	-lopencv_flann

SOURCES += main.cpp \
    Capture.cpp \
	GuiMainWindow.cpp \
	FaceTracking.cpp \
    VideoBuffer.cpp \
    VideoStream.cpp \
	DisplayStream.cpp \
	StandController.cpp \
    VideoDevice.cpp \
    TestCapture.cpp

HEADERS  += Capture.hpp \
	GuiMainWindow.hpp \
    FaceTracking.hpp \
    VideoBuffer.hpp \
    VideoStream.hpp \
	DisplayStream.hpp \
	StandController.hpp \
	Coordinate.hpp \
    VideoDevice.hpp \
    TestCapture.hpp
