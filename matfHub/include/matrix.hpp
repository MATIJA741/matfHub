#ifndef MATRIX_HPP

#define MATRIX_HPP

#include <iostream>
#include <vector>

#include <QString>

#include <armadillo>

class Matrix {

public:
  // konstruktori
  Matrix(const unsigned rows = 1, const unsigned columns = 1);
  Matrix(const unsigned rows, const unsigned columns, const QString data);
  Matrix(const Matrix &other);
  // destruktor
  ~Matrix();

  // get
  std::pair<unsigned, unsigned> getShape();
  QString toString();

  static Matrix *getSaved(unsigned int index);

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
  unsigned saveMatrix();
  std::pair<unsigned, unsigned> loadLeft(unsigned index);
  std::pair<unsigned, unsigned> loadRight(unsigned index);

  // functions

  Matrix *transpose();
  Matrix *inverse();
  Matrix *ones();
  Matrix *eye();
  Matrix *diag();

  // operators
  Matrix *operator+(const Matrix &other) const;
  Matrix *operator-(const Matrix &other) const;
  Matrix *operator*(const Matrix &other) const;

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
