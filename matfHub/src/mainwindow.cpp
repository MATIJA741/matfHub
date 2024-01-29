#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/schedule.h"
#include "../include/filemanager.hpp"
#include "../include/matrix.hpp"
#include "../include/parser.hpp"
#include "../include/config.hpp"

#include "../include/calendar.h"

#include "../include/plotter.hpp"
#include "../include/statistics.hpp"
#include "../include/views.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <stack>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <QDebug>
#include <string>
#include <matplot/matplot.h>

#include <fstream>
#include <map>
#include <QPalette>
#include <QStyleFactory>
#include <QStyle>

// #include <iostream>
// #include "../include/notes.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG (qDebug() << __FILENAME__ << ":" << __LINE__ << ":\t")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    schedule = std::make_unique<Schedule>();

    m_fileManager = new FileManager(this);

    QString hubPath = Config::getConfig()->getHubPath();

    // ako ne postoji dir MATF, pravimo ga
    if(!QDir(hubPath).exists()){//-||-
        qDebug() << hubPath;
        QDir().mkdir(hubPath);
    }

    // Kalendar
    calendar = new class Calendar(ui);


    // Notes klasa
    m_notes = new class Notes(ui);
    QString sPath = ""; //ovde kasnije dodati path i gurnuti ga u setRootPath

    //Ucitavanje rasporeda
    QString schedulePath = Config::getConfig()->getConfigPath() + "/raspored.json";
    QFile scheduleFile(schedulePath);
    if (scheduleFile.exists() && scheduleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        schedule->loadSchedule(ui);
        scheduleFile.close();
    }
    // Ctrl+S shortcut za cuvanje datoteke u notesu
    QShortcut *saveShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    connect(saveShortcut, &QShortcut::activated, this, &MainWindow::on_saveNotesButton_clicked);
    // Ctrl+O za otvaranje datoteke u notesu
    QShortcut *openShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this);
    connect(openShortcut, &QShortcut::activated, this, &MainWindow::on_openFileNotesButton_clicked);
    // Ctrl+N za novu datoteku u notesu
    QShortcut *newShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_N), this);
    connect(newShortcut, &QShortcut::activated, this, &MainWindow::on_newFileNotesButton_clicked);

    ui->dirView->setModel(m_fileManager->m_dirModel);
    ui->dirView->hideColumn(1);
    ui->dirView->hideColumn(2);
    ui->dirView->setColumnWidth(0, 200);

    connect(ui->dirView, &QTreeView::doubleClicked, this, &MainWindow::dirViewDoubleClicked);

    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::backButtonClicked);
    connect(ui->forwardButton, &QPushButton::clicked, this, &MainWindow::forwardButtonClicked);
    connect(ui->homeButton, &QPushButton::clicked, this, &MainWindow::homeButtonClicked);
    connect(ui->dotDotButton, &QPushButton::clicked, this, &MainWindow::dotDotButtonClicked);
    connect(ui->newFolderButton, &QPushButton::clicked, this, &MainWindow::newFolderButtonClicked);

    connect(ui->currentFilePath, &QLineEdit::editingFinished, this, &MainWindow::currentFilePathEditingFinished);

    setUpFileView();




