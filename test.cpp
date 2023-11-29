#include "Test.h"
#include <QDebug>

Test::Test(QObject *parent, bool isChild)
    : QObject(parent), isChild(isChild) {

    cpr = new CPR(this, isChild);

}

//Test::~Test() {
//    qDebug("we in the test destruct");
////    delete cpr;
//}

void Test::onButtonClicked() {
    cpr->startCPR(); //starts the CPR simulation
}

CPR *Test::getCPRObject() const {
    return cpr;
}
