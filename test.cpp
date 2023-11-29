#include "Test.h"
#include <QDebug>

//file just to test the CPR function, will get deleted once we implement the function that will actually call CPR
Test::Test(QObject *parent, bool isChild)
    : QObject(parent), isChild(isChild) {

    cpr = new CPR(this, isChild);

}

void Test::onButtonClicked() {
    cpr->startCPR(); //starts the CPR simulation
}

CPR *Test::getCPRObject() const {
    return cpr;
}
