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
# win32-g++:QMAKE_LFLAGS += -static -static-libgcc
#-------------------------------------------------

DEFINES += CROSSPLATFORM

# Deploy ./images/ catalog in build directory
# http://dragly.org/2013/11/05/copying-data-files-to-the-build-directory-when-working-with-qmake/
D_IN = $$PWD/images
D_OUT = $$OUT_PWD

win32: {
    D_OUT = $$D_OUT/images
    D_IN ~= s,/,\\,g
    D_OUT ~= s,/,\\,g

    DEFINES -= CROSSPLATFORM
}

deploy.commands = $(COPY_DIR) $$D_IN $$D_OUT
first.depends = $(first) deploy
export(first.depends)
export(deploy.commands)
QMAKE_EXTRA_TARGETS += first deploy

# For Debug
QMAKE_CXXFLAGS_DEBUG += -D_DEBUG

# For icon
win32:RC_FILE = imgbench_win.rc

TARGET = ImageBench
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

RESOURCES += \
    imgbench_qt.qrc

OTHER_FILES += \
    imgbench_win.rc
