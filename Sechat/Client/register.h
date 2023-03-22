#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpSocket>
#include <QGridLayout>
#include <QWidget>
#include <QMessageBox>
#include <QMouseEvent>

class Register : public QDialog{
    Q_OBJECT

public:
    Register(QWidget *parent = 0);
    ~Register();

    QLineEdit *accountEditC;
    QLineEdit *passwordEditC1;
    QLineEdit *passwordEditC2;
    QLabel *accountLabelC;
    QLabel *passwordLabelC1;
    QLabel *passwordLabelC2;
    QPushButton *pick;
    QTcpSocket *tcpSocket;
    QString lock(QString);

    void changeLook(QWidget*,int,int);

protected:
    void mouseMoveEvent(QMouseEvent *e);

public slots:
    void slotOfCreate();
    void slotOfTcp(QTcpSocket*);
};

#endif // REGISTER_H
