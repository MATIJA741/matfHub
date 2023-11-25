#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFileSystemModel>
#include <stack>

class MainWindow;
class FileManager
{
public:
    FileManager(MainWindow* mw);

    QString oldPath;
    QString appPath;
    QString hubPath;
    QString currPath;
    QString currentlySelected;

    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;

    std::stack<QString> navigationBefore;
    std::stack<QString> navigationAfter;


    void changeDir(const QString path);
    void createNewFolder();

    void dirViewDoubleClicked(const QModelIndex &index);
    void fileViewDoubleClicked(const QModelIndex &index);
    void backButtonClicked();
    void forwardButtonClicked();
    void homeButtonClicked();
    void dotDotButtonClicked();
    void currentFilePathEditingFinished();
    void fileViewCustomContextMenuRequested(const QPoint &pos);

private:
    MainWindow *m_mainWindow;

};

#endif // FILEMANAGER_H
