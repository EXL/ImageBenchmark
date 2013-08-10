#-------------------------------------------------
#
# Project created by QtCreator 2013-08-10T21:34:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#-------------------------------------------------
# For static build (need static Qt librares)
# Warning: please use qt_ru.qm coincident with the Qt version
# QMAKE_LFLAGS += -static -static-libgcc
#-------------------------------------------------

# For Debug
QMAKE_CXXFLAGS_DEBUG += -D_DEBUG

# For icon
win32-g++:RC_FILE = imgbench_win.rc

TARGET = ImageBench
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

RESOURCES += \
    imgbench_qt.qrc

OTHER_FILES += \
    imgbench_win.rc
