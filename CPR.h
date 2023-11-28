#ifndef CPR_H
#define CPR_H

#include <QWidget>
#include <QTimer>
#include <QLabel>

class Interface;

class CPR : public QWidget {
    Q_OBJECT

public:
    CPR(QWidget *parent = nullptr, bool isChild = false);
    ~CPR();

     void setInterface(Interface* value) {interF = value;}
     void startCPR();

private slots:
    void updateCPRFeedback();

private:
    Interface* interF;
    QTimer *compressionTimer;
    QLabel *feedbackLabel;
    int compressionCount;
    bool isChild;
};

#endif // CPR_H
