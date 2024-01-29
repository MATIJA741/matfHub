#ifndef PLOTTER_HPP

#define PLOTTER_HPP

#include <iostream>
#include <vector>

#include <cmath>
#include <numeric>
#include <algorithm>
#include <matplot/matplot.h>

#include <cstring>

class Plotter{

public:
    //destruktor
    ~Plotter();

    static auto getPlotter() -> Plotter*;

    void operator()() const;

    void linSpace(double lowerBound, double upperBound, size_t step);
    void transformData(double (*func)(double));
    void transformDataParse(double (*func1)(double, char*), double (*func2)(double, char*));

    void savePlot();

    //setters
    void xData(std::vector<double> &newX);
    void yData(std::vector<double> &newY);
    void output(std::string text);

    //getters
    [[nodiscard]] std::vector<double> xData() const;
    [[nodiscard]] std::vector<double> yData() const;
    [[nodiscard]] auto output() const -> std::string;

private:

    //konstruktor
    Plotter();
    static Plotter* plotPtr;

    std::vector<double> _xData;
    std::vector<double> _yData;

    std::string _output;
};

#endif
