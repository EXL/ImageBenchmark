#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QSize>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QTableWidget>
#include <QTableWidgetItem>

#ifdef _DEBUG
#include <QDebug>
#endif

struct FloatTableWidgetItem : public QTableWidgetItem
{
    explicit FloatTableWidgetItem(const QString &text) : QTableWidgetItem(text) { }

    bool operator <(const QTableWidgetItem &other) const
    {
        return text().toFloat() < other.text().toFloat();
    }
};

struct ImagesNotFoundExcp
{
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

    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *quitAction;
    QAction *refreshAction;
    QAction *aboutAction;
    QAction *aboutQtAction;

    bool createTableGrid(const QString &path);

    quint64 __rdtsc(void);

    QLabel *createImageLabel(const QString &fileName);
    QTableWidgetItem *createTypeLabel(const QString &fileType, bool qInit);
    FloatTableWidgetItem *getTimeLabel();

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
