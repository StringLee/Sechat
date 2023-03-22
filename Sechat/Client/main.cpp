#include "widget.h"
#include "signin.h"
#include <QApplication>
#include "link.h"
int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    Link w;
    w.show();
    return a.exec();
}
