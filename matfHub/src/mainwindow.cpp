#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/schedule.h"
#include "../include/filemanager.hpp"
#include "../include/matrix.hpp"
#include "../include/parser.hpp"
#include "../include/plotter.hpp"
#include "../include/statistics.hpp"

#include <QSplitter>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <stack>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <QDebug>
#include <string>
#include <matplot/matplot.h>



// #include "../include/notes.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    schedule = std::make_unique<Schedule>();

    m_fileManager = new FileManager(this);

    // ako ne postoji dir MATF, pravimo ga
    if(!QDir("MATF").exists()){
        QDir().mkdir("MATF");
    }

    notes = new class Notes();
    QString sPath = ""; //ovde kasnije dodati path i gurnuti ga u setRootPath

    ui->dirView->setModel(m_fileManager->dirModel);
    ui->dirView->hideColumn(1);
    ui->dirView->hideColumn(2);
    ui->dirView->setColumnWidth(0, 200);
    //ui->dirView->setRootIndex(dirModel->setRootPath(hubPath));
    ui->fileView->setModel(m_fileManager->fileModel);
    ui->fileView->setRootIndex(m_fileManager->fileModel->setRootPath(m_fileManager->hubPath));
    ui->currentFilePath->setText(m_fileManager->currPath);
    ui->fileView->setSelectionMode(QAbstractItemView::ExtendedSelection);//klik odabere kliknutu i oddabere ostale; shift klik selektuje sve izmedju selektovane i kliknute, ctrl klik odabere kliknutu i ne oddabere ostale

    //TODO FIX THIS!!!
//    connect(ui->pbMatrixTest, &QPushButton::clicked, this, &MainWindow::pbMatrixTest);

    connect(ui->pbChangeRegular, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangeMatrix, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangePlot, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangeStat, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);

    connect(ui->pbParser, &QPushButton::clicked, this, &MainWindow::calculateRegular);

    connect(ui->pbMatrixTranspose, &QPushButton::clicked, this, &MainWindow::calculateMatrixTranspose);
    connect(ui->pbMatrixInverse, &QPushButton::clicked, this, &MainWindow::calculateMatrixInverse);
    connect(ui->pbMatrixDiag, &QPushButton::clicked, this, &MainWindow::calculateMatrixDiag);
    connect(ui->pbMatrixOne, &QPushButton::clicked, this, &MainWindow::calculateMatrixOne);

    connect(ui->pbMatrixLoad1, &QPushButton::clicked, this, &MainWindow::parseMatrix1);
    connect(ui->pbMatrixLoad2, &QPushButton::clicked, this, &MainWindow::parseMatrix2);

    connect(ui->pbMatrixAdd, &QPushButton::clicked, this, &MainWindow::calculateMatrixAdd);
    connect(ui->pbMatrixSubtract, &QPushButton::clicked, this, &MainWindow::calculateMatrixSubtract);
    connect(ui->pbMatrixMultiply, &QPushButton::clicked, this, &MainWindow::calculateMatrixMultiply);
    connect(ui->pbMatrixDivide, &QPushButton::clicked, this, &MainWindow::calculateMatrixDivide);

    connect(ui->pbPlot, &QPushButton::clicked, this, &MainWindow::plot);
    connect(ui->pbSin, &QPushButton::clicked, this, &MainWindow::plotSin);
    connect(ui->pbCos, &QPushButton::clicked, this, &MainWindow::plotCos);
    connect(ui->pbTan, &QPushButton::clicked, this, &MainWindow::plotTan);
    connect(ui->pbLn, &QPushButton::clicked, this, &MainWindow::plotLn);
    connect(ui->pbLog, &QPushButton::clicked, this, &MainWindow::plotLog);
    connect(ui->pbExp, &QPushButton::clicked, this, &MainWindow::plotExp);
    connect(ui->pbAbs, &QPushButton::clicked, this, &MainWindow::plotAbs);
    connect(ui->pbNeg, &QPushButton::clicked, this, &MainWindow::plotNeg);
    connect(ui->pbSquare, &QPushButton::clicked, this, &MainWindow::plotSquare);
    connect(ui->pbRoot, &QPushButton::clicked, this, &MainWindow::plotRoot);
    connect(ui->pbLinspace, &QPushButton::clicked, this, &MainWindow::plotLinspace);

    connect(ui->pbMean, &QPushButton::clicked, this, &MainWindow::statCalcMean);
    connect(ui->pbVariance, &QPushButton::clicked, this, &MainWindow::statCalcVariance);
    connect(ui->pbMedian, &QPushButton::clicked, this, &MainWindow::statCalcMedian);
    connect(ui->pbMode, &QPushButton::clicked, this, &MainWindow::statCalcMode);
    connect(ui->pbHist, &QPushButton::clicked, this, &MainWindow::statPlotHist);
    connect(ui->pbBar, &QPushButton::clicked, this, &MainWindow::statPlotBar);
    connect(ui->pbBox, &QPushButton::clicked, this, &MainWindow::statPlotBox);

}
//TODO move this, change serbian to english
// std::vector<double> cppSplit(std::string &s){

