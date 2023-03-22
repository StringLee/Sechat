#ifndef SIGNIN_H
#define SIGNIN_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include "register.h"
#include <QMap>
#include <QString>
#include "widget.h"
#include <QMouseEvent>
#include <QTcpSocket>

class SignIn : public QDialog
{
    Q_OBJECT

public:
    SignIn(QWidget *parent = 0);
    ~SignIn();

    QLineEdit *accountEdit;
    QLineEdit *passwordEdit;
    QLabel *accountLabel;
    QLabel *passwordLabel;
    QPushButton *signIn;
    QPushButton *create;
    Register *dialog;
    Widget * mainWidget;
    QTcpSocket *tcpSocket;
    QString lock(QString);
    QByteArray unlock(QByteArray);

    void changeLook(QWidget*,int,int);

protected:
    void mouseMoveEvent(QMouseEvent *e);

public slots:
    void slotOfCreate();
    void slotOfSignIn();
    void slotOfReceiveTcp(QTcpSocket*);
    void slotOfDepart();

signals:
    void signalOfSendAccount(QString,QTcpSocket*);
    void signalOfMessage(QString);
    void signalOfSendTcp(QTcpSocket*);
};

#endif // SIGNIN_H