//TODO mozda neki connectCalc(this) koji ovo sve zove da bi konstruktor bio cistiji? slicno za ostale konekcije
    plt = Plotter::getPlotter();
    stat = Statistics::getStatistics();

    parser = new Parser();
    parserPlot = new Parser();

    m1 = new Matrix(0, 0);
    m2 = new Matrix(0, 0);
    m3 = new Matrix(0, 0);

    history = History::getHistory();

    connect(ui->pbChangeRegular, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangeMatrix, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangePlot, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);
    connect(ui->pbChangeStat, &QPushButton::clicked, this, &MainWindow::changeStackedWidgetPage);

    connect(ui->pbParser, &QPushButton::clicked, this, &MainWindow::calculateRegular);

    connect(ui->pbMatrixTranspose, &QPushButton::clicked, this, &MainWindow::calculateMatrixTranspose);
    connect(ui->pbMatrixInverse, &QPushButton::clicked, this, &MainWindow::calculateMatrixInverse);
    connect(ui->pbMatrixEye, &QPushButton::clicked, this, &MainWindow::calculateMatrixEye);
    connect(ui->pbMatrixOne, &QPushButton::clicked, this, &MainWindow::calculateMatrixOne);

    connect(ui->pbMatrixLoad1, &QPushButton::clicked, this, &MainWindow::reshapeMatrix1);
    connect(ui->pbMatrixLoad2, &QPushButton::clicked, this, &MainWindow::reshapeMatrix2);

    connect(ui->pbMatrixAdd, &QPushButton::clicked, this, &MainWindow::calculateMatrixAdd);
    connect(ui->pbMatrixSubtract, &QPushButton::clicked, this, &MainWindow::calculateMatrixSubtract);
    connect(ui->pbMatrixMultiply, &QPushButton::clicked, this, &MainWindow::calculateMatrixMultiply);
    connect(ui->pbMatrixDiagonal, &QPushButton::clicked, this, &MainWindow::calculateMatrixDiag);

    //connect(ui->listFileView, &QPushButton::clicked, this, &MainWindow::on_fileView_doubleClicked);

    //kalk

    connect(ui->pbSaveMatrix, &QPushButton::clicked, this, &MainWindow::saveMatrix);

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
    connect(ui->pbPlotParse, &QPushButton::clicked, this, &MainWindow::plotParse);
    connect(ui->pbLinspace, &QPushButton::clicked, this, &MainWindow::plotLinspace);
    connect(ui->pbPlotSave, &QPushButton::clicked, this, &MainWindow::savePlotting);

    connect(ui->pbMean, &QPushButton::clicked, this, &MainWindow::statCalcMean);
    connect(ui->pbVariance, &QPushButton::clicked, this, &MainWindow::statCalcVariance);
    connect(ui->pbStd, &QPushButton::clicked, this, &MainWindow::statCalcStd);
    connect(ui->pbMedian, &QPushButton::clicked, this, &MainWindow::statCalcMedian);
    connect(ui->pbMode, &QPushButton::clicked, this, &MainWindow::statCalcMode);
    connect(ui->pbHist, &QPushButton::clicked, this, &MainWindow::statPlotHist);
    connect(ui->pbBar, &QPushButton::clicked, this, &MainWindow::statPlotBar);
    connect(ui->pbBox, &QPushButton::clicked, this, &MainWindow::statPlotBox);
    connect(ui->pbSwitchMatrices, &QPushButton::clicked, this, &MainWindow::switchMatrices);

    connect(ui->pbHistorySave, &QPushButton::clicked, this, &MainWindow::historySave);

    connect(ui->textEdit, &QTextEdit::textChanged, this, [this](){
        m_notes->notesContentChanged(this, ui);
    });

}

std::vector<double> cppSplit(const std::string &s) {

    std::string delimiter = ", ";

    std::vector<double> niz;

    std::istringstream ss(s);
    std::string token;

    std::locale defaultLocale = std::locale::global(std::locale("C"));

    while (std::getline(ss, token, ',')) {
        token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);

        try {
            double value = std::stod(token);
            std::cout << token << " " << std::setprecision(15) << value << std::endl;
            niz.push_back(value);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << e.what() << std::endl;
        }
    }

    std::locale::global(defaultLocale);

    return niz;
}

std::vector<std::string> cppSplitString(std::string &s){

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

    return niz;
}

inline auto square(double s) -> double{
    return pow(s, 2);
}

inline auto negation(double s) -> double{
    return -s;
}

MainWindow::~MainWindow()
{
    calendar->saveHistory();
    delete ui;
}


// Funkcionalnosti Notes toolbara

// Nova Datoteka
void MainWindow::on_newFileNotesButton_clicked()
{
    m_notes->newClicked(ui, this);
}

// Otvori Datoteku
void MainWindow::on_openFileNotesButton_clicked()
{
    if(ui->tabWidgetMatfHub->currentIndex() == 2){
        m_notes->openClicked(ui, this);
    }
}

// Sacuvaj Datoteku (Save As ako nije prethodno cuvana, Save ako jeste.)
void MainWindow::on_saveNotesButton_clicked()//save/save as? trenutno najlaksa opcija da se sacuva izmena jednog fajla u drugi je ctrl+a ctrl+c ctrl+n ctrl+v ctrl+s (takodje bilo bi kul da se prva tri dugmeta aktiviraju i na ctrl+n ctrl+s i ctrl+o
{
    if(ui->tabWidgetMatfHub->currentIndex() == 2){
        m_notes->saveClicked(ui, this);
    }
}

// Kopiranje teksta
void MainWindow::on_copyNotesButton_clicked()
{
    m_notes->copyClicked(ui);
}

// Nalepljivanje teksta
void MainWindow::on_pasteNotesButton_clicked()
{
    m_notes->pasteClicked(ui);
}

// Isecanje teksta
void MainWindow::on_cutNotesButton_clicked()
{
    m_notes->cutClicked(ui);
}

// Korak unazad
void MainWindow::on_undoNotesButton_clicked()
{
    m_notes->undoClicked(ui);
}

// Korak unapred
void MainWindow::on_redoNotesButton_clicked()
{
    m_notes->redoClicked(ui);
}



// funkc fMenadzera


//u levom pogledu dupli klik na folder smesta desni pogled u njega
void MainWindow::dirViewDoubleClicked(const QModelIndex &index)
{
    m_fileManager->dirViewDoubleClicked(index);
}

//u desnom pogledu dupli klik na stavku vrsi odgovarajucu akciju
void MainWindow::fileViewDoubleClicked(const QModelIndex &index)
{
    m_fileManager->fileViewDoubleClicked(index);
}