//     std::cout << s << std::endl;
//     //TODO more flexible?
//     std::string delimiter = ", ";

//     std::vector<double>niz;

//     size_t pos = 0;
//     std::string token;
//     while ((pos = s.find(delimiter)) != std::string::npos) {
//         token = s.substr(0, pos);
//         std::cout << token << " " << stod(token) << std::endl;
//         niz.push_back(stod(token));
//         s.erase(0, pos + delimiter.length());
//     }
//     niz.push_back(stod(s));

//     //TODO pointer?
//     return niz;
// }

//TODO study code
std::vector<double> cppSplit(const std::string &s) {
    std::cout << s << std::endl;

    // TODO: more flexible?
    std::string delimiter = ", ";

    std::vector<double> niz;

    std::istringstream ss(s);
    std::string token;

    // Temporarily set the locale to "C" to ensure '.' is used as the decimal point
    std::locale defaultLocale = std::locale::global(std::locale("C"));

    while (std::getline(ss, token, ',')) {
        // Trim whitespaces from the token
        token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);

        try {
            double value = std::stod(token);
            std::cout << token << " " << std::setprecision(15) << value << std::endl;
            niz.push_back(value);
        } catch (const std::invalid_argument& e) {
            // Handle invalid argument (e.g., if conversion fails)
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            // Handle out of range (e.g., if the value is too large or too small)
            std::cerr << "Out of range: " << e.what() << std::endl;
        }
    }

    // Restore the original locale
    std::locale::global(defaultLocale);

    // TODO: pointer?
    return niz;
}

//TODO change name
std::vector<std::string> cppSplitString(std::string &s){

    //TODO more flexible?
    std::string delimiter = ", ";

    std::vector<std::string>niz;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        niz.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    niz.push_back(s);

    //TODO pointer?
    return niz;
}

void MainWindow::showMatrix(Matrix *m){

    QString history = ui->tbParser->toPlainText();

    for(int i = 0; i < m->dimension1(); i++) {
        for(int j = 0; j < m->dimension2(); j++) {
            history += QString::number(m->data()(i,j));
            history += " ";
        }
        history += '\n';
    }
    history += "------------------------\n";
    ui->tbParser->setText(history);
}

MainWindow::~MainWindow()
{
    //je l ovde ide "close without saving?"?
    delete ui;
}

// Funkcionalnosti Notes toolbara
void MainWindow::on_newFileToolbarButton_clicked()
{
    notes->newClicked(ui);
}

void MainWindow::on_openFileToolbarButton_clicked()
{
    notes->openClicked(ui, this);
}

