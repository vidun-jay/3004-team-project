#include "CPR.h"
#include "interface.h"
#include <QDebug>


CPR::CPR(QWidget *parent, bool isChild)
    : QWidget(parent), compressionCount(0), isChild(isChild) {

    qDebug("we in the cpr construct");

    compressionTimer = new QTimer(this);
    connect(compressionTimer, &QTimer::timeout, this, &CPR::updateCPRFeedback);
}

CPR::~CPR() {
    delete compressionTimer;
}

void CPR::startCPR() {
    qDebug("we in the cpr start");
    interF->appendToTextBrowser("inside start");
    compressionCount = 0;
    compressionTimer->start(60000 / 100); //to simulate 100 chest compressions/min
}

void CPR::updateCPRFeedback() {
    qDebug("we in the cpr update");

    ++compressionCount;

    //to differentiate if we are giving CPR to a child or an adult (logic is the same for both but can be modified if CPR for child should be different)
    if (isChild) {
        //to simulate a real rescuer trying to give CPR to a victim, the initial ones wont be deep enough but after a few chest compressions, the rescuer is able to get the right depth (value can be changed)
        if (compressionCount <= 20) {
            interF->appendToTextBrowser("Increase the compression depth for the child");
        } else {
            interF->appendToTextBrowser("Good compressions");
        }
    } else {
        if (compressionCount <= 20) {
            if (interF != nullptr){
                interF->appendToTextBrowser("PUSH HARDER");
            } else{
                qDebug("its null");
            }
        } else {
            interF->appendToTextBrowser("GOOD COMPRESSIONS");
        }
    }

    //ends the CPR after 100 chest compressions since thats the recommended amount according to AED-AdminGuide (can be changed if needed)
    if (compressionCount >= 100) {
        compressionTimer->stop();
        interF->appendToTextBrowser("STOP CPR");
    }
}
