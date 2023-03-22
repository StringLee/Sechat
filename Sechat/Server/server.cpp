#include "server.h"
Server::Server(QObject * parent,int port) : QTcpServer(parent){
    listen(QHostAddress::Any,port);
}

void Server::incomingConnection(int socketDescriptor){
    ClientSocket * clientSocket = new ClientSocket;
    connect(clientSocket,SIGNAL(signalOfUpdateClient(QString)),this,SLOT(slotOfUpdateClients(QString)));
    connect(clientSocket,SIGNAL(signalOfDisconnect(int)),this,SLOT(slotOfDisconnect(int)));
    clientSocket->setSocketDescriptor(socketDescriptor);
    clientSocketList.append(clientSocket);
}

void Server::slotOfUpdateClients(QString message){
    for(int i = 0; i < clientSocketList.count(); i ++){
        QTcpSocket * mes = clientSocketList.at(i);
        mes->write(lock(message).toUtf8().data());
    }
    emit signalOfUpdateServer(message);
}

void Server::slotOfDisconnect(int descriptor){
    for(int i = 0; i < clientSocketList.count(); i ++){
        QTcpSocket * mes = clientSocketList.at(i);
        if(mes->socketDescriptor() == descriptor){
            clientSocketList.removeAt(i);
            return;
        }
    }
}

QString Server::lock(QString begin){
    std::wstring wString = begin.toStdWString();
    static QByteArray key = "MY_KEY";
    for (int i = 0; i < wString.size(); i++){
        wString[i] = wString[i] ^ key[i % key.size()];
    }
    return QString::fromStdWString(wString).toUtf8();
}

QByteArray Server::unlock(QByteArray data){
    return lock(data.data()).toUtf8();
}

