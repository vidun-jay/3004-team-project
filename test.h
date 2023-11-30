#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "CPR.h"

class Test : public QObject {
    Q_OBJECT

public:
    Test(QObject *parent = nullptr, bool isChild = false);
//    ~Test();

    void onButtonClicked();

    //getters
    CPR *getCPRObject() const;

private:
    CPR *cpr;
    bool isChild;
};

#endif // TEST_H
