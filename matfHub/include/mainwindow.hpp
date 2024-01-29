#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "history.hpp"
#include "schedule.h"
#include <QListView>
#include <QMainWindow>
#include <QString>
#include <QtCore>
#include <QtGui>
#include <memory>
#include <stack>

#include <QSortFilterProxyModel>

#include "calendar.h"
#include "matrix.hpp"
#include "notes.h"
#include "parser.hpp"
#include "plotter.hpp"
#include "statistics.hpp"
#include "views.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class FileManager;
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void fileViewSetPath(const QString path);
  void currentFilePathSetPath(const QString path); // v
  Notes *m_notes;
  FileManager *m_fileManager;
  Ui::MainWindow *ui;
  QPalette lightPalette;

  QString currentFilePathGetPath();

private slots:

  void actionExitTriggered();
  void actionChangeHubLocationTriggered();

  // notes slots
  void on_newFileNotesButton_clicked();
  void on_openFileNotesButton_clicked();
  void on_saveNotesButton_clicked();
  void on_copyNotesButton_clicked();
  void on_pasteNotesButton_clicked();
  void on_cutNotesButton_clicked();
  void on_undoNotesButton_clicked();
  void on_redoNotesButton_clicked();

  // fmanager slots
  void dirViewDoubleClicked(const QModelIndex &index);

  void backButtonClicked();
  void forwardButtonClicked();
  void homeButtonClicked();
  void dotDotButtonClicked();
  void newFolderButtonClicked();

  void currentFilePathEditingFinished();

  void fileViewDoubleClicked(const QModelIndex &index);

  void fileViewCustomContextMenuRequested(const QPoint &pos,
                                          QAbstractItemView *view);
  void showFileView(QAbstractItemView *view);

  // calculator things

  void changeStackedWidgetPage();

  // calc functions
  void writeToHistoryTB(History *history);
  // regular
  void calculateRegular();

  QStringList matrixStringToStringList(QString str);
  void reshapeMatrix(unsigned dim1, unsigned dim2, unsigned pos,
                     QStringList content);
  void reshapeMatrix(unsigned dim1, unsigned dim2, unsigned pos);
  void reshapeMatrix1();
  void reshapeMatrix2();
  QString readM1Data();
  QString readM2Data();

  void calculateMatrixAdd();
  void calculateMatrixSubtract();
  void calculateMatrixMultiply();
  void calculateMatrixDiag();

  void calculateMatrixTranspose();
  void calculateMatrixInverse();
  void calculateMatrixEye();
  void calculateMatrixOne();

  void saveMatrix();
  void loadMatrix(unsigned int pos, QStringList strLst, unsigned d1,
                  unsigned d2);
  void switchMatrices();

  // plotting
  void plot();
  void plotSin();
  void plotCos();
  void plotTan();
  void plotLn();
  void plotLog();
  void plotExp();
  void plotAbs();
  void plotNeg();
  void plotSquare();
  void plotRoot();
  void plotParse();

  void plotLinspace();
  void savePlotting();

  // Staistics
  void statCalcMean();
  void statCalcVariance();
  void statCalcStd();
  void statCalcMedian();
  void statCalcMode();
  void statPlotHist();
  void statPlotBar();
  void statPlotBox();

  void historySave();

  // kmiljanify
  void on_smerBox_activated(int index);

  void on_rasporedStartButton_clicked();

  void on_scrapeButton_clicked();

  void on_sledeciButton_clicked();

  void on_SacuvajButton_clicked();

  void on_prethodniButton_clicked();

  void on_calendarWidget_clicked(const QDate &date);

  void on_addToCalendarButton_clicked();

  void on_removeItemButton_clicked();

  void on_removeAllButton_clicked();

  void on_actionLight_Mode_triggered();

private:
  std::stack<QString> navigationBefore;
  std::stack<QString> navigationAfter;
  std::unique_ptr<Schedule> schedule;

  Calendar *calendar;

  int countSelected(const QAbstractItemView *view);
  QModelIndex getSelectedIndex(const QAbstractItemView *view);
  QModelIndexList getSelectedIndices(const QAbstractItemView *view);
  QAbstractItemView *m_activeFileView;

  void setUpFileView(
      /*enum tipPogleda*/); // funkcija prima enum neki tipa i onda pravi pogled
                            // na osnovu enuma i podesava ga, kao sto smo u
                            // mejnu radili do sada, zatim brise dete od
                            // ui->fileViewLayout i daje mu novo dete, ovo koje
                            // je napravio

  Parser *parser;
  Parser *parserPlot;
  Matrix *m1;
  Matrix *m2;
  Matrix *m3;
  Plotter *plt;
  Statistics *stat;
  History *history;

  int countSelected(const QListView *view);
  QModelIndex getSelectedIndex(const QListView *view);
  QModelIndexList getSelectedIndices(const QListView *view);
};
#endif // MAINWINDOW_HPP
