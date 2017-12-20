MOC_DIR = .moc
OBJECTS_DIR = .obj

TEMPLATE = app
CONFIG += qt warn_on release thread
SOURCES = field.cpp
TARGET = field
CONFIG -= app_bundle

INCLUDEPATH += /usr/local/include/opencv2/

enseirb {
	INCLUDEPATH += /opt/opencv/include/opencv2
	QMAKE_LFLAGS += -Wl,-R/opt/opencv/lib -L/opt/opencv/lib
}


LIBS = \
-L/opt/opencv/lib/ \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_core \
-lopencv_features2d \
-lopencv_flann \
-lopencv_calib3d \
-lopencv_video
