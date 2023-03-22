#include "register.h"
#include <QGridLayout>
#include <QWidget>
#include <QMessageBox>

#include <QMap>

Register::Register(QWidget *parent) : QDialog(parent){
    tcpSocket = new QTcpSocket;

    passwordEditC1 = new QLineEdit;
    passwordEditC2 = new QLineEdit;
    accountEditC = new QLineEdit;
    accountLabelC = new QLabel("账号:");
    passwordLabelC1 = new QLabel("密码:");
    passwordLabelC2 = new QLabel("确认密码:");
    passwordEditC1->setEchoMode(QLineEdit::Password);
    passwordEditC2->setEchoMode(QLineEdit::Password);
    pick = new QPushButton("确定");

    accountEditC->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    passwordEditC1->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    passwordEditC2->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    pick->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");

    QGridLayout *mainLayoutC = new QGridLayout;
    mainLayoutC->addWidget(accountLabelC,1,0);
    mainLayoutC->addWidget(accountEditC,1,1);
    mainLayoutC->addWidget(passwordLabelC1,2,0);
    mainLayoutC->addWidget(passwordEditC1,2,1);
    mainLayoutC->addWidget(passwordLabelC2,3,0);
    mainLayoutC->addWidget(passwordEditC2,3,1);
    mainLayoutC->addWidget(pick,4,0,1,2);

    setLayout(mainLayoutC);
    setWindowTitle("注册 -Sechat");
    setWindowIcon(QIcon(":/pics/Pics/ico.ico"));
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);

    connect(pick,SIGNAL(clicked(bool)),this,SLOT(slotOfCreate()));
    connect(pick,SIGNAL(clicked(bool)),this,SLOT(accept()));

    this->setMouseTracking(true);
}

Register::~Register(){
}

void Register::slotOfCreate(){
    QString tempAccount = accountEditC->text();
    QString tempPassword1 = passwordEditC1->text();
    QString tempPassword2 = passwordEditC2->text();
    if(tempAccount == "" || tempPassword1 == "" || tempPassword2 == ""){
        QMessageBox::information(this,"错误","输入内容不能为空");
    }else if(tempPassword1 != tempPassword2){
        QMessageBox::information(this,"错误","两次密码不一致");
    }else{
        tcpSocket->write(lock("@" + tempAccount + "#" + tempPassword1).toUtf8().data());
    }
    accountEditC->setText(0);
    passwordEditC1->setText(0);
    passwordEditC2->setText(0);
}

void Register::slotOfTcp(QTcpSocket * tcp){
    tcpSocket = tcp;
}

QString Register::lock(QString begin){
    std::wstring wString = begin.toStdWString();
    static QByteArray key = "MY_KEY";
    for (int i = 0; i < wString.size(); i++){
        wString[i] = wString[i] ^ key[i % key.size()];
    }
    return QString::fromStdWString(wString).toUtf8();
}

void Register::mouseMoveEvent(QMouseEvent *e){
    int x = e->x();
    int y = e->y();
    changeLook(pick,x,y);
    changeLook(accountEditC,x,y);
    changeLook(passwordEditC1,x,y);
    changeLook(passwordEditC2,x,y);
}

void Register::changeLook(QWidget *widget,int x,int y){
    widget->setStyleSheet("border:2px groove black;border-radius:5px;padding:2px 4px; ");
    if(x > widget->geometry().left()-3 && x < widget->geometry().right()+3 && y < widget->geometry().bottom()+3 && y > widget->geometry().top()-3){
        widget->setStyleSheet("border:2px groove red;border-radius:5px;padding:2px 4px;");
    }
}
