#ifndef PLOTTER_HPP

#define PLOTTER_HPP

#include <iostream>
#include <vector>

#include <cmath>
#include <numeric>
#include <algorithm>
#include <matplot/matplot.h>

#include <string>

class Plotter {
 public:
    // destruktor
    ~Plotter();

    static Plotter* getPlotter();

    void operator()() const;

    void linSpace(double lowerBound, double upperBound, size_t step);
    void transformData(double (*func)(double));
    void transformDataParse(double (*func1)(double, char*), double (*func2)(double, char*));

    void savePlot();

    // setters
    void xData(std::vector<double> &newX);
    void yData(std::vector<double> &newY);
    void output(std::string text);

    // getters
    std::vector<double> xData() const;
    std::vector<double> yData() const;
    std::string output() const;

 private:
    // konstruktor
    Plotter();
    static Plotter* plotPtr;

    std::vector<double> _xData;
    std::vector<double> _yData;

    std::string _output;
};

#endif
