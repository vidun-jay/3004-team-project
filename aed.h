#ifndef AED_H
#define AED_H

#include "CPR.h"
#include <QObject>
#include <QString>
#include <QPixmap>


class AED : public QObject {
    Q_OBJECT

public:
    explicit AED(QObject *parent = nullptr, bool isChild = false);

    enum HeartRhythm {
        SinusRhythm,
        Asystole,
        VentricularFibrillation,
        VentricularTachycardia
    };

    void decreaseBattery(int consumption);
    void performSelfTest();
    bool analyzeHeartRhythm(HeartRhythm rhythm);
    QPixmap getHeartRhythmGraph(HeartRhythm rhythm, const QSize& size);

signals:
    void selfTestCompleted(bool success);
    void heartRhythmAnalyzed(bool shockable);
    void graphReady(QPixmap graph);
    void appendMessage(const QString& message);

private:
    QPixmap generateGraphData(HeartRhythm rhythm, const QSize& size);
    CPR *cpr;
    bool isChild;
};

#endif // AED_H
