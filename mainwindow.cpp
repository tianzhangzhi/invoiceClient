#include <QDebug>
#include <QImage>
#include <QGraphicsScale>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "windows.h"
#include "winuser.h"

extern QAction *action_quit;
QSystemTrayIcon *trayicon;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QImage *img = new QImage();
    img->load("bg.jpg");
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(*img));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
    setStatusBar(0);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(600, 600);
    setWindowIcon(QIcon("xt.png"));
    setWindowTitle("兴通开票");

    trayicon = new QSystemTrayIcon(this);
    connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));
    trayicon->setIcon(QIcon("xt.png"));

    QMenu *tray_menu = new QMenu();
    tray_menu->addAction(action_quit);
    trayicon->setContextMenu(tray_menu);
    trayicon->show();
}

void MainWindow::changeEvent(QEvent *e)
{
    qDebug() << "changevent" << e->type();
    if(e->type()!=QEvent::WindowStateChange) return;
    if(this->windowState()==Qt::WindowMinimized)
    {
        this->hide();
        trayicon->show();
    }
}

void MainWindow::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    //单击
    case QSystemTrayIcon::Trigger:
        //双击
    case QSystemTrayIcon::DoubleClick:
        if(this->isHidden())
        {
            //恢复窗口显示
            this->show();
            //一下两句缺一均不能有效将窗口置顶
            this->setWindowState(Qt::WindowActive);
            this->activateWindow();
        }
        else
        {
            this->hide();
        }
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


