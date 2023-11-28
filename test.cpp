#include "Test.h"
#include <QDebug>

//file just to test the CPR function, will get deleted once we implement the function that will actually call CPR
Test::Test(QObject *parent, bool isChild)
    : QObject(parent), isChild(isChild) {
    qDebug("we in the test construct");

    cpr = new CPR(nullptr, isChild);
}

Test::~Test() {
    qDebug("we in the test destruct");
    delete cpr; // Clean up
}

void Test::onButtonClicked() {
    qDebug("we in the test click");
    cpr->startCPR(); // Start the CPR simulation
}
