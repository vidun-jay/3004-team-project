#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "CPR.h"

class Test : public QObject {
    Q_OBJECT

public:
    Test(QObject *parent = nullptr, bool isChild = false, Interface* interface = nullptr);
    ~Test();

public slots:
    void onButtonClicked(); // Slot to handle button click

private:
    CPR *cpr;
    bool isChild;
};

#endif // TEST_H