void MainWindow::on_saveToolbarButton_clicked()//save/save as? trenutno najlaksa opcija da se sacuva izmena jednog fajla u drugi je ctrl+a ctrl+c ctrl+n ctrl+v ctrl+s (takodje bilo bi kul da se prva tri dugmeta aktiviraju i na ctrl+n ctrl+s i ctrl+o
{
    notes->saveClicked(ui, this);
}

void MainWindow::on_copyToolbarButton_clicked()
{
    notes->copyClicked(ui);
}

void MainWindow::on_pasteToolbarButton_clicked()
{
    notes->pasteClicked(ui);
}

void MainWindow::on_cutToolbarButton_clicked()
{
    notes->cutClicked(ui);
}

void MainWindow::on_undoToolbarButton_clicked()
{
    notes->undoClicked(ui);
}

void MainWindow::on_redoToolbarButton_clicked()
{
    notes->redoClicked(ui);
}


// funkc fMenadzera
void MainWindow::on_dirView_clicked(const QModelIndex &index)
{
//ovo nam vr ne treba
}

//u levom pogledu dupli klik na folder smesta desni pogled u njega
void MainWindow::on_dirView_doubleClicked(const QModelIndex &index)
{
    m_fileManager->dirViewDoubleClicked(index);
}

//u desnom pogledu dupli klik na stavku vrsi odgovarajucu akciju
void MainWindow::on_fileView_doubleClicked(const QModelIndex &index)
{
    m_fileManager->fileViewDoubleClicked(index);
}

//navigacija u prethodno prikazani folder
void MainWindow::on_backButton_clicked()
{
    m_fileManager->backButtonClicked();
}

//navigacija u sledece prikazan folder
void MainWindow::on_forwardButton_clicked()
{
    m_fileManager->forwardButtonClicked();
}

//navigacija u glavni hab folder
void MainWindow::on_homeButton_clicked()
{
    m_fileManager->homeButtonClicked();
}

//navigacija u roditeljski folder
void MainWindow::on_dotDotButton_clicked()
{
    m_fileManager->dotDotButtonClicked();
}

//navigacija ka unesenoj putanji, ako postoji folder na tom mestu
void MainWindow::on_currentFilePath_editingFinished()
{
    m_fileManager->currentFilePathEditingFinished();
}

//kreiranje novog foldera unutar trenutnog
void MainWindow::on_newFolderButton_clicked()
{
    m_fileManager->createNewFolder();
}

//kreiranje popapa sa kontekst menijem u desnom pogledu
void MainWindow::on_fileView_customContextMenuRequested(const QPoint &pos)// !!!!!! ovo bi bilo lepo takodje izmestiti u filemanager.cpp ali prvo connect je iz QObject ili tako necega treba to nasledjivanje srediti drugo poslednja linija funkcije (i jos neke) zove privatne metode privatnih polja iz ui pa bi i to trebalo prebroditi nekako nekim seterom
{//pored komentara u prethodnoj liniji takodje ova f-ja postaje glomazna, bilo bi lepo mozda razdvojiti je u pozive f-ja koje pojedinacno prave akcije
    //TODO cut, copy, paste
    //TODO make new [...]
    //makar document (.txt) zbog notesa, jos nesto ako ima smisla u odnosu na to sta ostali naprave
    QMenu* menu = new QMenu(this);

    QAction* newFolderAction = new QAction("New Folder", this);
    menu->addAction(newFolderAction);
    connect(newFolderAction, &QAction::triggered, [=]() {//jednakost u prvim zagradama hvata sve promenjive iz pozivajuce funkcije u pozvanu, ne pitaj koja magija niti zasto je to potrebno..
        m_fileManager->createNewFolder();
    });

    QAction* selectAllAction = new QAction("Select All", this);
    menu->addAction(selectAllAction);
    connect(selectAllAction, &QAction::triggered, [=](){
        ui->fileView->clearSelection();
        ui->fileView->selectAll();//ovo ne radi kako sam zamislio i blago je reci da mi ide na zivce :)
        //mozda i radi nego ne prikazuje sve kao plavo, to plavo mozda nije select kako ga mi shvatamo
        //to plavo tacno jeste selekt kako ga mi shvatamo, samo je trebalo podesiti seleksn mod pogledu. obrisi ova tri komentara kada procitas.
    });

    int noSelected = countSelected(ui->fileView);
    if(noSelected == 1){
        QAction* renameAction = new QAction("Rename", this);
        menu->addAction(renameAction);
        connect(renameAction, &QAction::triggered, [=](){
            QModelIndex selectedIndex = getSelectedIndex(ui->fileView);
            QString oldName = m_fileManager->getNameFromIndex(selectedIndex);
            QString newName = QInputDialog::getText(this, "Preimenuj fajl", "Nov Naziv:", QLineEdit::Normal, oldName);
            m_fileManager->renameSelectedFile(selectedIndex, newName);
        });
    }

    if(noSelected > 0){
        QAction* deleteAction = new QAction("Delete", this);
        menu->addAction(deleteAction);
        connect(deleteAction, &QAction::triggered, [=](){
            QModelIndexList selectedIndices = getSelectedIndices(ui->fileView); //meni se indices vise svidja kao mnozina od index nego indexes ali Bojane slobodno promeni ako zelis nije mi bitno
            m_fileManager->deleteSelectedFiles(selectedIndices);
        });
    }


    menu->popup(ui->fileView->viewport()->mapToGlobal(pos));


}


