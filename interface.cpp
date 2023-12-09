#include "interface.h"
#include "ui_interface.h"
#include <functional>
#include <QDebug>
#include <QThread>

//interface constructor
Interface::Interface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Interface)
{
    ui->setupUi(this);

    ui->chooseAge->addItem("Adult");
    ui->chooseAge->addItem("Child");
    ui->battery->setValue(100);

    aed = new AED(this); //initializes the aed object
    deliverShock = new QTimer(this); //initializes a timer for delivering a shock

    //connects the shock timer to the updateCountdown function
    connect(deliverShock, &QTimer::timeout, this, &Interface::updateCountdown);

    //connects the power button to the AED self-test to start the device
    connect(ui->powerButton, &QPushButton::clicked, this, &Interface::onPowerButtonClicked);

    //connects the chargeBattery button to the batteryCharged function
    connect(ui->chargeBattery, &QPushButton::clicked, this, &Interface::batteryCharged);

    //connects the electrode button to the electrodeRemoved function
    connect(ui->electrodeButton, &QPushButton::clicked, this, &Interface::electrodeRemoved);

    //connects the signal and slots
    connect(aed, &AED::selfTestCompleted, this, &Interface::onSelfTestCompleted);
    connect(aed, &AED::heartRhythmAnalyzed, this, &Interface::onHeartRhythmAnalyzed);
    connect(aed, &AED::graphReady, this, &Interface::onGraphReady);
    connect(aed, &AED::appendMessage, this, &Interface::appendToTextBrowser);
}

//interface destructor
Interface::~Interface()
{
    delete ui;
}

//adds the text to the textbrowser
void Interface::appendToTextBrowser(const QString& message) {
    ui->output->append(message);
}

// mark a step as complete by coloring it green
void Interface::setStepColor(QWidget *stepWidget) {
    QPalette p = stepWidget->palette();
    p.setColor(QPalette::Base, Qt::green);
    stepWidget->setPalette(p);
}

//resets the steps to white when running again
void Interface::resetStepColor(QWidget *stepWidget) {
    QPalette p = stepWidget->palette();
    p.setColor(QPalette::Base, Qt::white);
    stepWidget->setPalette(p);
}

//depletes the battery once the self-test is done
void Interface::onSelfTestCompleted(bool success) {
    updateBatteryStatus(1); //reduce battery by 1% for self-test

    if (success){
        appendToTextBrowser("Selt-test was successful. Everything is working as intended.");
        QThread::sleep(1);
    } else {
        appendToTextBrowser("Self-test has failed. Call maintenance to fix the issue.");
    }

    isAnalyzing = true; //sets the flag to tru

    if (success){
        int randomNumber = rand() % 4; //sets a random number that will choose which heart rhythm we start with

        AED::HeartRhythm rhythm;
        switch (randomNumber) {
            case 0:
                rhythm = AED::SinusRhythm;
                break;
            case 1:
                rhythm = AED::VentricularFibrillation;
                break;
            case 2:
                rhythm = AED::Asystole;
                break;
            case 3:
                rhythm = AED::VentricularTachycardia;
                break;
        }

        //message that places the pads on the victim (either child or adult)
        (ui->chooseAge->currentText() == "Child") ? appendToTextBrowser("Placing child pads on victims chest") : appendToTextBrowser("Placing adult pads on victims chest");
        setStepColor(ui->step3);

        //message that starts the analysis
        appendToTextBrowser("Do not touch victim for accurate analysis. Starting analysis...");


        //if we start with a Sinus Rhythm, that means the victim is ok
        if (rhythm == AED::SinusRhythm){
            isAnalyzing = false; //sets it to false
            appendToTextBrowser("Analysis complete. No issue with victim");
            setStepColor(ui->step4);
            updateBatteryStatus(10); //reduce baterry by 10% for analysis

          //else, the victim is not ok and we start the process
        } else {
            isAnalyzing = false; //sets the flag to false
            aed->analyzeHeartRhythm(rhythm);
        }

        //updates the graph based on the heart rhythm
        QPixmap graph = aed->getHeartRhythmGraph(rhythm, ui->finalGraph->size());
        ui->finalGraph->setPixmap(graph);
    }
}

//depletes the battery once the analysis is done
void Interface::onHeartRhythmAnalyzed(bool shockable) {
    appendToTextBrowser("Analysis complete.");
    setStepColor(ui->step4);

    if (shockable) {
        shockVal = 3;
        deliverShock->start(1000); //starts the timer with 1 second intervals
    }

    updateBatteryStatus(10); //reduce battery by 10% for analysis
}

