
#include "../include/statistics.hpp"


//konstruktor
Statistics::Statistics(std::vector<double> const &xData)
    : _xData(xData){}

//destruktor
Statistics::~Statistics(){
    std::cout << "Unisten statistics" << std::endl;
}

//metode
double Statistics::mean(){

    auto data = this->xData();

    return std::accumulate(cbegin(data), cend(data), 0.0) / (data.size());
}

double Statistics::variance(){

    auto data = this->xData();
    auto mean = this->mean();

    std::vector<double> diff(data.size());
    transform(cbegin(data), cend(data), begin(diff), [mean](auto x){return pow(x-mean,2);});
    return std::accumulate(cbegin(diff), cend(diff), 0.0) / (data.size() - 1);
}

double Statistics::std(){
    return std::sqrt(this->variance());
}

double Statistics::median(){

    auto data = this->xData();
    auto n = data.size() / 2;

    nth_element(begin(data), begin(data)+n, end(data));
    auto med = data[n];

    if(!(data.size() & 1)) {
        auto maxIt = std::max_element(cbegin(data), cbegin(data)+n);
        med = (*maxIt + med) / 2.0;
    }

    return med;
}

double Statistics::mode(){

    std::map<double, int> occurences;
    auto data = this->xData();

    for(auto const &x : data)
        occurences[x]++;

    auto it = std::max_element(cbegin(occurences), cend(occurences), [](const auto &x, const auto &y) {
        return x.second < y.second;
    });

    return it->first;
}

//PLOTTER 
//TODO "calculator" this

void Statistics::histogram(){
        auto data = xData();
    matplot::hist(data);
    matplot::show();
}

void Statistics::barplot(){

    auto data = this->xData();
    auto names = this->textData();

    matplot::bar(data);
    matplot::gca()->x_axis().ticklabels(names);

    matplot::show();

}

void Statistics::boxplot(){

    auto data = this->xData();
    auto names = this->textData();
    matplot::boxplot(data, names);
    matplot::show();

}

//getters setters

std::vector<double> Statistics::xData() const{
    return this->_xData;
}

std::vector<std::string> Statistics::textData() const{
    return this->_textData;
}

void Statistics::xData(const std::vector<double> &newX){
    this->_xData = newX;
}

void Statistics::textData(const std::vector<std::string> &newTextData){
    this->_textData = newTextData;
}
