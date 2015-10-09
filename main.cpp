#include "widget.h"
#include <QApplication>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int ret = 1;
    try {
        Widget widget;
        widget.show();
        ret = app.exec();
    } catch (ImagesNotFoundExcp &e) {
        qDebug() << e.getWhat();
        app.closeAllWindows();
        ret = 2;
        app.exit(ret);
    }
    return ret;
}
