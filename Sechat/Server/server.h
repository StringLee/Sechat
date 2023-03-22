#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include "clientsocket.h"
#include <QListWidget>
class Server : public QTcpServer{
    Q_OBJECT

public:
    Server(QObject * parent = 0, int port = 0);
    QList<ClientSocket*> clientSocketList;

    void incomingConnection(int socketDescriptor);
    QString lock(QString);
    QByteArray unlock(QByteArray);

signals:
    void signalOfUpdateServer(QString);

public slots:
    void slotOfUpdateClients(QString);
    void slotOfDisconnect(int);
};

#endif // SERVER_H