//naredne dve funkcije mozda treba pomeriti u filemanager klasu ali mislim da je to pametnije uraditi kada krenemo da spajamo grane u zavisnosti od toga sta odlucimo kako ce se ponasati meni bar glavnog prozora

//izlazak iz aplikacije, logicno
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_smerBox_activated(int index)
{
    schedule->changeModule(ui, index);
}


void MainWindow::on_rasporedStartButton_clicked()
{
    schedule->findSchedule(ui);
}


void MainWindow::on_scrapeButton_clicked()
{
    schedule->scrapeSchedule(ui);
}

//promena glavnog hab foldera
void MainWindow::on_actionChangeHubLocation_triggered()
{
    QString newHubPath = QFileDialog::getExistingDirectory(this, "Odaberi direktorijum");
    m_fileManager->hubPath = newHubPath;   //hubPath treba uciniti trajnim nakon zatvaranja programa
    m_fileManager->currPath = m_fileManager->hubPath;
    ui->currentFilePath->setText(m_fileManager->currPath);
    ui->fileView->setRootIndex(m_fileManager->fileModel->setRootPath(m_fileManager->currPath));
}



//geteri seteri i ostala narusavanja mejnovih privatnosti
void MainWindow::fileViewSetPath(const QString path){
    ui->fileView->setRootIndex(m_fileManager->fileModel->setRootPath(path));
}
void MainWindow::currentFilePathSetPath(const QString path){
    ui->currentFilePath->setText(path);
}
QString MainWindow::currentFilePathGetPath(){
    return ui->currentFilePath->text();
}


int MainWindow::countSelected(const QListView* view){

    return view->selectionModel()->selectedIndexes().length();

}

QModelIndex MainWindow::getSelectedIndex(const QListView* view){
    return view->selectionModel()->selectedIndexes().first();
}

QModelIndexList MainWindow::getSelectedIndices(const QListView* view){
    return view->selectionModel()->selectedIndexes();
}


//kalkulator things
//void MainWindow::pbMatrixTest(){
//    Matrix *m1 = new Matrix(3, 3);
//    std::cout << m1 << std::endl;
//}


void MainWindow::changeStackedWidgetPage(){
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    QString buttonText = buttonSender->text();
    std::cout << buttonText.toStdString() << std::endl;

    if(buttonText == "Regular calculator")
        ui->stackedWidget->setCurrentIndex(0);
    else if(buttonText == "Matrix calculator")
        ui->stackedWidget->setCurrentIndex(1);
    else if(buttonText == "Plotting calculator")
        ui->stackedWidget->setCurrentIndex(2);
    else if(buttonText == "Statistics")
        ui->stackedWidget->setCurrentIndex(3);
}

