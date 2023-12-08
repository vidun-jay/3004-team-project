#include "aed.h"
#include <cmath>
#include <QPainter>

//AED constructor
AED::AED(QObject *parent, bool isChild) : QObject(parent), isChild(isChild) {
    //initializes a CPR object using isChild (to indicate if its a child or an adult)
    cpr = new CPR(isChild);
}

//simulate self-tests
void AED::performSelfTest() {
    bool testSuccess = rand() % 4 != 0; //gives a 1/4 chance that the self-test will fail or not
    emit selfTestCompleted(testSuccess); //emits the result to interface
 }

//analyzes and checks if the rhythm is shockable or not
bool AED::analyzeHeartRhythm(AED::HeartRhythm rhythm) {
    bool shockable = (rhythm == AED::VentricularFibrillation || rhythm == AED::VentricularTachycardia); //if the heart rhythm is either Vfib or Vtach then its a shockable one
    emit heartRhythmAnalyzed(shockable); //emits the result to interface

    return shockable;
}

//generates a QPixmap that represents the heart rhythm graph
QPixmap AED::getHeartRhythmGraph(AED::HeartRhythm rhythm, const QSize& size) {
    QPixmap graph = generateGraphData(rhythm, size); //calls the function that makes the graph
    emit graphReady(graph); //emits the graph to interface

    return graph;
}

//generates the actual graph
QPixmap AED::generateGraphData(AED::HeartRhythm rhythm, const QSize& size) {
    QPixmap graph(size); //sets the desired size of the graph
    graph.fill(Qt::white); //sets background colour of the graph
    QPainter painter(&graph); //creates a QPainter object to make the lines on the graph
    painter.setPen(Qt::black); //sets the coulour of the line

    //plots the graph based on the rhythm we choose
    switch (rhythm) {
        //plots the graph for Synus Rhythm (or pulseless electrical activity)
        case SinusRhythm:{
            int width = 500; //sets the width of the area thats being drawn
            int height = 200; //sets the height of the area thats being drawn
            int p_wave = width / 15; //sets the width of the P wave
            int qrs_complex = width / 60; //sets the width of the QRS complex
            int t_wave = width / 30; //sets the width of the T wave
            int segment_length = p_wave + qrs_complex + t_wave; //calculates the total length of one wave cycle
            int amplitude = 30; //sets the amplitude of the area

            int previousY = height / 2; //initializes the previous Y value

            //loops through all x values to plot the waveform
            for (int x = 0; x < width; ++x) {
                int cycle = x % segment_length; //calculates what part of the cycle we are on
                int y = height / 2; //initilizes the y values of the point

                //checks if we are before the P wave
                if (cycle < p_wave) {
                    //calculates the y value using a sine function
                    y += (sin(cycle * M_PI / p_wave) * amplitude / 3);

                  //checks if we are in the QRS complex
                } else if (cycle < (p_wave + qrs_complex)) {
                    if (cycle < p_wave + qrs_complex / 3) {
                        //creates the Q component
                        y -= amplitude;
                    } else if (cycle < p_wave + 2 * qrs_complex / 5) {
                        //creates the R component
                        y += amplitude / 2;
                    } else {
                        //creates the S component
                        y -= amplitude / 5;
                    }

                  //checks if we are before the T wave
                } else {
                    //calculates the y value using a sine function
                    y += (sin((cycle - p_wave - qrs_complex) * M_PI / t_wave) * amplitude / 3);
                }

                //creates a line between the previous y val to the new y val
                painter.drawLine(x - 1, previousY, x, y);
                previousY = y; //updates the previous Y val
            }
            break;
    }

        //plots the graph for Asystole (or flatline)
        case Asystole:
            //draws a flatline
            painter.drawLine(0, graph.height() / 2, graph.width(), graph.height() / 2);
            break;

        //plots the graph for Ventricullar Fibrillation (a random wave)
        case VentricularFibrillation: {
        int previousY = graph.height() / 2; //sets the previous Y value (set at half the height of the graph)
        int amplitude = graph.height() / 4; //sets the high of the peak/value of the wave
        int pointSpacing = 6; //sets the spacing between each point

        //loops through all x values to plot the waveform
        for (int x = 0; x < graph.width(); x += pointSpacing) {
            int randomAmplitude = rand() % amplitude - (amplitude / 2); //calculates a random amplitude for a point
            int y = graph.height() / 2 + randomAmplitude; //calculates the y value of that point

            //creates a line between the previous y val to the new y val
            painter.drawLine(x - pointSpacing, previousY, x, y);
            previousY = y; //updates the previous Y val
        }
        break;
    }
        //plots the graph for Ventricullar Tachycardia
        case VentricularTachycardia: {
        int previousY = graph.height() / 2; //sets the previous Y value (set at half the height of the graph)
        int amplitude = graph.height() / 8; //sets the high of the peak/value of the wave
        int frequency = -20; //sets the frequency of the wave (is negative to flip the wave to more closely match the expected form)

        //loops through all x values to plot the waveform
        for (int x = 0; x < graph.width(); x++) {
            //creates a sine wave
            int y = (sin(x * M_PI / frequency) * amplitude) + graph.height() / 2;

            //creates a line between the previous y val to the new y val
            painter.drawLine(x - 1, previousY, x, y);
            previousY = y; //updates the previous Y val
        }
        break;
    }

    }

    painter.end(); //finishes the drawing
    return graph;
}

