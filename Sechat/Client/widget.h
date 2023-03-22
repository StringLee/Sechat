#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QHostAddress>
#include <QTcpSocket>
#include <QMap>
#include <QFile>
#include <QTimer>
#include "signal.h"
#include <QComboBox>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTextCodec>
#include <QFileInfo>
#include <QFileDialog>
#include <QFile>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QListWidget *contentListWidget;
    QLineEdit *sendLineEdit;
    QPushButton *sendBtn;
    QGridLayout *mainLayout;
    QString userName;
    QPushButton *selectFile;
    QPushButton *sendFile;
    QComboBox *pickFile;
    QPushButton *getFile;
    QList<QString> fileName;
    QList<QFile*> file;
    QLabel *serverIPLabel;
    QLineEdit *serverIPLineEdit;
    QTcpSocket *tcpSocket;
    QPushButton *regret;
    QPushButton *reply;
    int times = 0;

    void addFile(QString);
    void downloadFile(QString);
    void takeBack(QString);
    void replyBack(QString);
    QString lock(QString);
    QByteArray lock(QByteArray);
    QByteArray unlock(QByteArray);
    void changeLook(QWidget * widget,int x,int y);

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);

public slots:
    void slotOfDataReceived(QString);
    void slotOfSend();
    void slotOfAceptAccount(QString,QTcpSocket*);
    void slotOfSelectFile();
    void slotOfSendFile();
    void slotOfGetFile();
    void slotOfRegret();
    void slotOfReply();
};

#endif // WIDGET_H
