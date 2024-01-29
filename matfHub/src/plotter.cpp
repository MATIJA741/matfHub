// Copyright 2024 MATF Studentarija
#include "../include/plotter.hpp"

Plotter* Plotter::plotPtr = nullptr;

// konstruktor
Plotter::Plotter() {}

Plotter::~Plotter() {
    std::cout << "Unisten plotter" << std::endl;
}

Plotter* Plotter::getPlotter() {
    if (plotPtr == nullptr) {
        plotPtr = new Plotter();
    }
    return plotPtr;
}

void Plotter::operator()() const {
    std::vector<double> zero(this->xData().size());
    std::fill(zero.begin(), zero.end(), 0);

    auto p = matplot::plot(zero, this->xData(), this->xData(), zero, this->xData(), this->yData());

    p[2]->line_width(3);
    p[2]->color("red");

    matplot::show();
}

// metode

void Plotter::linSpace(double lowerBound, double upperBound, size_t step) {
    std::vector<double>x = matplot::linspace(lowerBound, upperBound, step);
    this->xData(x);
    this->yData(x);
}

void Plotter::transformData(double (*func)(double)) {
    std::vector<double>y = this->yData();
    std::transform(begin(y), end(y), begin(y), [&func](auto &x) {return func(x); });

    this->yData(y);
}

void Plotter::savePlot() {
    (*this)();
    matplot::save("../img/chart.jpg");
}

// set
void Plotter::xData(std::vector<double> &newX) {
    this->_xData = newX;
}

void Plotter::yData(std::vector<double> &newY) {
    this->_yData = newY;
}

void Plotter::output(std::string text) {
    this->_output = text;
}

// get
std::vector<double> Plotter::xData() const {
    return this->_xData;
}

std::vector<double> Plotter::yData() const {
    return this->_yData;
}

std::string Plotter::output() const {
    return this->_output;
}