Parser *parser = new Parser();
void MainWindow::calculateRegular(){

    //TODO make x+5 work (if possible)
//    Parser *parser = new Parser();

    //TODO so it works with char*
    char *expr = ui->leParser->text().toStdString().data();
    parser->eval_exp(expr);
    double res = parser->eval_exp(expr);
    QString qres = QString::number(res);

    QString history = ui->tbParser->toPlainText();

    history += expr;
    history += "\n";
    history += qres;
    history += "\n--------------\n";

    ui->leParser->setText(qres);
    ui->tbParser->setText(history);

//    delete parser; parser = nullptr;
}

//TODO what to do with it
Matrix *m1;
Matrix *m2;
//TODO figure out why it's working with ints instead of doubles
void MainWindow::parseMatrix1(){


    //TODO polymorph
    int dim1 = ui->leMatrixDim11->text().toInt();
    int dim2 = ui->leMatrixDim12->text().toInt();
    m1 = new Matrix(dim1, dim2);

    std::string in = ui->leMatrixData1->text().toStdString();

    std::vector<double> data = cppSplit(in);

    //TODO exception if data.size() isn't right
    //TODO fix this?
    arma::mat A = arma::conv_to<arma::mat>::from(data);
    A.reshape(dim1, dim2);

    m1->data(A);
    showMatrix(m1);

}

void MainWindow::parseMatrix2(){

    //TODO polymorph
    int dim1 = ui->leMatrixDim21->text().toInt();
    int dim2 = ui->leMatrixDim22->text().toInt();
    m2 = new Matrix(dim1, dim2);

    std::string in = ui->leMatrixData2->text().toStdString();

    std::vector<double> data = cppSplit(in);

    //TODO exception if data.size() isn't right
    //TODO fix this?
    arma::mat A = arma::conv_to<arma::mat>::from(data);
    A.reshape(dim1, dim2);

    m2->data(A);
    showMatrix(m2);
}

void MainWindow::calculateMatrixTranspose(){

    m1 = m1->transpose();
    std::cout << "Transposed matrix: " << std::endl;
    std::cout << m1 << std::endl;
    showMatrix(m1);
}
//TODO exceptions
void MainWindow::calculateMatrixInverse(){

    m1 = m1->inverse();
    std::cout << "Inverse" << std::endl;
    std::cout << m1 << std::endl;
    showMatrix(m1);
}

void MainWindow::calculateMatrixDiag(){

    m1 = m1->diag();
    std::cout << "Diag" << std::endl;
    std::cout << m1 << std::endl;
    showMatrix(m1);
}

void MainWindow::calculateMatrixOne(){

    m1 = m1->ones();
    std::cout << m1 << std::endl;
    std::cout << "One" << std::endl;
    showMatrix(m1);
}


//TODO save this somewhere?
void MainWindow::calculateMatrixAdd(){

    m1->data((*m1+*m2)->data());
    std::cout << *m1+*m2 << std::endl;
    std::cout << "Add" << std::endl;
    showMatrix(m1);
}

void MainWindow::calculateMatrixSubtract(){

    m1->data((*m1-*m2)->data());
    std::cout << *m1-*m2 << std::endl;
    std::cout << "Subtract" << std::endl;
    showMatrix(m1);
}

void MainWindow::calculateMatrixMultiply(){

    m1->data((*m1**m2)->data());
    std::cout << *m1**m2 << std::endl;
    std::cout << "Multiply" << std::endl;
    showMatrix(m1);
}
//TODO exceptions
void MainWindow::calculateMatrixDivide(){

    m1->data((*m1 / *m2)->data());
    std::cout << *m1 / *m2 << std::endl;
    std::cout << "Divide" << std::endl;
    showMatrix(m1);
}

