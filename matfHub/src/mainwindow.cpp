#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <stack>
// #include <iostream>
// #include "../include/notes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    notes = new class Notes();
    QString sPath = ""; //ovde kasnije dodati path i gurnuti ga u setRootPath
    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath(QDir::rootPath());
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->treeView->setModel(dirModel);

    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::rootPath());
    fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    ui->listView->setModel(fileModel);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(fileModel->setRootPath(sPath));

    navigationBefore.push(dirModel->fileInfo(index).absoluteFilePath());
}


void MainWindow::on_pushButton_clicked()
{
    if (!navigationBefore.empty()){
        QString sBack = navigationBefore.top();
        navigationBefore.pop();
        fileModel->setRootPath(sBack);
        dirModel->setRootPath(sBack);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    //Ovde za forward
}


void MainWindow::on_pushButton_3_clicked()
{
    //Ovde za return to home
}


// Funkcionalnosti Notes toolbara
void MainWindow::on_pushButton_5_clicked()
{
    notes->newClicked(ui);
}

void MainWindow::on_pushButton_6_clicked()
{
    notes->openClicked(ui, this);
}

void MainWindow::on_pushButton_8_clicked()
{
    notes->saveClicked(ui, this);
}

void MainWindow::on_pushButton_7_clicked()
{
    notes->copyClicked(ui);
}

void MainWindow::on_pushButton_11_clicked()
{
    notes->pasteClicked(ui);
}

void MainWindow::on_pushButton_9_clicked()
{
    notes->cutClicked(ui);
}

void MainWindow::on_pushButton_10_clicked()
{
    notes->undoClicked(ui);
}

void MainWindow::on_pushButton_4_clicked()
{
    notes->redoClicked(ui);
}
