#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include "server.h"
#include <QColor>
#include <QGridLayout>
#include <QMouseEvent>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QListWidget * conversation;
    QPushButton * run;
    QPushButton * takeBack;
    int port;
    Server * server;
    void changeLook(QWidget * widget,int x,int y);

protected:
    void mouseMoveEvent(QMouseEvent *e);

public slots:
    void slotOfRun();
    void slotOfUpdateServer(QString);
    void slotOfTakeBack();
};

#endif // WIDGET_H
