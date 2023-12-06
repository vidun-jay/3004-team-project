#include "CPR.h"
#include "interface.h"
#include <QDebug>

//CPR Constructor
CPR::CPR(bool isChild)
    : compressionCount(0), isChild(isChild) {

    compressionTimer = new QTimer(this); //creates a timer for the chest compressions

    //cnnects the timer timeout to the updateCRPFeedback function
    connect(compressionTimer, &QTimer::timeout, this, &CPR::updateCPRFeedback);
}

//CPR Destructor
CPR::~CPR() {
    delete compressionTimer;
}

//starts the CPR process
void CPR::startCPR() {
    compressionCount = 0;
    compressionTimer->start(600); //to simulate 100 chest compressions/min
}

//main CPR function
void CPR::updateCPRFeedback() {
    //to differentiate if we are giving CPR to a child or an adult (logic is the same for both but can be modified if CPR for child should be different)
    if (isChild) {
        //to simulate a real rescuer trying to give CPR to a victim, the initial ones wont be deep enough but after a few chest compressions, the rescuer is able to get the right depth (value can be changed)
        if (compressionCount == 0){
            emit messageToDisplay("STARTING CPR");
        } else if (compressionCount > 0 && compressionCount <= 10) {
            emit messageToDisplay("Increase the compression depth for the child");
        } else {
            emit messageToDisplay("Good compressions");
        }
    } else {
        if (compressionCount == 0){
            emit messageToDisplay("STARTING CPR");
        } else if (compressionCount > 0 && compressionCount <= 10) {
            emit messageToDisplay("PUSH HARDER");
        } else {
            emit messageToDisplay("GOOD COMPRESSIONS");
        }
    }

    ++compressionCount;

    //ends the CPR after 20 chest compressions
    if (compressionCount > 20) {
        compressionTimer->stop();
        emit messageToDisplay("STOP CPR (CPR lasts full 2 minutes but cut short for the simulation)");
        emit cprCompleted();
    }
}
