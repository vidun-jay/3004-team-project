#ifndef CPR_H
#define CPR_H

#include <QWidget>
#include <QTimer>
#include <QLabel>

class Interface;

class CPR : public QWidget {
    Q_OBJECT

public:
    CPR(bool isChild = false);
    ~CPR();

    void startCPR();

    //setters
     void setInterface(Interface* value) {interF = value;}

private slots:
    void updateCPRFeedback();

signals:
    void messageToDisplay(const QString& message);
    void cprCompleted();

private:
    Interface* interF;
    QTimer *compressionTimer;
    QLabel *feedbackLabel;
    int compressionCount;
    bool isChild;
};

#endif // CPR_H
