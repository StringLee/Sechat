#include "link.h"
Link::Link(QWidget *parent) : QWidget(parent){
    signIn = new SignIn;

    IPLabel = new QLabel("IP地址:");
    IPEdit = new QLineEdit;
    IPEdit->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px;");
    IPEdit->setText("127.0.0.1");
    enter = new QPushButton("连接至服务器");
    enter->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(IPLabel,0,0);
    mainLayout->addWidget(IPEdit,0,1);
    mainLayout->addWidget(enter,1,0,1,2);

    setLayout(mainLayout);
    setWindowIcon(QIcon(":/pics/Pics/ico.ico"));
    setWindowTitle("连接 -Sechat");
    setFixedSize(310,110);

    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    this->setMouseTracking(true);

    port = 8010;
    serverIP =new QHostAddress();
    tcpSocket = new QTcpSocket;

    connect(enter,SIGNAL(clicked(bool)),this,SLOT(slotOfEnter()));
    connect(this,SIGNAL(signalOfSendTcp(QTcpSocket*)),signIn,SLOT(slotOfReceiveTcp(QTcpSocket*)));
}

void Link::slotOfEnter(){
    serverIP->setAddress(IPEdit->text());
    tcpSocket->connectToHost(*serverIP,port);
    this->close();
    signIn->show();
    emit signalOfSendTcp(tcpSocket);
}

void Link::mouseMoveEvent(QMouseEvent *e){
    int x = e->x();
    int y = e->y();
    changeLook(enter,x,y);
    changeLook(IPEdit,x,y);
}

void Link::changeLook(QWidget *widget,int x,int y){
    widget->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    if(x > widget->geometry().left()-3 && x < widget->geometry().right()+3 && y < widget->geometry().bottom()+3 && y > widget->geometry().top()-3){
        widget->setStyleSheet("border:2px groove red;border-radius:5px;padding:2px 4px;");
    }
}
