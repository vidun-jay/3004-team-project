#include "interface.h"

#include <QApplication>

int main(int argc, char *argv[]){
    srand(time(NULL)); //to make the randome odds more random
    QApplication a(argc, argv);
    Interface w;
    w.show();
    return a.exec();
}
