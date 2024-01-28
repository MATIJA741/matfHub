#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <vector>
#include "./matrix.hpp"

class History
{
public:
    ~History();

    static History* getHistory();
    //
    void writeHistory(std::string title, std::string line);
    std::string lastLine();
    void editLastLine(std::string line);
    
    void addMatrix(Matrix *m);
    void saveHistory();

    //
    std::vector<std::string> history() const;

    void history(std::vector<std::string> &newHistory);

    //static?
    const std::string EOC = "--------------------\n";

private:
    History(std::vector<std::string>h = std::vector<std::string>());
    static History* historyPtr;

    std::vector<Matrix*> _savedMatrices;

    std::vector<std::string> _history;
};


#endif // HISTORY_H
