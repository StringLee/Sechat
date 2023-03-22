#ifndef LINK_H
#define LINK_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMouseEvent>
#include <QGridLayout>
#include "signin.h"

class Link : public QWidget{
    Q_OBJECT

public:
    Link(QWidget *parent = 0);

    QLabel *IPLabel;
    QLineEdit *IPEdit;
    QPushButton *enter;
    QTcpSocket *tcpSocket;
    QHostAddress *serverIP;
    int port;
    SignIn *signIn;

    void changeLook(QWidget*,int,int);

protected:
    void mouseMoveEvent(QMouseEvent *e);

public slots:
    void slotOfEnter();

signals:
    void signalOfSendTcp(QTcpSocket*);
};

#endif // LINK_H
