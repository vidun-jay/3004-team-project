#ifndef INTERFACE_H
#define INTERFACE_H

#include "aed.h"
#include <QMainWindow>
#include <QLabel>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Interface; }
QT_END_NAMESPACE

class Interface : public QMainWindow
{
    Q_OBJECT

public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

    void appendToTextBrowser(const QString& message);
    void onSelfTestCompleted(bool success);
    void onHeartRhythmAnalyzed(bool shockable);
    void onGraphReady(const QPixmap &graph);
    void updateBatteryStatus(int depletion);
    void batteryCharged();
    void handleReviveAttempt(AED::HeartRhythm initialRhythm);
    void onCPRCompleted();
    void electrodeRemoved();
    void setStepColor(QWidget *stepWidget);
    void resetStepColor(QWidget *stepWidget);
    void start();

private slots:
    void onPowerButtonClicked();
    void updateCountdown();

private:
    Ui::Interface *ui;
    AED *aed;
    QTimer *deliverShock;
    int shockVal;
    AED::HeartRhythm previousRhythm;
    bool isAnalyzing = false;
    bool isShock = false;
    bool isPerformingCPR = false;
};
#endif // INTERFACE_H
