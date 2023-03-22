#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <string>
#include <QFileInfo>
#include <QListWidget>
class ClientSocket :public QTcpSocket{
    Q_OBJECT

public:
    ClientSocket();
    QList<QFile*> file;
    QList<QString> fileName;
    QSqlDatabase db;
    QSqlQuery *query;

    void receiveFile(QString);
    void receiveData(QString);
    void receiveRegister(QString);
    void receiveSignIn(QString);
    void sendFile(QString);
    QString lock(QString);
    QByteArray lock(QByteArray);
    QByteArray unlock(QByteArray);

signals:
    void signalOfUpdateClient(QString);
    void signalOfDisconnect(int);

public slots:
    void slotOfDisconnect();
    void slotOfDepart();

};

#endif // CLIENTSOCKET_H