//////////////////////////////////////////////////////
//TODO global classes?
void MainWindow::plot(){
//    Plotter *plt = new Plotter("sin");

    plt->plot();
    std::cerr <<"Crtanje: " << std::endl;
    std::cerr << "\t" << ui->leLinspaceLB->text().toStdString() << " ";
    std::cerr << ui->leLinspaceUB->text().toStdString() << std::endl;
}

void MainWindow::plotLinspace(){
    //TODO GLOBAL CLASSES?
    plt = new Plotter("dd");

    double lowerBound = ui->leLinspaceLB->text().toDouble();
    double upperBound = ui->leLinspaceUB->text().toDouble();
    double numOfDots = ui->leLinspaceS->text().toDouble();
    plt->linSpace(lowerBound, upperBound, numOfDots);

    // //TODO remove
    // std::vector<double>x = plt->xData();
    // plt->yData(x);

    ui->leState->setText("x");
    ui->lbLin->setText("Vektor je uspesno ucitan");
    std::cerr << "Resetovan y" << std::endl;
    std::cerr <<"Postavljen linspace" << std::endl;
}

void MainWindow::plotSin(){

    ui->leState->setText("sin(" + ui->leState->text() + ")" );

    plt->transformSin();

    //TODO sme ovako? Enkapsulacija
//    plt->plotSin(plt->xData());
}

void MainWindow::plotCos(){

    ui->leState->setText("cos(" + ui->leState->text() + ")" );

    plt->transformCos();
}

void MainWindow::plotTan(){

    ui->leState->setText("tan(" + ui->leState->text() + ")" );

    plt->transformTan();
}

void MainWindow::plotLn(){

    ui->leState->setText("ln(" + ui->leState->text() + ")" );

    plt->transformLn();
}

void MainWindow::plotLog(){

    ui->leState->setText("log(" + ui->leState->text() + ")" );

    plt->transformLog();
}

void MainWindow::plotExp(){

    ui->leState->setText("e^(" + ui->leState->text() + ")" );

    plt->transformExp();
}

void MainWindow::plotAbs(){

    ui->leState->setText("|" + ui->leState->text() + "|" );

    plt->transformAbs();
}

void MainWindow::plotNeg(){
    std::string state = ui->leState->text().toStdString();
    if (state[0] == '-') {
        std::string new_state = state.substr(2, state.length()-3);
        ui->leState->setText(QString::fromStdString(new_state));
    } else
        ui->leState->setText("-(" + ui->leState->text() + ")" );

    plt->transformNeg();
}

void MainWindow::plotSquare(){

    ui->leState->setText("(" + ui->leState->text() + ")²" );

    plt->transformSquare();
}

void MainWindow::plotRoot(){

    ui->leState->setText("√(" + ui->leState->text() + ")" );

    plt->transformRoot();
}

//stat
//TODO global?
Statistics *stat = new Statistics();
void MainWindow::statCalcMean(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    ui->tbParser->setText(QString::number(stat->mean()));
}

void MainWindow::statCalcVariance(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));
    
    ui->tbParser->setText(QString::number(stat->variance()));
}

void MainWindow::statCalcMedian(){

    //TODO razmisli
    // stat->loadData();
    
    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    ui->tbParser->setText(QString::number(stat->median()));
}

void MainWindow::statCalcMode(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    ui->tbParser->setText(QString::number(stat->mode()));
}

void MainWindow::statPlotHist(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));
    
    stat->histogram();
}

void MainWindow::statPlotBar(){

    auto input1 = ui->leStat->text().toStdString();
    auto input2 = ui->leStatNames->text().toStdString();
    stat->xData(cppSplit(input1));
    stat->textData(cppSplitString(input2));

    stat->barplot();
}

void MainWindow::statPlotBox(){
    auto input1 = ui->leStat->text().toStdString();
    auto input2 = ui->leStatNames->text().toStdString();
    stat->xData(cppSplit(input1));
    stat->textData(cppSplitString(input2));

    stat->boxplot();
}


