#ifndef MATRIX_HPP

#define MATRIX_HPP

#include <QString>
#include <armadillo>
#include <iostream>
#include <vector>

class Matrix
{

  public:
    // konstruktori
    Matrix(const unsigned rows = 1, const unsigned columns = 1);
    Matrix(const unsigned rows, const unsigned columns, const QString data);
    Matrix(const Matrix &other);
    // destruktor

    // get
    auto getShape();
    auto toString();

    static auto getSaved(unsigned int index);

    // getters
    unsigned rows() const;
    unsigned columns() const;
    arma::mat data() const;

    // setters
    void rows(unsigned u);
    void columns(unsigned u);
    void data(arma::mat &newData);

    void setData(QString data);
    void setValue(double value, unsigned i, unsigned j);
    void reshapeMatrix(unsigned col, unsigned row);

    static void switchMatrices(Matrix *m1, Matrix *m2);

    // set
    auto saveMatrix();
    auto loadLeft(unsigned index);
    auto loadRight(unsigned index);

    // functions

    auto transpose();
    auto inverse();
    auto ones();
    auto eye();
    auto diag();

    // operators
    auto operator+(const Matrix &other) const;
    auto operator-(const Matrix &other) const;
    auto operator*(const Matrix &other) const;

    Matrix &operator=(const Matrix &other);

  private:
    unsigned m_rows;
    unsigned m_columns;
    arma::mat *m_data;

    // TODO this...
    static std::vector<Matrix *> m_savedMatrices;
};

std::ostream &operator<<(std::ostream &out, const Matrix *value);

#endif // MATRIX_HPP
