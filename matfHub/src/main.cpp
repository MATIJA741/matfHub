#include "../include/mainwindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("windows");
    MainWindow w;
    w.show();
    return a.exec();
}
