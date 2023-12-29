#include "../include/matrix.hpp"

#include <QStringList>
#include <QDebug>

#define DEBUG (qDebug() << __FILE__ << ":" << __LINE__ << ":\t")

std::vector<Matrix*> Matrix::m_savedMatrices;


//konstruktori
Matrix::Matrix(const unsigned int rows, const unsigned int columns):
    m_rows(rows), m_columns(columns)
{
    m_data = new arma::mat(rows, columns);
}
Matrix::Matrix(const unsigned rows, const unsigned columns, const QString data):
    m_rows(rows), m_columns(columns)
{
    m_data = new arma::mat(rows, columns);
}
Matrix::Matrix(const Matrix &other):
    m_rows(other.m_rows), m_columns(other.m_columns), m_data(other.m_data)
{

}
    //destruktor
Matrix::~Matrix(){

}

    //getteri
//TODO kontra?
std::pair<unsigned, unsigned> Matrix::getShape(){
    return {this->columns(), this->rows()};
}

QString Matrix::toString(){
    //DEBUG << "m_rows: " << m_rows;
    //DEBUG << "m_cols: " << m_columns;
    QString res = "";
    for(int i = 0; i < m_rows; ++i){
        res += "|\t";
        for(int j = 0; j < m_columns; ++j){
            //qDebug() << "i: " << i << " j: " << j;
            res += QString::number((*(this->m_data))(i,j)).toStdString();
            res += "\t";
        }
        res += "|\n";
    }
    return res;
}

Matrix* Matrix::getSaved(unsigned int index){
    return m_savedMatrices[index];
}

   // setteri

void Matrix::setData(QString data){
    int r=0, c=0;

    auto values = data.split(" ");

    for(auto &value : values){
        (*(this->m_data))(r, c) = value.toDouble();
        c++;
        if(c == m_columns){
            c = 0;
            r++;
        }
    }
}
//moze metod
void Matrix::setValue(double value, unsigned i, unsigned j){
    (*(this->m_data))(i, j) = value;
}

unsigned Matrix::saveMatrix(){
    Matrix* toSave = new Matrix(this->rows(), this->columns());
    arma::mat tmp = this->data();
    toSave->data(tmp);
    qDebug().noquote() << toSave->toString();
    m_savedMatrices.push_back(toSave);

    return m_savedMatrices.size()-1;
}
std::pair<unsigned, unsigned> Matrix::loadLeft(unsigned index){
    Matrix* toLoad = m_savedMatrices[index];
    qDebug().noquote() << toLoad->toString();
    this->reshapeMatrix(toLoad->m_columns, toLoad->m_rows);
    *this = *(toLoad);
    qDebug().noquote() << this->toString();
    return {toLoad->m_columns, toLoad->m_rows};
}
std::pair<unsigned, unsigned> Matrix::loadRight(unsigned index){
    Matrix* toLoad = m_savedMatrices[index];
    qDebug().noquote() << toLoad->toString();
    this->reshapeMatrix(toLoad->m_columns, toLoad->m_rows);
    *this = *(toLoad);
    qDebug().noquote() << this->toString();
    return {toLoad->m_columns, toLoad->m_rows};
}


    //functions
void Matrix::reshapeMatrix(unsigned col, unsigned row){
    arma::mat* newMat = new arma::mat(row, col);//namerno je kontra, don't worry about it
    newMat->fill(0);
    qDebug().noquote() << this->toString();
    for(int i = 0; i < (row < m_rows ? row : m_rows); ++i){
        for(int j = 0; j < (col < m_columns ? col : m_columns); ++j){
            (*newMat)(i, j) = (*m_data)(i, j);
        }
    }
    delete m_data;
    m_data = newMat;
    m_columns = col;
    m_rows = row;
}

//methods
Matrix *Matrix::transpose() {
    Matrix* newMatrix = new Matrix(this->columns(), this->rows());
    arma::mat tmp = arma::trans(this->data());
    newMatrix->data(tmp);

    return newMatrix;
}

Matrix *Matrix::ones(){
    Matrix* newMatrix = new Matrix(this->rows(), this->columns());
    arma::mat tmp = this->data().ones();
    newMatrix->data(tmp);

    return newMatrix;
}

Matrix *Matrix::eye(){
    Matrix* newMatrix = new Matrix(this->rows(), this->columns());
    arma::mat tmp = this->data().eye();
    newMatrix->data(tmp);

    return newMatrix;
}

Matrix *Matrix::inverse(){
    Matrix* newMatrix = new Matrix(this->rows(), this->columns());
    arma::mat tmp = arma::inv(this->data());
    newMatrix->data(tmp);

    return newMatrix;
}

Matrix *Matrix::diag(){
    Matrix* newMatrix = new Matrix(this->rows(), this->columns());
    arma::mat tmp = arma::diagmat(this->data());
    newMatrix->data(tmp);

    return newMatrix;
}

//operators
Matrix* Matrix::operator + (const Matrix &other) const{

    Matrix* newMat = new Matrix(this->rows(), this->columns());
    arma::mat *newData = new arma::mat(this->rows(), this->columns());

    *newData = this->data() + other.data();
    newMat->data(*newData);

    return newMat;
}

Matrix* Matrix::operator - (const Matrix &other) const{

    Matrix* newMat = new Matrix(this->rows(), this->columns());
    arma::mat *newData = new arma::mat(this->rows(), this->columns());

    *newData = this->data() - other.data();
    newMat->data(*newData);

    return newMat;
}

Matrix* Matrix::operator * (const Matrix &other) const{

    Matrix* newMat = new Matrix(this->rows(), this->columns());
    arma::mat *newData = new arma::mat(this->rows(), this->columns());

    *newData = this->data() * other.data();
    newMat->data(*newData);

    return newMat;
}


Matrix &Matrix::operator = (const Matrix &other) {

    this->columns(other.columns());
    this->rows(other.rows());

    arma::mat tmp = this->data();
    tmp.reshape(this->rows(), this->columns());

    return *this;
}


//getters
unsigned Matrix::rows() const{
    return this->m_rows;
}

unsigned Matrix::columns() const{
    return this->m_columns;
}

arma::mat Matrix::data() const{
    return *m_data;
}

//setters

void Matrix::rows(unsigned u){
    this->m_rows = u;
}

void Matrix::columns(unsigned u){
    this->m_columns = u;
}

void Matrix::data(arma::mat &newData){
    *(this->m_data) = newData;
}
