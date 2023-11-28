#include "Test.h"
#include <QDebug>

Test::Test(QObject *parent, bool isChild)
    : QObject(parent), isChild(isChild) {
    qDebug("we in the test construct");

    cpr = new CPR(nullptr, isChild);

    // Connect the CPR simulation output to a slot in the Interface class
    // This requires that you have a mechanism to send the text to the Interface class.
    // For example, you might emit a signal that Interface listens to.
}

Test::~Test() {
    qDebug("we in the test destruct");
    delete cpr; // Clean up
}

void Test::onButtonClicked() {
    qDebug("we in the test click");
    cpr->startCPR(); // Start the CPR simulation
}
