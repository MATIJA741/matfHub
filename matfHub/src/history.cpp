#include "../include/history.hpp"
#include "../include/config.hpp"

#include <fstream>
#include <QFile>


History* History::historyPtr = nullptr;

History::History(std::vector<std::string>h){

}
History::~History(){
    std::cout << "History unisten" << std::endl;
}

History* History::getHistory(){
    if(historyPtr == nullptr){
        historyPtr = new History();
    }
    return historyPtr;
}

//friendly?
void History::writeHistory(std::string title, std::string line){
    title += "\n";
    line += "\n";

    this->_history.push_back(title);
    this->_history.push_back(line);
    this->_history.push_back(this->EOC);
}


std::string History::lastLine(){
    auto history = this->history();
    int numOfLastLine = history.size()-1;
    return history[numOfLastLine];
}

void History::editLastLine(std::string line){
    if(this->history().size() == 0){
        this->writeHistory(" ", " ");
    }
    auto history = this->history();
    int numOfLastLine = history.size()-1;
    history[numOfLastLine] = line;
    this->history(history);
}

void History::addMatrix(Matrix *m){

    this->_savedMatrices.push_back(m);
}

void History::saveHistory(){
    QFile file(Config::getConfig()->getHubPath() + "/hist.txt");
    file.open(QIODevice::WriteOnly);
    auto history = this->history();
    for(auto const &x : history){
        file.write(x.c_str());
    }
}

//get
std::vector<std::string> History::history() const{
    return this->_history;
}

//set
void History::history(std::vector<std::string> &newHistory){
    this->_history = newHistory;
}

