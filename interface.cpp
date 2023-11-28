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
    ui->output->append("Test Message");

    qDebug("we in the inter constrct");
    Test *test = new Test(this, false); // isChild is a boolean indicating if it's child CPR
    connect(ui->powerButton, &QPushButton::clicked, test, &Test::onButtonClicked);

    ui->choosePads->addItem(QString("1"));
}

Interface::~Interface()
{
    qDebug("we in the inter destruct");
    ui->output->append("in destruct");
    delete ui;
}

//adds the text to the textbrowser
void Interface::appendToTextBrowser(const QString& message) {
    qDebug("we in the inter append");
    qDebug() << "Current thread:" << QThread::currentThread();
//    qDebug() << "Interface thread:" << this->thread();
//    ui->output->append(message);
    ui->choosePads->addItem(QString("2"));

}

