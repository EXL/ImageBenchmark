#include "widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

#ifdef _DEBUG
#include <QDebug>
#endif

#define VERSION_APP 0.1

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    image_c = 0;

    img_size.setHeight(48);
    img_size.setWidth(48);

    createWidgets();
    createActions();
    createMenus();

    setWidgetSettings();

    refreshSlot();
}

void Widget::setWidgetSettings()
{
    setLayout(mainLayout);
    setWindowTitle(tr("Image Benchmark"));
    setWindowIcon(QIcon("://icons/imgbench.png"));
    setContentsMargins(-9, -9, -9, -9);
    setFixedSize(240, 320);
}

void Widget::aboutDialog()
{
    QMessageBox::about(this, tr("Image Benchmark"), tr("<p><b>Version %1</b></p>"
                                                       "<p>Application and source code available under license GPL v.3.0:</p>"
                                                       "<p><a href='http://www.gnu.org/licenses/gpl.html'>GNU GENERAL PUBLIC LICENSE</a></p>"
                                                       "<p><a href='https://github.com/EXL/ImageBenchmark'>Source Code on GitHub</a></p>"
                                                       "<p>Program to test the speed of the output of different image formats to Qt Library.</p>"
                                                       "<p>Supported formats: *.bmp, *.gif, *.jpeg, *.jpg, *.mng, *.pbm, *.pgm, *.png, *.ppm, "
                                                       "*.svg, *.svgz, *.tif, *.tiff, *.xbm, *.xpm.</p>"
                                                       "<p>Emails: <a href='mailto:exlmotodev@google.com'>exlmotodev@google.com</a>, <a href='mailto:exl@bk.ru'>exl@bk.ru</a></p>"
                                                       "<p>(c) EXL, <a href='http://forum.motofan.ru'>forum.motofan.ru</a>, 2013</p>").arg(VERSION_APP));
}

void Widget::refreshSlot()
{
    delete minorLayout;
    delete scrollWidget;
    delete scrollArea;
    delete mainLayout;

    createWidgets();

    mainLayout->setMenuBar(menuBar);
    setLayout(mainLayout);
}

bool Widget::createTableGrid(const QString &path)
{
    QDir dir(path);

    QStringList::Iterator it;
    int i;

    QStringList nameFilter;
    nameFilter << "*.png"
               << "*.jpg"
               << "*.jpeg"
               << "*.bmp"
               << "*.gif"
               << "*.tiff"
               << "*.tif"
               << "*.pbm"
               << "*.ico"
               << "*.mng"
               << "*.pgm"
               << "*.ppm"
               << "*.svg"
               << "*.svgz"
               << "*.tga"
               << "*.xbm"
               << "*.xpm";

    QStringList files = dir.entryList(nameFilter,
                                      QDir::Files);

    for(it = files.begin(), i = 1; it != files.end(); ++i, ++it, ++image_c){
        minorLayout->addWidget(createImageLabel(QFileInfo(path, *it).absoluteFilePath()), i, 0, Qt::AlignCenter);
        minorLayout->addWidget(createTypeLabel(QString(QFileInfo(path, *it).completeSuffix().toUpper()), i == 1), i, 1, Qt::AlignCenter);
        minorLayout->addWidget(getTimeLabel(), i, 2, Qt::AlignCenter);
    }

    return (image_c > 0) ? true : false;
}

/***************************************************************************
 * (c) Kai Tietz-2
 * http://mingw.5.n7.nabble.com/Trouble-with-rdtsc-on-amd64-in-32-bit-environment-tp21369p21372.html
 ***************************************************************************/

quint64 Widget::__rdtsc()
{
#ifdef _WIN64
    unsigned __int64 val1, val2;
#else
    unsigned int val1, val2;
#endif
    __asm__ __volatile__ (
                "rdtsc"
                : "=a" (val1), "=d" (val2));
    return ((quint64)val1) | (((quint64)val2) << 32);
}

/**************************************************************************/

QLabel *Widget::createImageLabel(const QString &fileName)
{
    quint64 start = __rdtsc();

    QLabel *imageLabel = new QLabel;
    imageLabel->setScaledContents(true);
    imageLabel->setToolTip(fileName);
    if (!fileName.isEmpty()) {
        QImage image(fileName);

        if (image.isNull()) {
            QMessageBox::information(this,
                                     tr("Image Benchmark"),
                                     tr("Cannot load %1.").arg(fileName));
            exit(1);
        }

        imageLabel->setPixmap(QPixmap::fromImage(image).scaled(img_size));
    }

    quint64 end = __rdtsc();
    time = end - start;

    return imageLabel;
}

QLabel *Widget::createTypeLabel(const QString &fileType, bool qInit)
{
    QLabel *typeLabel = new QLabel;
    qInit ? typeLabel->setText(fileType + " (INIT)") : typeLabel->setText(fileType);
    return typeLabel;
}

QLabel *Widget::getTimeLabel()
{
    QLabel *timeLabel = new QLabel;
    timeLabel->setText(QString("%1").arg((float)time / 10000) + tr(" Pts"));
    return timeLabel;
}

void Widget::createWidgets()
{
    minorLayout = new QGridLayout;

    minorLayout->addWidget(new QLabel(tr("Image")), 0, 0, Qt::AlignCenter);
    minorLayout->addWidget(new QLabel(tr("Type (format)")), 0, 1, Qt::AlignCenter);
    minorLayout->addWidget(new QLabel(tr("Clocks / 10^4")), 0, 2, Qt::AlignCenter);

    if (!createTableGrid("images")) {
#ifdef _DEBUG
        qDebug() << "Fatal Error: image(s) not found!";
#endif
        QMessageBox::critical(this,
                              tr("Image Benchmark"),
                              tr("Cannot load image files in the \"./images\" folder!\n"
                                 "Check the existence of the directory \"./images\".\n"
                                 "Check the existence of the images files there."));
        exit(2);
    }

    scrollWidget = new QWidget;
    scrollWidget->setLayout(minorLayout);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollWidget);

    mainLayout = new QGridLayout;
    mainLayout->addWidget(scrollArea, 0, 0);
}

void Widget::createActions()
{
    refreshAction = new QAction(this);
    refreshAction->setText(tr("&Refresh"));
    refreshAction->setShortcut(Qt::Key_R);
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(refreshSlot()));

    quitAction = new QAction(this);
    quitAction->setText(tr("&Quit"));
    quitAction->setShortcut(Qt::Key_F10);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(this);
    aboutAction->setText(tr("&About"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutDialog()));

    aboutQtAction = new QAction(this);
    aboutQtAction->setText(tr("A&bout Qt Library"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void Widget::createMenus()
{
    fileMenu = new QMenu;
    fileMenu->setTitle(tr("&File"));
    fileMenu->addAction(refreshAction);

    fileMenu->addSeparator();

    fileMenu->addAction(quitAction);

    helpMenu = new QMenu;
    helpMenu->setTitle(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    menuBar = new QMenuBar;
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(helpMenu);

    mainLayout->setMenuBar(menuBar);
}

Widget::~Widget()
{
    /* Empty Destructor */
}
