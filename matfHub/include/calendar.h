#ifndef CALENDAR_H
#define CALENDAR_H

#include <QString>
#include <QDateTime>

#include <QFile>
#include <QDateEdit>
#include <QDate>


#include "../ui_mainwindow.h"

class Calendar
{
public:
    Calendar(Ui::MainWindow* mw);

    QMap<QDate, QList<QString>> date_to_note;
    QMap<int, QList<QString>> day_to_class;
    QDate selectedDate = QDate::currentDate();

    void dateChanged(Ui::MainWindow *ui, QDate);
    void taskAdded(Ui::MainWindow *ui);
    void removeTask(Ui::MainWindow *ui);
    void removeAll(Ui::MainWindow *ui);
    void saveHistory();
    // Inicijalizuje mapu sa prethodnom istorijom
    void initializeMap();
    void initializeClassMap();
    void addCourse(QDate next_d, QString desc);

};

#endif // CALENDAR_H
