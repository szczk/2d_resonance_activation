#ifndef __AVERAGE_TIME_HH__
#define __AVERAGE_TIME_HH__

#include <iostream>
#include <map>
#include "Measure.hh"
#include "../core/Settings.hh"
#include <cmath>

using namespace std;

class SingleGammaAverageTime;

/**
 * Miara - sredni czas ucieczki (MFPT)
 *
 */
class AverageTimeMeasure : public Measure {

private:

    map<double,SingleGammaAverageTime *> * averages;

    void init();
    void destroy();

    SingleGammaAverageTime * getSingleGammaAverage(double gamma);

public:

    AverageTimeMeasure();
    ~AverageTimeMeasure();


    void addValue(double gamma ,double value);
    double getAverage(double gamma);
    void save();
};





class SingleGammaAverageTime {

private:
    double average;
    unsigned int count;


public:
    SingleGammaAverageTime() : average(0.0), count(0) {}


    void addValue(double v) {
        //moving average
        average = average + ((v - average)/(count + 1.0));
        ++count;
// 	  clog << average << endl;
    }

    double getAverage() {
        return average;

    }
    unsigned int getCount() {
        return count;
    }
    
    

};

#endif
