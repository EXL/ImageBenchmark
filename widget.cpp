#include "widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

#include <ctime> // for clock()

#ifdef _DEBUG
#include <QDebug>
#endif

#define VERSION_APP 0.2

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
    setFixedSize(sizeHint().width() * 1.2, sizeHint().height() * 2);
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
                                                       "<p>Emails: <a href='mailto:exlmotodev@gmail.com'>exlmotodev@gmail.com</a>, <a href='mailto:exl@bk.ru'>exl@bk.ru</a></p>"
                                                       "<p>(c) EXL, <a href='http://forum.motofan.ru'>forum.motofan.ru</a>, 2013</p>").arg(VERSION_APP));
}

void Widget::refreshSlot()
{
    delete tableWidget;
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
    int vecSize;
    const int columnConst = 2;

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

    QVector<QVector<QTableWidgetItem *> > vecWidgetPointers2D;
    QVector<QWidget *> vecWidget;
    for (it = files.begin(), i = 0; it != files.end(); ++i, ++it, ++image_c) {
        vecWidget.push_back(createImageLabel(QFileInfo(path, *it).absoluteFilePath()));
        QVector<QTableWidgetItem *> tmpVec;
        tmpVec.push_back(createTypeLabel(QString(QFileInfo(path, *it).completeSuffix().toUpper()), i == 0));
        tmpVec.push_back(getTimeLabel());

        vecWidgetPointers2D.push_back(tmpVec);
    }

    vecSize = vecWidget.size();

    tableWidget->setRowCount(vecSize);
    tableWidget->setColumnCount(columnConst + 1);
    tableWidget->setColumnWidth(0, img_size.width());

    for (int i = 0; i < vecSize; ++i) {
        tableWidget->setRowHeight(i, img_size.height());
        tableWidget->setCellWidget(i, 0, vecWidget[i]);
        for (int j = 0; j < columnConst; ++j) {
            tableWidget->setItem(i, j + 1, vecWidgetPointers2D[i][j]);
        }
    }

    QStringList tableLabels;
    tableLabels << tr("Image") << tr("Type (format)");
#ifndef CROSSPLATFORM
    tableLabels << tr("Clocks / 10^4");
#else
    tableLabels << tr("Milliseconds");
#endif

    tableWidget->setHorizontalHeaderLabels(tableLabels);
    tableWidget->setSortingEnabled(true);

    return (image_c > 0) ? true : false;
}

/***************************************************************************
 * (c) Kai Tietz-2
 * http://mingw.5.n7.nabble.com/Trouble-with-rdtsc-on-amd64-in-32-bit-environment-tp21369p21372.html
 ***************************************************************************/

quint64 Widget::__rdtsc()
{
#ifndef CROSSPLATFORM
#ifdef _WIN64
    unsigned __int64 val1, val2;
#else
    unsigned int val1, val2;
#endif
    __asm__ __volatile__ (
                "rdtsc"
                : "=a" (val1), "=d" (val2));
    return ((quint64)val1) | (((quint64)val2) << 32);
#else
    return ::clock();
#endif
}

/**************************************************************************/

QLabel *Widget::createImageLabel(const QString &fileName)
{
    quint64 start = __rdtsc();

    QLabel *imageLabel = new QLabel(this);
    imageLabel->setScaledContents(true);
    imageLabel->setToolTip(fileName);
    if (!fileName.isEmpty()) {
        QImage image(fileName);

        if (image.isNull()) {
            QMessageBox::information(this,
                                     tr("Image Benchmark"),
                                     tr("Cannot load %1.").arg(fileName));
            throw ImagesNotFoundExcp("Widget::createImageLabel()->!fileName.isEmpty(): Cannot Load Image Files!");
        }

        imageLabel->setPixmap(QPixmap::fromImage(image).scaled(img_size));
    }

    quint64 end = __rdtsc();
    time = end - start;

    imageLabel->setStyleSheet("QLabel { background: #CEEACE; }");

    return imageLabel;
}

QTableWidgetItem *Widget::createTypeLabel(const QString &fileType, bool qInit)
{
    QTableWidgetItem *typeLabel = new QTableWidgetItem(qInit ? fileType + " (INIT)" : fileType);
    typeLabel->setTextAlignment(Qt::AlignCenter);
    typeLabel->setBackgroundColor(QColor("#CEEACE"));
    return typeLabel;
}

FloatTableWidgetItem *Widget::getTimeLabel()
{
#ifndef CROSSPLATFORM
    FloatTableWidgetItem *timeLabel = new FloatTableWidgetItem(QString::number(time / 10000, 'f', 3));
#else
    FloatTableWidgetItem *timeLabel = new FloatTableWidgetItem(QString::number(time / CLOCKS_PER_SEC * 1000, 'f', 3));
#endif
    timeLabel->setTextAlignment(Qt::AlignCenter);
    timeLabel->setBackgroundColor(QColor("#CEEACE"));
    return timeLabel;
}

void Widget::createWidgets()
{
    tableWidget = new QTableWidget(this);

    if (!createTableGrid("images")) {
#ifdef _DEBUG
        qDebug() << "Fatal Error: image(s) not found!";
#endif
        QMessageBox::critical(this,
                              tr("Image Benchmark"),
                              tr("Cannot load image files in the \"./images\" folder!\n"
                                 "Check the existence of the directory \"./images\".\n"
                                 "Check the existence of the images files there."));
        throw ImagesNotFoundExcp("Widget::createWidgets()->!createTableGrid(\"images\"): Cannot Load Image Files!");
    }

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tableWidget);
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
    fileMenu = new QMenu(this);
    fileMenu->setTitle(tr("&File"));
    fileMenu->addAction(refreshAction);

    fileMenu->addSeparator();

    fileMenu->addAction(quitAction);

    helpMenu = new QMenu(this);
    helpMenu->setTitle(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    menuBar = new QMenuBar(this);
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(helpMenu);

    mainLayout->setMenuBar(menuBar);
}

Widget::~Widget()
{
    /* Empty Destructor */
}
