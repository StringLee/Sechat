#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent){

    tcpSocket = new QTcpSocket;

    contentListWidget = new QListWidget;
    sendLineEdit = new QLineEdit;
    sendBtn = new QPushButton("发送");
    selectFile = new QPushButton("选择文件");
    sendFile = new QPushButton("发送文件");
    sendFile->setEnabled(false);
    pickFile = new QComboBox;
    pickFile->addItem("暂无文件","暂无文件");
    getFile = new QPushButton("下载文件");
    getFile->setEnabled(false);
    pickFile->setView(new QListView());
    regret = new QPushButton("撤回");
    reply = new QPushButton("回复");

    contentListWidget->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    sendLineEdit->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    sendBtn->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    selectFile->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    sendFile->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    pickFile->setStyleSheet("QComboBox{border:2px groove black;border-radius:5px;padding:2px 4px;}QComboBox::down-arrow{image:url(:/pics/Pics/arrow.png);}QComboBox::drop-down{background-color:white}");
    getFile->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    regret->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    reply->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(contentListWidget,0,0,1,2);
    mainLayout->addWidget(sendLineEdit,1,0);
    mainLayout->addWidget(sendBtn,1,1);
    mainLayout->addWidget(regret,2,0);
    mainLayout->addWidget(reply,2,1);
    mainLayout->addWidget(selectFile,3,0);
    mainLayout->addWidget(sendFile,3,1);
    mainLayout->addWidget(pickFile,4,0);
    mainLayout->addWidget(getFile,4,1);


    setLayout(mainLayout);
    setPalette(QPalette(Qt::white));
    setWindowIcon(QIcon(":/pics/Pics/ico.ico"));
    this->setMouseTracking(true);

    connect(sendBtn,SIGNAL(clicked()),this,SLOT(slotOfSend()));
    connect(selectFile,SIGNAL(clicked(bool)),this,SLOT(slotOfSelectFile()));
    connect(sendFile,SIGNAL(clicked(bool)),this,SLOT(slotOfSendFile()));
    connect(getFile,SIGNAL(clicked(bool)),this,SLOT(slotOfGetFile()));
    connect(regret,SIGNAL(clicked(bool)),this,SLOT(slotOfRegret()));
    connect(reply,SIGNAL(clicked(bool)),this,SLOT(slotOfReply()));
}

Widget::~Widget(){
}

void Widget::slotOfDataReceived(QString message){
    if(message[0] == '@'){
        addFile(QString::fromStdString(message.toStdString().substr(1)));
        contentListWidget->addItem("你收到了 "+QString::fromStdString(message.toStdString().substr(1)));
    }else if(message[0] == '*') downloadFile(QString::fromStdString(message.toStdString().substr(1)));
    else if(message[0] == '%') takeBack(QString::fromStdString(message.toStdString().substr(1)));
    else contentListWidget->addItem(message);
}

void Widget::slotOfAceptAccount(QString name,QTcpSocket *tcp){
    userName = name;
    setWindowTitle(userName+"的聊天窗口 -Sechat");
    tcpSocket = tcp;
    QString msg= "$\"" + userName + "\"" + "加入了聊天室";
    tcpSocket->write(lock(msg).toUtf8().data());
}

void Widget::slotOfSend(){
    if(sendLineEdit->text()==""){
        return ;
    }
    QString msg= "$" + userName + " : "+sendLineEdit->text();
    tcpSocket->write(lock(msg).toUtf8().data());
    sendLineEdit->clear();
}

void Widget::slotOfSelectFile(){
    QString filePath = QFileDialog::getOpenFileName(this, "Open", "../");
    QFileInfo info(filePath);
    fileName.append(info.fileName());
    QFile *tempFile = new QFile;
    tempFile->setFileName(filePath);
    file.append(tempFile);
    QString temp = QString("你选择了 %1").arg(info.fileName());
    contentListWidget->addItem(temp);
    sendFile->setEnabled(true);
}

void Widget::slotOfSendFile(){
     if(file.size() > 0){
       QByteArray pre;
       pre.append('!');
       QByteArray name = fileName.at(fileName.size()-1).toUtf8();
       pre.append(name);
       pre.append('#');
       file.at(file.size()-1)->open(QIODevice::ReadOnly);
       QByteArray body = file.at(file.size()-1)->readAll();
       file.at(file.size()-1)->close();
       pre.append(body);
       tcpSocket->write(lock(pre));
    }
     sendFile->setEnabled(false);
}

