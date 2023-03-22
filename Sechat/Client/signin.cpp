#include "signin.h"
#include "register.h"
SignIn::SignIn(QWidget *parent): QDialog(parent){
    tcpSocket = new QTcpSocket;

    mainWidget = new Widget;
    dialog = new Register(this);

    QLabel * label = new QLabel;
    label->setPixmap(QPixmap(":/pics/Pics/titile.png"));
    passwordEdit = new QLineEdit;
    accountEdit = new QLineEdit;
    accountLabel = new QLabel("账号:");
    passwordLabel = new QLabel("密码:");
    passwordEdit->setEchoMode(QLineEdit::Password);
    signIn = new QPushButton("登录");
    create = new QPushButton("注册");

    passwordEdit->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    accountEdit->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    signIn->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    create->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(label,0,0,1,2);
    mainLayout->addWidget(accountLabel,1,0);
    mainLayout->addWidget(accountEdit,1,1);
    mainLayout->addWidget(passwordLabel,2,0);
    mainLayout->addWidget(passwordEdit,2,1);
    mainLayout->addWidget(create,3,0,1,2);
    mainLayout->addWidget(signIn,4,0,1,2);

    setLayout(mainLayout);
    setWindowTitle("登录 -Sechat");
    setWindowIcon(QIcon(":/pics/Pics/ico.ico"));
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    setFixedSize(226,260);
    this->setMouseTracking(true);

    connect(create,SIGNAL(clicked(bool)),this,SLOT(slotOfCreate()));
    connect(signIn,SIGNAL(clicked(bool)),this,SLOT(slotOfSignIn()));
    connect(this,SIGNAL(signalOfMessage(QString)),mainWidget,SLOT(slotOfDataReceived(QString)));
    connect(this,SIGNAL(signalOfSendAccount(QString,QTcpSocket*)),mainWidget,SLOT(slotOfAceptAccount(QString,QTcpSocket*)));
    connect(this,SIGNAL(signalOfSendTcp(QTcpSocket*)),dialog,SLOT(slotOfTcp(QTcpSocket*)));
}

SignIn::~SignIn(){

}

void SignIn::slotOfCreate(){
    emit signalOfSendTcp(tcpSocket);
    dialog->show();
}

void SignIn::slotOfDepart(){
    QString temp = unlock(tcpSocket->readAll()).data();
    if(temp == "?"){
        QMessageBox::information(this,"失败","用户名或密码错误");
    }else if(temp == "!"){
        QMessageBox::information(this,"成功","登录成功");
        emit signalOfSendAccount(accountEdit->text(),tcpSocket);
        mainWidget->show();
        this->close();
    }else if(temp == "+"){
        QMessageBox::information(this,"成功","注册成功");
    }else if(temp == "-"){
        QMessageBox::information(this,"失败","注册失败,用户名重复");
    }else{
        emit signalOfMessage(temp);
    }
}

void SignIn::slotOfSignIn(){
    tcpSocket->write(lock("^"+ accountEdit->text() + "#" + passwordEdit->text()).toUtf8().data());
}

void SignIn::slotOfReceiveTcp(QTcpSocket* tcp){
    tcpSocket = tcp;
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(slotOfDepart()));
}

QString SignIn::lock(QString begin){
    std::wstring wString = begin.toStdWString();
    static QByteArray key = "MY_KEY";
    for (int i = 0; i < wString.size(); i++){
        wString[i] = wString[i] ^ key[i % key.size()];
    }
    return QString::fromStdWString(wString).toUtf8();
}

QByteArray SignIn::unlock(QByteArray data){
    return lock(data.data()).toUtf8();
}

void SignIn::mouseMoveEvent(QMouseEvent *e){
    int x = e->x();
    int y = e->y();
    changeLook(signIn,x,y);
    changeLook(create,x,y);
    changeLook(passwordEdit,x,y);
    changeLook(accountEdit,x,y);
}

void SignIn::changeLook(QWidget *widget,int x,int y){
    widget->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    if(x > widget->geometry().left()-3 && x < widget->geometry().right()+3 && y < widget->geometry().bottom()+3 && y > widget->geometry().top()-3){
        widget->setStyleSheet("border:2px groove red;border-radius:5px;padding:2px 4px;");
    }
}
