#ifndef MATRIX_HPP

#define MATRIX_HPP

#include <iostream>
#include <vector>

#include <QString>

#include <armadillo>

class Matrix{

public:
//konstruktori
    Matrix(const unsigned rows = 1, const unsigned columns = 1);
    Matrix(const unsigned rows, const unsigned columns, const QString data);
    Matrix(const Matrix &other);
    //destruktor
    ~Matrix();

    //get
    auto getShape() -> std::pair<unsigned, unsigned>;
    QString toString();

    static auto getSaved(unsigned int index) -> Matrix*;

    //getters
    [[nodiscard]] auto rows() const -> unsigned;
    [[nodiscard]] auto columns() const -> unsigned;
    [[nodiscard]] auto data() const -> arma::mat;

    //setters
    void rows(unsigned u);
    void columns(unsigned u);
    void data(arma::mat &newData);

    void setData(QString data);
    void setValue(double value, unsigned i, unsigned j);
    void reshapeMatrix(unsigned col, unsigned row);

    static void switchMatrices(Matrix* m1, Matrix* m2);


    //set
    auto saveMatrix() -> unsigned;
    auto loadLeft(unsigned index) -> std::pair<unsigned, unsigned>;
    auto loadRight(unsigned index) -> std::pair<unsigned, unsigned>;

    //functions

    auto transpose() -> Matrix *;
    auto inverse() -> Matrix *;
    auto ones() -> Matrix *;
    auto eye() -> Matrix *;
    auto diag() -> Matrix *;

    //operators
    auto operator + (const Matrix &other) const -> Matrix *;
    auto operator - (const Matrix &other) const -> Matrix *;
    auto operator * (const Matrix &other) const -> Matrix *;


    auto operator = (const Matrix &other) -> Matrix &;

    
private:

    unsigned m_rows;
    unsigned m_columns;
    arma::mat* m_data;

    //TODO this...
    static std::vector<Matrix*> m_savedMatrices;

};

auto operator<<(std::ostream &out, const Matrix *value) -> std::ostream &;

#endif //MATRIX_HPP
