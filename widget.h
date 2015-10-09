#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QSize>
#include <QMenu>
#include <QAction>
#include <QMenuBar>

struct ImagesNotFoundExcp {
    ImagesNotFoundExcp(const QString &a_what) : what(a_what) { }
    QString getWhat() const { return what; }

private:
    const QString what;
};

class Widget : public QWidget
{
    Q_OBJECT

    QSize img_size;

    uint image_c;
    double time;

    QGridLayout *mainLayout;
    QGridLayout *minorLayout;

    QWidget *scrollWidget;

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *quitAction;
    QAction *refreshAction;
    QAction *aboutAction;
    QAction *aboutQtAction;

    QScrollArea *scrollArea;

    bool createTableGrid(const QString &path);

    quint64 __rdtsc(void);

    QLabel *createImageLabel(const QString &fileName);
    QLabel *createTypeLabel(const QString &fileType, bool qInit);
    QLabel *getTimeLabel();

    void createWidgets();
    void createActions();
    void createMenus();
    void setWidgetSettings();
private slots:
    void aboutDialog();
    void refreshSlot();
public:
    Widget(QWidget *parent = 0);
    ~Widget();
};

#endif // WIDGET_H
