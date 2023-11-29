#include "interface.h"
#include "ui_interface.h"
#include "test.h"
#include <functional>
#include <QDebug>
#include <QThread>

Interface::Interface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Interface)
{
    ui->setupUi(this);

    Test *test = new Test(this, false); //isChild is a boolean indicating if it's child CPR

    //connects the power button to the CPR start function to start the CPR process (THIS SHOULD CHANGE SO THAT SELF-TEST STARTS FIRST AFTER TURNING ON)
    connect(ui->powerButton, &QPushButton::clicked, test, &Test::onButtonClicked);

    //connect the signal from CPR to the append function in Interface
    connect(test->getCPRObject(), &CPR::messageToDisplay, this, &Interface::appendToTextBrowser);

}

Interface::~Interface()
{
    delete ui;
}

//adds the text to the textbrowser
void Interface::appendToTextBrowser(const QString& message) {
    ui->output->append(message);
}

