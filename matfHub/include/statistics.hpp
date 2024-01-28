#ifndef STATISTICS_HPP

#define STATISTICS_HPP

#include <cmath>
#include <iostream>
#include <map>
#include <matplot/matplot.h>
#include <numeric>
#include <vector>

class Statistics
{

  public:
    // destruktor
    ~Statistics();

    static auto getStatistics();

    // metode
    auto mean() -> double;
    auto variance() -> double;
    auto std() -> double;
    auto median() -> double;
    auto mode() -> double;

    void boxplot();
    void barplot();
    void histogram();

    // setters
    void xData(const std::vector<double> &newX);
    void textData(const std::vector<std::string> &newTextData);

    // getters
    std::vector<double> xData() const;
    std::vector<std::string> textData() const;

  private:
    Statistics(std::vector<double> const &xData = std::vector<double>());
    static Statistics *statPtr;

    std::vector<double> _xData;
    std::vector<std::string> _textData;
};

#endif // STATISTICS_HPP