//updates the countdown and delivers the shock
void Interface::updateCountdown() {    
    isShock = true;

    //displays a message that warns everyone to stand back before the shock is delivered
    if (shockVal >= 1) {
        QString message = QString("Stay back, a shock will be delivered in %1 seconds").arg(shockVal);
        appendToTextBrowser(message);
        shockVal--;
        isShock = true;

      //delivers the shock
    } else {
        deliverShock->stop();
        appendToTextBrowser("SHOCK DELIVERED");
        setStepColor(ui->step5);
        updateBatteryStatus(10); //reduce batter by 10% for shock
        isShock = false;

        isPerformingCPR = true;
        // Start CPR process here
        bool isChild = ui->chooseAge->currentText() == "Child";
        CPR *cpr = new CPR(isChild);
        connect(cpr, &CPR::messageToDisplay, this, &Interface::appendToTextBrowser);
        connect(cpr, &CPR::cprCompleted, this, &Interface::onCPRCompleted);
        setStepColor(ui->step6);
        cpr->startCPR();
        isPerformingCPR = true;
    }
}

//dispays the graph on the interface
void Interface::onGraphReady(const QPixmap &graph) {
    QPixmap scaledGraph = graph.scaled(ui->finalGraph->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->finalGraph->setPixmap(scaledGraph);

}

//updates the battery status when it gets depleted
void Interface::updateBatteryStatus(int depletion) {
    int currentBatteryLevel = ui->battery->value();
    ui->battery->setValue(currentBatteryLevel - depletion);
}

//recharges the battery back to 100%
void Interface::batteryCharged(){
    ui->battery->setValue(100);
}

//makes the steps background white and calls the start function
void Interface::onPowerButtonClicked(){
    resetStepColor(ui->step1);
    resetStepColor(ui->step2);
    resetStepColor(ui->step3);
    resetStepColor(ui->step4);
    resetStepColor(ui->step5);
    resetStepColor(ui->step6);
    start();

}

//handles the analysis and choosing another heart rhythm after the first revive attempt
void Interface::handleReviveAttempt(AED::HeartRhythm rhythm) {
    //checks if there isnt enough battery for the next rhythm if it isnt a Sinus Rhythm
    if (ui->battery->value() <=15 && rhythm != AED::SinusRhythm){
        qDebug("in if");
        appendToTextBrowser("There is not enough battery left for making a successful run.");

      //if its a Sinus Rhythm, that means the victim is now ok
    } else if (rhythm == AED::SinusRhythm) {
        qDebug("in else if");
        appendToTextBrowser("Starting another analysis...");
        appendToTextBrowser("Victim stabilized with Sinus Rhythm.");

      //else, we choose another heart rhythm to analyze
    } else {
        qDebug("in else");
        appendToTextBrowser("Starting another analysis...");
        onHeartRhythmAnalyzed(rhythm == AED::VentricularFibrillation || rhythm == AED::VentricularTachycardia || rhythm == AED::Asystole);
    }
}

//handles when a rescuer finishes CPR
void Interface::onCPRCompleted() {
    //chooses between the non-shockable rhythm
    AED::HeartRhythm newRhythm = (rand() % 2 == 0) ? AED::SinusRhythm : AED::Asystole;

    //updates the grpah with the new heart rhythm
    QPixmap graph = aed->getHeartRhythmGraph(newRhythm, ui->finalGraph->size());
    ui->finalGraph->setPixmap(graph);

    //if the current heart rhythm and the previous heart rhythm are both Asystole, then the victim has died
    if (previousRhythm == AED::Asystole && newRhythm == AED::Asystole) {
        appendToTextBrowser("Resuscitation failed. Victim is deceased.");
    } else {
        handleReviveAttempt(newRhythm);
    }

    //updates the previous heart rhythm with the current one
    previousRhythm = newRhythm;
}

// resets the interface and the AED to prepare for a new analysis
void Interface::resetToAnalysisStep() {
    // stop any ongoing shock delivery or CPR operation
    if (deliverShock->isActive()) {
        deliverShock->stop();
    }

    isPerformingCPR = false;

    // reset the internal state
    isAnalyzing = false;
    isShock = false;
    isPerformingCPR = false;

    resetStepColor(ui->step1);
    resetStepColor(ui->step2);
    resetStepColor(ui->step3);
    resetStepColor(ui->step4);
    resetStepColor(ui->step5);
    resetStepColor(ui->step6);

    // reset the heart rhythm graph to the initial state
    QPixmap emptyGraph;
    ui->finalGraph->setPixmap(emptyGraph);

    appendToTextBrowser("Electrodes removed");
    appendToTextBrowser("Reattaching electrodes");

    // finally, restart the AED process
    start();
}


//handles what happens when the electrode pads are removed mid process
void Interface::electrodeRemoved(){
    appendToTextBrowser("Electrode pads has been removed, the analysis can't be completed");

    isAnalyzing = false;
    isShock = false;
    isPerformingCPR = false;

    resetToAnalysisStep();
}

//starts the simulation
void Interface::start(){
    appendToTextBrowser("Checking if patient is ok...");
    // once POST is complete, check if patient is ok
    setStepColor(ui->step1);

    appendToTextBrowser("Calling 911.");
    setStepColor(ui->step2);


    //if the battery level is greater than 20%, the AED can function like usual
    if (ui->battery->value() > 20){
        aed->performSelfTest();

      //if its not then it doesnt have enough battery to complete the whole process (self-test, analysis and shock)
    } else {
        appendToTextBrowser("There is not enough battery left for making a successful run.");
    }
}