//navigacija u prethodno prikazani folder
void MainWindow::backButtonClicked()
{
    m_fileManager->backButtonClicked();
    ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija u sledece prikazan folder
void MainWindow::forwardButtonClicked()
{
    m_fileManager->forwardButtonClicked();
    ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija u glavni hab folder
void MainWindow::homeButtonClicked()
{
    m_fileManager->homeButtonClicked();
    ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija u roditeljski folder
void MainWindow::dotDotButtonClicked()
{
    m_fileManager->dotDotButtonClicked();
    ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//navigacija ka unesenoj putanji, ako postoji folder na tom mestu
void MainWindow::currentFilePathEditingFinished()
{
    m_fileManager->currentFilePathEditingFinished();
    ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

//kreiranje novog foldera unutar trenutnog
void MainWindow::newFolderButtonClicked()
{
    m_fileManager->createNewFolder();
}

//kreiranje popapa sa kontekst menijem u desnom pogledu
void MainWindow::fileViewCustomContextMenuRequested(const QPoint &pos, QAbstractItemView* view)// TODO !!!!!! ovo bi bilo lepo takodje izmestiti u filemanager.cpp ali prvo connect je iz QObject ili tako necega treba to nasledjivanje srediti drugo poslednja linija funkcije (i jos neke) zove privatne metode privatnih polja iz ui pa bi i to trebalo prebroditi nekako nekim seterom
{//TODO pored komentara u prethodnoj liniji takodje ova f-ja postaje glomazna, bilo bi lepo mozda razdvojiti je u pozive f-ja koje pojedinacno prave akcije
    //TODO cut, copy, paste mozda?

    QMenu* menu = new QMenu(this);

    QAction* newFolderAction = new QAction("New Folder", this);
    menu->addAction(newFolderAction);
    connect(newFolderAction, &QAction::triggered, this, [=]() {
        m_fileManager->createNewFolder();
    });

    QAction* newDocumentAction = new QAction("New Document", this);
    menu->addAction(newDocumentAction);
    connect(newDocumentAction, &QAction::triggered, this, [=](){
        m_fileManager->createNewDocument();
    });

    QAction* selectAllAction = new QAction("Select All", this);
    menu->addAction(selectAllAction);
    connect(selectAllAction, &QAction::triggered, this, [=](){
        for(int i = 0; i <  ui->fileViewLayout->count(); ++i){
            auto widget = ui->fileViewLayout->itemAt(i)->widget();
            QAbstractItemView* view = static_cast<QAbstractItemView*>(widget);
            view->clearSelection();
            view->selectAll();
        }
    });

    QMenu* sortSubMenu = new QMenu("Sort by", menu);

    QAction* sortByName = new QAction("Name", this);
    sortSubMenu->addAction(sortByName);
    connect(sortByName, &QAction::triggered, this, [=](){
        m_fileManager->sortByName();
    });

    QAction* sortByDate = new QAction("Date", this);
    sortSubMenu->addAction(sortByDate);
    connect(sortByDate, &QAction::triggered, this, [=](){
        m_fileManager->sortByDate();
    });

    QAction* sortBySize = new QAction("Size", this);
    sortSubMenu->addAction(sortBySize);
    connect(sortBySize, &QAction::triggered, this, [=](){
        m_fileManager->sortBySize();
    });

    QAction* sortByType = new QAction("Type", this);
    sortSubMenu->addAction(sortByType);
    connect(sortByType, &QAction::triggered, this, [=](){
        m_fileManager->sortByType();
    });

    menu->addMenu(sortSubMenu);

    QMenu* viewSubMenu = new QMenu("View", menu);

    QAction* list = new QAction("List", this);//ovo mozda ima smisla smestiti u petlju samo bi onda naziv akcije morao nekako da se dobije od parsiranja naziva ovih pogleda TOTO?
    viewSubMenu->addAction(list);
    connect(list, &QAction::triggered, this, [this](){MainWindow::showFileView(ui->listFileView);});
    QAction* table = new QAction("Details", this);
    viewSubMenu->addAction(table);
    connect(table, &QAction::triggered, this, [this](){MainWindow::showFileView(ui->detailsFileView);});
    QAction* thumbnails = new QAction("Thumbnails", this);
    viewSubMenu->addAction(thumbnails);
    connect(thumbnails, &QAction::triggered, this, [this](){MainWindow::showFileView(ui->thumbnailsFileView);});

    menu->addMenu(viewSubMenu);

    int noSelected = countSelected(view);
    if(noSelected == 1){
        QAction* renameAction = new QAction("Rename", this);
        menu->addAction(renameAction);
        connect(renameAction, &QAction::triggered, this, [=](){
            QModelIndex selectedIndex = getSelectedIndex(view);
            QString oldName = m_fileManager->getNameFromIndex(selectedIndex);
            QString newName = QInputDialog::getText(this, "Preimenuj fajl", "Nov Naziv:", QLineEdit::Normal, oldName);
            m_fileManager->renameSelectedFile(selectedIndex, newName);
        });
    }

    if(noSelected > 0){
        QAction* deleteAction = new QAction("Delete", this);
        menu->addAction(deleteAction);
        connect(deleteAction, &QAction::triggered, this, [=](){
            QModelIndexList selectedIndices = getSelectedIndices(view); //meni se indices vise svidja kao mnozina od index nego indexes ali Bojane slobodno promeni ako zelis nije mi bitno
            m_fileManager->deleteSelectedFiles(selectedIndices);
        });
    }

    menu->popup(view->viewport()->mapToGlobal(pos));

}

void MainWindow::showFileView(QAbstractItemView* view){

    for(int i = 0; i < ui->fileViewLayout->count(); ++i){
        QAbstractItemView* widget = static_cast<QAbstractItemView*>(ui->fileViewLayout->itemAt(i)->widget());
        widget->hide();
    }
    view->show();

}
//naredne dve funkcije mozda treba pomeriti u filemanager klasu ali mislim da je to pametnije uraditi kada krenemo da spajamo grane u zavisnosti od toga sta odlucimo kako ce se ponasati meni bar glavnog prozora

//izlazak iz aplikacije, logicno
void MainWindow::actionExitTriggered()
{
    calendar->saveHistory();
    QApplication::quit();
}


void MainWindow::on_actionLight_Mode_triggered()
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
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

void MainWindow::on_sledeciButton_clicked()
{
    schedule->nextSchedule(ui);
}

void MainWindow::on_prethodniButton_clicked()
{
    schedule->prevSchedule(ui);
}

void MainWindow::on_SacuvajButton_clicked()
{
    schedule->saveSchedule(ui);
    calendar->initializeClassMap();
}

//promena glavnog hab foldera
void MainWindow::actionChangeHubLocationTriggered()
{
    QString newHubPath = QFileDialog::getExistingDirectory(this, "Odaberi direktorijum");
    Config::getConfig()->setHubPath(newHubPath);
    m_fileManager->m_currPath = newHubPath;
    m_fileManager->m_hubPath = newHubPath;
    ui->currentFilePath->setText(m_fileManager->m_currPath);
    ui->detailsFileView->setRootIndex(m_fileManager->m_fileModel->setRootPath(m_fileManager->m_currPath));
}



//geteri seteri i ostala narusavanja mejnovih privatnosti
void MainWindow::fileViewSetPath(const QString path){
    for(int i = 0; i < ui->fileViewLayout->count(); ++i){
        auto view = static_cast<QAbstractItemView*>(ui->fileViewLayout->itemAt(i)->widget());
        view->setRootIndex(m_fileManager->m_fileModel->setRootPath(path));
        if(QTableView* table = dynamic_cast<QTableView*>(view)){
            table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);//TODO iz nekog razloga cini mi se da ovo ne radi UVEK ako se prvi put ulazi u neki folder?
        }
    }
}
void MainWindow::currentFilePathSetPath(const QString path){
    ui->currentFilePath->setText(path);
}
QString MainWindow::currentFilePathGetPath(){
    return ui->currentFilePath->text();
}



int MainWindow::countSelected(const QAbstractItemView* view){
    return view->selectionModel()->selectedIndexes().length();
}

QModelIndex MainWindow::getSelectedIndex(const QAbstractItemView* view){
    return view->selectionModel()->selectedIndexes().first();
}

QModelIndexList MainWindow::getSelectedIndices(const QAbstractItemView* view){
    return view->selectionModel()->selectedIndexes();
}

void MainWindow::setUpFileView(/*tipPogleda*/){

    ui->currentFilePath->setText(m_fileManager->m_currPath);

    connect(ui->detailsFileView, &QTableView::doubleClicked, this, [this](){//TODO koji signal je odgovarajuc? nesto sto odgovara promeni m_currPath i nista drugo? komplikovan nacin bi bio currPath retvoriti u klasu koja pored stringa ima i signal koji se ovde salje pri promeni QStringa ali to me smara trenutno...
        ui->detailsFileView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    });

    for(int i = 0; i < ui->fileViewLayout->count(); ++i){
        auto widget = ui->fileViewLayout->itemAt(i)->widget();
        QAbstractItemView* view = static_cast<QAbstractItemView*>(widget);//auto ga vec kastuje u konkretan tip dakle QTableView, QListView itd B) ali kompajler u daljem kodu to ne zna nego je u fazonu ok ovo ce sigurno biti bar QObject tako da ipak mora da se kastuje sto je idiotski ali tako je D: jednom kada naprave funkcionalne procesore koji lenjo izracunavaju i kada ceo svet predje na normalne funkcionalne interpretirane jezike ovo nece biti problem ;)
        //qDebug() << view;//apdejt: i ovaj staticki kast ga kastuje u klasu koja treba da bude, nagadjam jer je AbstractView apstraktna klasa TODO nauci vise o cppu

        view->setModel(m_fileManager->m_fileModel);
        view->setRootIndex(m_fileManager->m_fileModel->setRootPath(m_fileManager->m_hubPath));
        view->setSelectionMode(QAbstractItemView::ExtendedSelection);

        //connect(ui->detailsFileView, &QTableView::doubleClicked, m_fileManager, &FileManager::fileViewDoubleClicked); ovo ne radi a zelim da proradi, TODO istraziti
        connect(view, &QAbstractItemView::doubleClicked, this, &MainWindow::fileViewDoubleClicked);
        connect(view, &QAbstractItemView::customContextMenuRequested, this, [this, view](QPoint pos){
            fileViewCustomContextMenuRequested(pos, view);
        });

        if(auto tableView = dynamic_cast<QTableView*>(view)){

             tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

            tableView->verticalHeader()->setVisible(false);
        }

    }

    //naredna linija zakomentarisana za lakse dibagovanje, TODO obrisati je kad tad
    showFileView(ui->listFileView);

}

//kalkulator things

void MainWindow::changeStackedWidgetPage(){
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    QString buttonText = buttonSender->text();
    std::cout << buttonText.toStdString() << std::endl;

    if(buttonText == "Kalkulator")
        ui->stackedWidget->setCurrentIndex(0);
    else if(buttonText == "Matrični Kalkulator")
        ui->stackedWidget->setCurrentIndex(1);
    else if(buttonText == "Kalkulator za Iscrtavanje")
        ui->stackedWidget->setCurrentIndex(2);
    else if(buttonText == "Statistika")
        ui->stackedWidget->setCurrentIndex(3);
}

//TODO global
int linesWritten = 0;

void MainWindow::writeToHistoryTB(History* history) {
    auto vecHistory = history->history();
    for (int i = linesWritten; i < vecHistory.size(); i++)
        this->ui->tbHistory->append(QString::fromStdString(vecHistory[i]));

    linesWritten = vecHistory.size();
}
void MainWindow::calculateRegular(){

    //TODO so it works with char*
    char *expr = ui->leParser->text().toStdString().data();
    double res = parser->evalExpression(expr);
    QString qres = QString::number(res);

    history->writeHistory(expr, qres.toStdString());
    writeToHistoryTB(history);

    ui->leParser->setText(qres);
}


QString MainWindow::readM1Data(){
    QString res("");
    //ovaj kod ce verovatno biti uzasan i pun bagova, radna verzija
    auto rows = ui->scrollAreaM1widget->children().first();
    for(auto &cols : rows->children()){
        for(auto &field : cols->children()){
            QLineEdit* lEdit = qobject_cast<QLineEdit*>(field);
            res += lEdit->text();
        }
    }
    return res;
}
QString MainWindow::readM2Data(){
    QString res("");
    //ovaj kod ce verovatno biti uzasan i pun bagova, radna verzija
    auto rows = ui->scrollAreaM2widget->children().first();
    for(auto &cols : rows->children()){
        for(auto &field : cols->children()){
            QLineEdit* lEdit = qobject_cast<QLineEdit*>(field);
            res += lEdit->text();
        }
    }
    return res;
}

QStringList MainWindow::matrixStringToStringList(QString str){
    str.replace("\n", "");
    str.replace("|", "");
    QStringList strLst = str.split("\t");
    strLst.removeAll("");
    return strLst;
}

void MainWindow::reshapeMatrix(unsigned dim1, unsigned dim2, unsigned pos){

    auto [oldDim2, oldDim1] = (pos == 1 ? m1->getShape() : m2->getShape());
    if(dim1 == oldDim1 && dim2 == oldDim2){
        return;
    }

    unsigned realDim1 = (dim1 <= 25 ? dim1 : 25);
    unsigned realDim2 = (dim2 <= 25 ? dim2 : 25);
    pos == 1 ? m1->reshapeMatrix(realDim2, realDim1) : m2->reshapeMatrix(realDim2, realDim1);

    QVBoxLayout* rows;
    QWidget* scrollAreaWidget = (pos == 1 ? ui->scrollAreaM1widget : ui->scrollAreaM2widget);
    if(scrollAreaWidget->children().isEmpty()){
        rows = new QVBoxLayout;
        scrollAreaWidget->setLayout(rows);
    } else {
        rows = qobject_cast<QVBoxLayout*>(scrollAreaWidget->children().first());
        while(!rows->isEmpty()){
            auto row = rows->takeAt(0)->layout();
            while(!row->isEmpty()){
                delete row->takeAt(0)->widget();
            }
            delete row;
        }
    }
    QStringList content = matrixStringToStringList(pos == 1 ? (m1->toString()) : (m2->toString()));
    for(int i = 0; i < realDim1; ++i){
        QHBoxLayout* fields = new QHBoxLayout;
        for(int j = 0; j < realDim2; ++j){
            QLineEdit* field = new QLineEdit;
            field->setText(content.at(j+i*realDim2));
            if(pos == 1){
                connect(field, &QLineEdit::editingFinished , this, [i, j, field, this](){
                    m1->setValue(field->text().toDouble(), i, j);
                    qDebug().noquote() << m1->toString();
                });
            }else{
                connect(field, &QLineEdit::editingFinished , this, [i, j, field, this](){
                    m2->setValue(field->text().toDouble(), i, j);
                    qDebug().noquote() << m2->toString();
                });
            }

            field->setMaximumSize(64, 32);
            fields->addWidget(field);
        }
        rows->addLayout(fields);
    }
}

void MainWindow::reshapeMatrix(unsigned dim1, unsigned dim2, unsigned pos, QStringList content){//TODO napraviti enum za pos LEFT i RIGHT

    auto [oldDim2, oldDim1] = (pos == 1 ? m1->getShape() : m2->getShape());

    unsigned realDim1 = (dim1 <= 25 ? dim1 : 25);
    unsigned realDim2 = (dim2 <= 25 ? dim2 : 25);

    pos == 1 ? m1->reshapeMatrix(realDim2, realDim1) : m2->reshapeMatrix(realDim2, realDim1);
    DEBUG << content;

    QVBoxLayout* rows;
    QWidget* scrollAreaWidget = (pos == 1 ? ui->scrollAreaM1widget : ui->scrollAreaM2widget);
    if(scrollAreaWidget->children().isEmpty()){
        rows = new QVBoxLayout;
        scrollAreaWidget->setLayout(rows);
    }else{
        rows = qobject_cast<QVBoxLayout*>(scrollAreaWidget->children().first());
        while(!rows->isEmpty()){
            auto row = rows->takeAt(0)->layout();
            while(!row->isEmpty()){
                delete row->takeAt(0)->widget();
            }
            delete row;
        }
    }
    for(int i = 0; i < realDim1; ++i){
        QHBoxLayout* fields = new QHBoxLayout;
        for(int j = 0; j < realDim2; ++j){
            QLineEdit* field = new QLineEdit;
            field->setText(content.at(j+i*realDim2));
            if(pos == 1){
                m1->setValue(field->text().toDouble(), i, j);
                connect(field, &QLineEdit::editingFinished , this, [i, j, field, this](){
                    m1->setValue(field->text().toDouble(), i, j);
                    qDebug().noquote() << m1->toString();
                });
            }else{
                m2->setValue(field->text().toDouble(), i, j);
                connect(field, &QLineEdit::editingFinished , this, [i, j, field, this](){
                    m2->setValue(field->text().toDouble(), i, j);
                    qDebug().noquote() << m2->toString();
                });
            }

            field->setMaximumSize(64, 32);
            fields->addWidget(field);
        }
        rows->addLayout(fields);
    }
}

void MainWindow::reshapeMatrix1(){
    int dim1 = ui->leMatrixDim11->text().toInt();
    int dim2 = ui->leMatrixDim12->text().toInt();
    reshapeMatrix(dim1, dim2, 1);
}

void MainWindow::reshapeMatrix2(){
    int dim1 = ui->leMatrixDim21->text().toInt();
    int dim2 = ui->leMatrixDim22->text().toInt();
    reshapeMatrix(dim1, dim2, 2);
}



void MainWindow::calculateMatrixAdd(){

    if(m1->columns() != m2->columns() || m1->rows ()!= m2->rows()){
        ui->leError->setText("Matrice razilicih dimenzija");
        return;
    }

    m3 = *m1 + *m2;

    history->writeHistory("Sabiranje matrica:", m3->toString().toStdString());
    writeToHistoryTB(history);
}

void MainWindow::calculateMatrixSubtract(){

    if(m1->columns() != m2->columns() || m1->rows ()!= m2->rows()){
        ui->leError->setText("Matrice razilicih dimenzija");
        return;
    }

    m3 = *m1 - *m2;

    history->writeHistory("Oduzimanje matrica:", m3->toString().toStdString());
    writeToHistoryTB(history);
}

void MainWindow::calculateMatrixMultiply(){

    if(m1->columns() != m2->rows()){
        ui->leError->setText("Matrice pogresnih dimenzija");
        return;
    }

    m3 = *m1 * *m2;
    DEBUG << m3->toString();

    history->writeHistory("Mnozenje matrica:", m3->toString().toStdString());
    writeToHistoryTB(history);
}

void MainWindow::calculateMatrixDiag(){
    if(m1->rows() != m1->columns()){
        ui->leError->setText("Matrica nije kvadratna");
        return;
    }

    m3 = m1->diag();
    history->writeHistory("Dijagonalna matrica:", m3->toString().toStdString());
    writeToHistoryTB(history);
}

void MainWindow::calculateMatrixTranspose(){

    m3 = m1->transpose();

    history->writeHistory("Transponovanje matrice:", m3->toString().toStdString());
    writeToHistoryTB(history);
}

//TODO singular matrices
void MainWindow::calculateMatrixInverse(){

    if(m1->rows() != m1->columns()){
        ui->leError->setText("Matrica nije kvadratna");
        return;
    }

    m3 = m1->inverse();

    history->writeHistory("Inverz matrice:", m3->toString().toStdString());
    writeToHistoryTB(history);
}

void MainWindow::calculateMatrixEye(){

    m3 = m1->eye();

    history->writeHistory("Identicka matrica:", m3->toString().toStdString());
    writeToHistoryTB(history);
}

void MainWindow::calculateMatrixOne(){

    m3 = m1->ones();

    history->writeHistory("Jedinicna matrica:", m3->toString().toStdString());
    writeToHistoryTB(history);
}


void MainWindow::saveMatrix(){
    unsigned index = m3->saveMatrix();
    QPushButton* loadLeft = new QPushButton("Ucitaj levo");
    QPushButton* loadRight = new QPushButton("Ucitaj desno");
    QHBoxLayout* savedM = new QHBoxLayout();
    auto layout = ui->savedMatricesLayout;
    layout->addLayout(savedM);
    QLabel* matrixName = new QLabel("b 865m");//TODO Mat::getSavedVectorLen.toStr
    savedM->addWidget(matrixName);
    savedM->addWidget(loadLeft);
    savedM->addWidget(loadRight);
    connect(loadLeft, &QPushButton::clicked, this, [this, index](){
        auto [d2, d1] = m3->loadLeft(index);
        loadMatrix(1, matrixStringToStringList(Matrix::getSaved(index)->toString()), d1, d2);
    });
    connect(loadRight, &QPushButton::clicked, this, [this, index](){
        auto [d2, d1] = m3->loadRight(index);
        loadMatrix(2, matrixStringToStringList(Matrix::getSaved(index)->toString()), d1, d2);
    });
}

void MainWindow::loadMatrix(unsigned int pos, QStringList strLst, unsigned d1, unsigned d2){
    reshapeMatrix(d1, d2, pos, strLst);
}

//////////////////////////////////////////////////////

void MainWindow::plotLinspace(){

    double lowerBound = ui->leLinspaceLB->text().toDouble();
    double upperBound = ui->leLinspaceUB->text().toDouble();
    double numOfDots = ui->leLinspaceS->text().toDouble();
    plt->linSpace(lowerBound, upperBound, numOfDots);

    ui->leState->setText("x");
    ui->leError->setText("Vektor je uspesno ucitan");
    std::cerr << "Resetovan y" << std::endl;
    std::cerr <<"Postavljen linspace" << std::endl;
}

void MainWindow::plot(){


    (*plt)();
    std::cerr <<"Crtanje: " << std::endl;
    std::cerr << "\t" << ui->leLinspaceLB->text().toStdString() << " ";
    std::cerr << ui->leLinspaceUB->text().toStdString() << std::endl;

    std::string lowerBound = ui->leLinspaceLB->text().toStdString();
    std::string upperBound = ui->leLinspaceLB->text().toStdString();
    std::string numOfDots = ui->leLinspaceS->text().toStdString();
    std::string function = ui->leState->text().toStdString();

    std::string linspaceString = "Plot: (" + lowerBound + ", " + upperBound + ")";
    linspaceString += " Dots: " + numOfDots;

    history->writeHistory(linspaceString, function);
    writeToHistoryTB(history);
}


void MainWindow::plotSin(){

    ui->leState->setText("sin(" + ui->leState->text() + ")" );

    plt->transformData(sin);

}



void MainWindow::plotCos(){

    ui->leState->setText("cos(" + ui->leState->text() + ")" );

    plt->transformData(cos);
}

void MainWindow::plotTan(){

    ui->leState->setText("tan(" + ui->leState->text() + ")" );

    plt->transformData(tan);
}

void MainWindow::plotLn(){

    ui->leState->setText("ln(" + ui->leState->text() + ")" );

    plt->transformData(log);
}

void MainWindow::plotLog(){

    ui->leState->setText("log(" + ui->leState->text() + ")" );

    plt->transformData(log10);
}

void MainWindow::plotExp(){

    ui->leState->setText("e^(" + ui->leState->text() + ")" );

    plt->transformData(exp);
}

void MainWindow::plotAbs(){

    ui->leState->setText("|" + ui->leState->text() + "|" );

    plt->transformData(std::abs);
}

void MainWindow::plotNeg(){
    std::string state = ui->leState->text().toStdString();

    if (state[0] == '-') {
        std::string new_state = state.substr(2, state.length()-3);
        ui->leState->setText(QString::fromStdString(new_state));
    } else
        ui->leState->setText("-(" + ui->leState->text() + ")" );

    plt->transformData(negation);
}

void MainWindow::plotSquare(){

    ui->leState->setText("(" + ui->leState->text() + ")²" );

    plt->transformData(square);
}

void MainWindow::plotRoot(){

    ui->leState->setText("√(" + ui->leState->text() + ")" );

    plt->transformData(sqrt);
}

void MainWindow::plotParse(){

    char *expr = strdup(ui->leState->text().toStdString().c_str());

    double tmp;
    std::vector<double> xs = plt->xData();
    if(xs.size() == 0){
        throw std::exception();
    }
    std::vector<double> ys = plt->xData();

    for (int i = 0; i < xs.size(); i++) {
        char tmp1[20];
        strcpy(tmp1, "x=");
        strcat(tmp1, std::to_string(xs[i]).c_str());

        tmp = parserPlot->evalExpression(tmp1);

        ys[i] = parserPlot->evalExpression(expr);
    }

    free(expr);

    plt->yData(ys);
    (*plt)();

    std::string lowerBound = ui->leLinspaceLB->text().toStdString();
    std::string upperBound = ui->leLinspaceLB->text().toStdString();
    std::string numOfDots = ui->leLinspaceS->text().toStdString();
    std::string function = ui->leState->text().toStdString();

    std::string linspaceString = "Plot: (" + lowerBound + ", " + upperBound + ")";
    linspaceString += " Dots: " + numOfDots;

    history->writeHistory(linspaceString, function);
    writeToHistoryTB(history);
}

void MainWindow::savePlotting(){
    plt->savePlot();
}

//stat
void MainWindow::statCalcMean(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    auto result = stat->mean();

    std::string hOutput = "Mean: \n" + input;
    history->writeHistory(hOutput, std::to_string(result));
    writeToHistoryTB(history);
}

void MainWindow::statCalcVariance(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    auto result = stat->variance();

    std::string hOutput = "Variance: \n" + input;
    history->writeHistory(hOutput, std::to_string(result));
    writeToHistoryTB(history);
}

void MainWindow::statCalcStd(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    auto result = stat->std();

    std::string hOutput = "Std: \n" + input;
    history->writeHistory(hOutput, std::to_string(result));
    writeToHistoryTB(history);
}

void MainWindow::statCalcMedian(){
    
    auto input = ui->leStat->text().toStdString();

    if(input.size() == 0){
        ui->leError->setText("Nisu uneti podaci");
        return;
    }

    stat->xData(cppSplit(input));

    auto result = stat->median();

    std::string hOutput = "Median: \n" + input;
    history->writeHistory(hOutput, std::to_string(result));
    writeToHistoryTB(history);
}

void MainWindow::statCalcMode(){

    auto input = ui->leStat->text().toStdString();
    stat->xData(cppSplit(input));

    auto result = stat->mode();

    std::string hOutput = "Mode: \n" + input;
    history->writeHistory(hOutput, std::to_string(result));
    writeToHistoryTB(history);
}

void MainWindow::statPlotHist(){

    auto input = ui->leStat->text().toStdString();

    if(input.size() == 0){
        ui->leError->setText("Nisu uneti podaci");
        return;
    }

    stat->xData(cppSplit(input));
    
    stat->histogram();

    history->writeHistory("Histogram", input);
}

void MainWindow::statPlotBar(){

    auto input1 = ui->leStat->text().toStdString();

    if(input1.size() == 0){
        ui->leError->setText("Nisu uneti podaci");
        return;
    }

    auto input2 = ui->leStatNames->text().toStdString();
    stat->xData(cppSplit(input1));
    stat->textData(cppSplitString(input2));

    stat->barplot();

    history->writeHistory("Barplot", input1 + input2);
}

void MainWindow::statPlotBox(){
    auto input1 = ui->leStat->text().toStdString();
    auto input2 = ui->leStatNames->text().toStdString();
    stat->xData(cppSplit(input1));
    stat->textData(cppSplitString(input2));

    stat->boxplot();

    history->writeHistory("Boxplot", input1 + input2);
}

//HISTORY

void MainWindow::historySave(){
    history->saveHistory();
    std::cout << "History saved!" << std::endl;
}


void MainWindow::switchMatrices(){

    auto [d11, d12] = m1->getShape();
    auto [d21, d22] = m2->getShape();

    Matrix::switchMatrices(m1, m2);

    DEBUG << "========================================";
    qDebug().noquote() << m1->toString();
    qDebug().noquote() << m2->toString();
    DEBUG << "========================================";

    reshapeMatrix(d21, d22, 1, matrixStringToStringList(m1->toString()));
    reshapeMatrix(d11, d12, 2, matrixStringToStringList(m2->toString()));
}
// Kalendar
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    calendar->dateChanged(ui, date);
}

void MainWindow::on_addToCalendarButton_clicked()
{
    calendar->taskAdded(ui);
}

void MainWindow::on_removeItemButton_clicked()
{
    calendar->removeTask(ui);
}

void MainWindow::on_removeAllButton_clicked()
{
    calendar->removeAll(ui);
}