void Widget::addFile(QString message){
    if(pickFile->itemText(0) == "暂无文件"){
        pickFile->setItemText(0,message);
        pickFile->setItemData(0,message);
    }else{
      bool repeat = false;
      for(int i = 0; i < times;i ++){
          if(pickFile->itemText(i) == message) repeat = true;
      }
       if(!repeat){
           pickFile->addItem(message,message);
           times ++;
       }
    }
    getFile->setEnabled(true);
}

void Widget::slotOfGetFile(){
    tcpSocket->write(lock("&"+pickFile->currentText()).toUtf8().data());
}

void Widget::downloadFile(QString data){
    QList<QString> temp = data.split("#");
    QFile *tempFile = new QFile;
    tempFile->setFileName(temp.at(0));
    file.append(tempFile);
    tempFile->open(QIODevice::WriteOnly);
    tempFile->write(temp.at(1).toUtf8());
    tempFile->close();
    contentListWidget->addItem("下载 "+temp.at(0) + " 成功");
}

void Widget::takeBack(QString message){
    for(int i = 0; i < contentListWidget->count(); i++){
        if(contentListWidget->item(i)->text() == message){
            contentListWidget->item(i)->setText("已被撤回");
        }
    }
}

void Widget::slotOfRegret(){
    bool isMine = true;
    for(int i = 0; i < userName.size();i ++){
       if(contentListWidget->selectedItems().at(0)->text().at(i) != userName[i]) isMine = false;
    }
    if(!isMine) QMessageBox::information(this,"失败","您不能撤回别人的发言");
    else if(contentListWidget->selectedItems().size() > 0){
        tcpSocket->write(lock(QString("%"+contentListWidget->selectedItems().at(0)->text()).toUtf8()));
    }
}

void Widget::slotOfReply(){
    if(sendLineEdit->text()=="" ||contentListWidget->selectedItems().size() == 0){
        return ;
    }
    tcpSocket->write(lock(QString("$"+userName+" 回复：\""+contentListWidget->selectedItems().at(0)->text()+"\" :\n    "+sendLineEdit->text()).toUtf8()));
    sendLineEdit->clear();
}

QString Widget::lock(QString begin){
    std::wstring wString = begin.toStdWString();
    static QByteArray key = "MY_KEY";
    for (int i = 0; i < wString.size(); i++){
        wString[i] = wString[i] ^ key[i % key.size()];
    }
    return QString::fromStdWString(wString).toUtf8();
}

QByteArray Widget::lock(QByteArray data){
    QString begin = data.data();
    std::wstring wString = begin.toStdWString();
    static QByteArray key = "MY_KEY";
    for (int i = 0; i < wString.size(); i++){
        wString[i] = wString[i] ^ key[i % key.size()];
    }
    return QString::fromStdWString(wString).toUtf8();
}


QByteArray Widget::unlock(QByteArray data){
    QString begin = data.data();
    std::wstring wString = begin.toStdWString();
    static QByteArray key = "MY_KEY";
    for (int i = 0; i < wString.size(); i++){
        wString[i] = wString[i] ^ key[i % key.size()];
    }
    return QString::fromStdWString(wString).toUtf8();
}

void Widget::mouseMoveEvent(QMouseEvent *e){
    int x = e->x();
    int y = e->y();
    changeLook(sendBtn,x,y);
    changeLook(contentListWidget,x,y);
    changeLook(sendLineEdit,x,y);
    changeLook(selectFile,x,y);
    changeLook(sendFile,x,y);
    changeLook(getFile,x,y);
    changeLook(regret,x,y);
    changeLook(reply,x,y);
    pickFile->setStyleSheet("QComboBox{border:2px groove black;border-radius:5px;padding:2px 4px;}QComboBox::down-arrow{image:url(:/pics/Pics/arrow.png);}QComboBox::drop-down{background-color:white}");
    if(x > pickFile->geometry().left()-3 && x < pickFile->geometry().right()+3 && y < pickFile->geometry().bottom()+3 && y > pickFile->geometry().top()-3){
        pickFile->setStyleSheet("QComboBox{border:2px groove red;border-radius:5px;padding:2px 4px;}QComboBox::down-arrow{image:url(:/pics/Pics/arrowRed.png);}QComboBox::drop-down{background-color:white} ");
    }
}

void Widget::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Return) slotOfSend();
    QWidget::keyPressEvent(e);
}

void Widget::changeLook(QWidget *widget,int x,int y){
    widget->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    if(x > widget->geometry().left()-3 && x < widget->geometry().right()+3 && y < widget->geometry().bottom()+3 && y > widget->geometry().top()-3){
        widget->setStyleSheet("border:2px groove red;border-radius:5px;padding:2px 4px;");
    }
}
