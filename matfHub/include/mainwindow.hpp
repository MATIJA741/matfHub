#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <stack>
#include <QListView>

#include "notes.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FileManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fileViewSetPath(const QString path);
    void currentFilePathSetPath(const QString path);
    QString currentFilePathGetPath();

private slots:
    void on_newFileToolbarButton_clicked();

    void on_openFileToolbarButton_clicked();

    void on_saveToolbarButton_clicked();

    void on_copyToolbarButton_clicked();

    void on_pasteToolbarButton_clicked();

    void on_cutToolbarButton_clicked();

    void on_undoToolbarButton_clicked();

    void on_redoToolbarButton_clicked();

    void on_dirView_doubleClicked(const QModelIndex &index);

    void on_backButton_clicked();

    void on_forwardButton_clicked();

    void on_homeButton_clicked();

    void on_fileView_doubleClicked(const QModelIndex &index);

    void on_dirView_clicked(const QModelIndex &index);

    void on_currentFilePath_editingFinished();

    void on_dotDotButton_clicked();

    void on_newFolderButton_clicked();

    void on_fileView_customContextMenuRequested(const QPoint &pos);

    void on_actionExit_triggered();

    void on_actionChangeHubLocation_triggered();

    //calculator things
    void pbMatrixTest();

private:
    Ui::MainWindow *ui;

    Notes *notes;

    FileManager* m_fileManager;
    int countSelected(const QListView* view);
    QModelIndex getSelectedIndex(const QListView* view);
    QModelIndexList getSelectedIndices(const QListView* view);

};
#endif // MAINWINDOW_HPP
