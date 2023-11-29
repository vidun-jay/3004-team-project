#include "CPR.h"
#include "interface.h"
#include <QDebug>


CPR::CPR(QObject *parent, bool isChild)
    : compressionCount(0), isChild(isChild) {

    compressionTimer = new QTimer(this);
    connect(compressionTimer, &QTimer::timeout, this, &CPR::updateCPRFeedback);
}

CPR::~CPR() {
    delete compressionTimer;
}

void CPR::startCPR() {
    compressionCount = 0;
    compressionTimer->start(600); //to simulate 100 chest compressions/min
}

void CPR::updateCPRFeedback() {
    //to differentiate if we are giving CPR to a child or an adult (logic is the same for both but can be modified if CPR for child should be different)
    if (isChild) {
        qDebug() << "count is: " << compressionCount;
        //to simulate a real rescuer trying to give CPR to a victim, the initial ones wont be deep enough but after a few chest compressions, the rescuer is able to get the right depth (value can be changed)
        if (compressionCount == 0){
            emit messageToDisplay("STARTING CPR");
        } else if (compressionCount > 0 && compressionCount <= 20) {
            emit messageToDisplay("Increase the compression depth for the child");
        } else {
            emit messageToDisplay("Good compressions");
        }
    } else {
        if (compressionCount == 0){
            emit messageToDisplay("STARTING CPR");
        } else if (compressionCount > 0 && compressionCount <= 20) {
            emit messageToDisplay("PUSH HARDER");
        } else {
            emit messageToDisplay("GOOD COMPRESSIONS");
        }
    }

    ++compressionCount;

    //ends the CPR after 100 chest compressions since thats the recommended amount according to AED-AdminGuide (can be changed if needed)
    if (compressionCount > 100) {
        compressionTimer->stop();
        emit messageToDisplay("STOP CPR");
    }
}
