#include "../include/matrix.hpp"

#include <QDebug>
#include <QStringList>

#define DEBUG (qDebug() << __FILE__ << ":" << __LINE__ << ":\t")

std::vector<Matrix *> Matrix::m_savedMatrices;

// konstruktori
Matrix::Matrix(const unsigned int rows, const unsigned int columns)
    : m_rows(rows),
      m_columns(columns)
{
    m_data = new arma::mat(rows, columns);
}
Matrix::Matrix(const unsigned rows, const unsigned columns, const QString data)
    : m_rows(rows),
      m_columns(columns)
{
    m_data = new arma::mat(rows, columns);
}
Matrix::Matrix(const Matrix &other)
    : m_rows(other.m_rows),
      m_columns(other.m_columns),
      m_data(other.m_data)
{
}

// getteri
// TODO kontra?
auto Matrix::getShape()
{
    return std::pair{this->rows(), this->columns()};
}

auto Matrix::toString()
{
    // DEBUG << "m_rows: " << m_rows;
    // DEBUG << "m_cols: " << m_columns;
    QString res = "";
    for (int i = 0; i < m_rows; ++i) {
        res += "|\t";
        for (int j = 0; j < m_columns; ++j) {
            // qDebug() << "i: " << i << " j: " << j;
            res += QString::number((*(this->m_data))(i, j)).toStdString();
            res += "\t";
        }
        res += "|\n";
    }
    return res;
}

auto Matrix::getSaved(unsigned int index)
{
    return m_savedMatrices[index];
}

// setteri

void Matrix::setData(QString data)
{
    int r = 0, c = 0;

    auto values = data.split(" ");

    for (auto &value : values) {
        (*(this->m_data))(r, c) = value.toDouble();
        c++;
        if (c == m_columns) {
            c = 0;
            r++;
        }
    }
}
// moze metod
void Matrix::setValue(double value, unsigned i, unsigned j)
{
    (*(this->m_data))(i, j) = value;
}

auto Matrix::saveMatrix()
{
    Matrix *toSave = new Matrix(this->rows(), this->columns());
    arma::mat tmp = this->data();
    toSave->data(tmp);
    qDebug().noquote() << toSave->toString();
    m_savedMatrices.push_back(toSave);

    return m_savedMatrices.size() - 1;
}
auto Matrix::loadLeft(unsigned index)
{
    Matrix *toLoad = m_savedMatrices[index];
    qDebug().noquote() << toLoad->toString();
    this->reshapeMatrix(toLoad->m_columns, toLoad->m_rows);
    *this = *(toLoad);
    qDebug().noquote() << this->toString();
    return std::pair{toLoad->m_columns, toLoad->m_rows};
}
auto Matrix::loadRight(unsigned index)
{
    Matrix *toLoad = m_savedMatrices[index];
    qDebug().noquote() << toLoad->toString();
    this->reshapeMatrix(toLoad->m_columns, toLoad->m_rows);
    *this = *(toLoad);
    qDebug().noquote() << this->toString();
    return std::pair{toLoad->m_columns, toLoad->m_rows};
}

// functions
void Matrix::reshapeMatrix(unsigned col, unsigned row)
{
    arma::mat *newMat =
        new arma::mat(row, col); // namerno je kontra, don't worry about it
    newMat->fill(0);
    qDebug().noquote() << this->toString();
    for (int i = 0; i < (row < m_rows ? row : m_rows); ++i) {
        for (int j = 0; j < (col < m_columns ? col : m_columns); ++j) {
            (*newMat)(i, j) = (*m_data)(i, j);
        }
    }
    delete m_data;
    m_data = newMat;
    m_columns = col;
    m_rows = row;
}

void Matrix::switchMatrices(Matrix *m1, Matrix *m2)
{

    arma::mat data1 = *(m1->m_data);
    unsigned col1 = m1->m_columns;
    unsigned row1 = m1->m_rows;

    arma::mat data2 = *(m2->m_data);
    unsigned col2 = m2->m_columns;
    unsigned row2 = m2->m_rows;

    //    *(m1->m_data) = *(m2->m_data);
    //    m1->m_columns = m2->m_columns;
    //    m1->m_rows = m2->m_rows;

    *(m2->m_data) = data1;
    m2->m_columns = col1;
    m2->m_rows = row1;

    *(m1->m_data) = data2;
    m1->m_columns = col2;
    m1->m_rows = row2;
}

// methods
auto Matrix::transpose()
{
    Matrix *newMatrix = new Matrix(this->columns(), this->rows());
    arma::mat tmp = arma::trans(this->data());
    newMatrix->data(tmp);

    return newMatrix;
}

auto Matrix::ones()
{
    Matrix *newMatrix = new Matrix(this->rows(), this->columns());
    arma::mat tmp = this->data().ones();
    newMatrix->data(tmp);

    return newMatrix;
}

auto Matrix::eye()
{
    Matrix *newMatrix = new Matrix(this->rows(), this->columns());
    arma::mat tmp = this->data().eye();
    newMatrix->data(tmp);

    return newMatrix;
}

auto Matrix::inverse()
{
    Matrix *newMatrix = new Matrix(this->rows(), this->columns());
    arma::mat tmp = arma::inv(this->data());
    newMatrix->data(tmp);

    return newMatrix;
}

auto Matrix::diag()
{
    Matrix *newMatrix = new Matrix(this->rows(), this->columns());
    arma::mat tmp = arma::diagmat(this->data());
    newMatrix->data(tmp);

    return newMatrix;
}

// operators
Matrix *Matrix::operator+(const Matrix &other) const
{

    Matrix *newMat = new Matrix(this->rows(), this->columns());
    arma::mat *newData = new arma::mat(this->rows(), this->columns());

    *newData = this->data() + other.data();
    newMat->data(*newData);

    return newMat;
}

Matrix *Matrix::operator-(const Matrix &other) const
{

    Matrix *newMat = new Matrix(this->rows(), this->columns());
    arma::mat *newData = new arma::mat(this->rows(), this->columns());

    *newData = this->data() - other.data();
    newMat->data(*newData);

    return newMat;
}

Matrix *Matrix::operator*(const Matrix &other) const
{

    Matrix *newMat = new Matrix(this->rows(), other.columns());
    arma::mat *newData = new arma::mat(this->rows(), other.columns());

    *newData = this->data() * other.data();
    newMat->data(*newData);

    return newMat;
}

Matrix &Matrix::operator=(const Matrix &other)
{

    this->columns(other.columns());
    this->rows(other.rows());

    arma::mat tmp = this->data();
    tmp.reshape(this->rows(), this->columns());

    return *this;
}

// getters
unsigned Matrix::rows() const
{
    return this->m_rows;
}

unsigned Matrix::columns() const
{
    return this->m_columns;
}

arma::mat Matrix::data() const
{
    return *m_data;
}

// setters

void Matrix::rows(unsigned u)
{
    this->m_rows = u;
}

void Matrix::columns(unsigned u)
{
    this->m_columns = u;
}

void Matrix::data(arma::mat &newData)
{
    *(this->m_data) = newData;
}
