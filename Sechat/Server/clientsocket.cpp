#include "clientsocket.h"

ClientSocket::ClientSocket(){
    connect(this,SIGNAL(readyRead()),this,SLOT(slotOfDepart()));
    connect(this,SIGNAL(signalOfDisconnect(int)),this,SLOT(slotOfDisconnect()));

    if(QSqlDatabase::contains("qt_sql_default_connection")) db = QSqlDatabase::database("qt_sql_default_connection");
    else  db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("account.db");
    db.open();
    query = new QSqlQuery(db);
    query->exec("create table automobil(account varchar,password varchar)");
}

void ClientSocket::receiveData(QString msg){
    emit signalOfUpdateClient(msg);
}

void ClientSocket::slotOfDisconnect(){
    emit signalOfDisconnect(this->socketDescriptor());
}

void ClientSocket::slotOfDepart(){
    QByteArray message = unlock(readAll());
    QString temp = message.data();
    if(temp[0] == '!') receiveFile(QString::fromStdString(temp.toStdString().substr(1)));
    else if(temp[0] == '@') receiveRegister(QString::fromStdString(temp.toStdString().substr(1)));
    else if(temp[0] == '^') receiveSignIn(QString::fromStdString(temp.toStdString().substr(1)));
    else if(temp[0] == '&') sendFile(QString::fromStdString(temp.toStdString().substr(1)));
    else if(temp[0] == '%') emit signalOfUpdateClient(temp);
    else receiveData(QString::fromStdString(temp.toStdString().substr(1)));
}

void ClientSocket::receiveFile(QString message){
    QList<QString> temp = message.split("#");
    QFile *tempFile = new QFile;
    tempFile->setFileName(temp.at(0));
    file.append(tempFile);
    tempFile->open(QIODevice::WriteOnly);
    tempFile->write(temp.at(1).toUtf8());
    tempFile->close();
    emit signalOfUpdateClient("@"+temp.at(0));
}

void ClientSocket::receiveRegister(QString message){
    QList<QString> temp = message.split("#");
    query->exec(QString("SELECT account,password FROM automobil WHERE account == \"%1\"").arg(temp.at(0)));
    QString account;
    if(query->first()) account = query->value(0).toString();
    if(account == ""){
       query->exec(QString("insert into automobil values(\"%1\",\"%2\")").arg(temp.at(0)).arg(lock(temp.at(1))));
       write(lock(QString("+")).toUtf8());
    }else{
       write(lock(QString("-")).toUtf8());
    }
}

void ClientSocket::receiveSignIn(QString message){
    QList<QString> temp = message.split("#");
    query->exec(QString("SELECT account,password FROM automobil WHERE account == \"%1\" and password == \"%2\"").arg(temp.at(0)).arg(unlock(temp.at(1).toUtf8()).data()));
    QString password;
    if(query->first()) password = query->value(0).toString();
    if(password != ""){
        write(lock(QString("!")).toUtf8());
    }else{
        write(lock(QString("?")).toUtf8());
    }
}

void ClientSocket::sendFile(QString name){
    QString filePath = name;
    QFileInfo info(filePath);
    fileName.append(info.fileName());
    QFile *tempFile = new QFile;
    tempFile->setFileName(filePath);
    file.append(tempFile);
    QByteArray pre;
    pre.append('*');
    QByteArray title = fileName.at(fileName.size()-1).toUtf8();
    pre.append(title);
    pre.append('#');
    file.at(file.size()-1)->open(QIODevice::ReadOnly);
    QByteArray body = file.at(file.size()-1)->readAll();
    file.at(file.size()-1)->close();
    pre.append(body);
    write(lock(pre));
}

QString ClientSocket::lock(QString begin){
    std::wstring wString = begin.toStdWString();
    static QByteArray key = "MY_KEY";
    for (int i = 0; i < wString.size(); i++){
        wString[i] = wString[i] ^ key[i % key.size()];
    }
    return QString::fromStdWString(wString).toUtf8();
}

QByteArray ClientSocket::lock(QByteArray data){
    QString begin = data.data();
    std::wstring wString = begin.toStdWString();
    static QByteArray key = "MY_KEY";
    for (int i = 0; i < wString.size(); i++){
        wString[i] = wString[i] ^ key[i % key.size()];
    }
    return QString::fromStdWString(wString).toUtf8();
}

QByteArray ClientSocket::unlock(QByteArray data){
    QString begin = data.data();
    std::wstring wString = begin.toStdWString();
    static QByteArray key = "MY_KEY";
    for (int i = 0; i < wString.size(); i++){
        wString[i] = wString[i] ^ key[i % key.size()];
    }
    return QString::fromStdWString(wString).toUtf8();
}
