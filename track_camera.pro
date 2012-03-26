#-------------------------------------------------
#
# Project created by QtCreator 2012-03-21T22:36:38
#
#-------------------------------------------------

QT       += core gui

TARGET = track_camera
TEMPLATE = app

LIBS += -L/usr/local/lib \
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
    GuiMain.cpp \
    FaceTracking.cpp \
    VideoBuffer.cpp

HEADERS  += Capture.hpp \
    GuiMain.hpp \
    FaceTracking.hpp \
    VideoBuffer.hpp

FORMS    += GuiMainWindow.ui
