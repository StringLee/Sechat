#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent){
    conversation = new QListWidget(this);
    run = new QPushButton("启动服务器");
    takeBack = new QPushButton("撤回");
    takeBack->setEnabled(false);

    run->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    conversation->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    takeBack->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(conversation,0,0);
    mainLayout->addWidget(takeBack,1,0);
    mainLayout->addWidget(run,2,0);

    setLayout(mainLayout);
    setWindowIcon(QIcon(":/Pic/Pics/icon.ico"));
    setWindowTitle("Sechat-服务器");
    setMouseTracking(true);

    port = 8010;
    
    connect(run,SIGNAL(clicked(bool)),this,SLOT(slotOfRun()));
}

Widget::~Widget(){
}

void Widget::slotOfRun(){
    server = new Server(this,port);
    connect(takeBack,SIGNAL(clicked(bool)),this,SLOT(slotOfTakeBack()));
    connect(server,SIGNAL(signalOfUpdateServer(QString)),this,SLOT(slotOfUpdateServer(QString)));
    run->setEnabled(false);
    run->setText("已启动");
    takeBack->setEnabled(true);
}

void Widget::slotOfUpdateServer(QString message){
    if(message[0] == '%'){
        for(int i = 0; i < conversation->count(); i++){
            if(conversation->item(i)->text() == QString::fromStdString(message.toStdString().substr(1))){
                conversation->item(i)->setText(conversation->item(i)->text()+"（已撤销）");
            }
        }
    }else conversation->addItem(message);
}

void Widget::slotOfTakeBack(){
    server->slotOfUpdateClients("%" + conversation->selectedItems().at(0)->text());
}

void Widget::mouseMoveEvent(QMouseEvent *e){
    int x = e->x();
    int y = e->y();
    changeLook(conversation,x,y);
    changeLook(takeBack,x,y);
    changeLook(run,x,y);
}

void Widget::changeLook(QWidget *widget,int x,int y){
    widget->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    if(x > widget->geometry().left()-3 && x < widget->geometry().right()+3 && y < widget->geometry().bottom()+3 && y > widget->geometry().top()-3){
        widget->setStyleSheet("border:2px groove red;border-radius:5px;padding:2px 4px;");
    }
}
