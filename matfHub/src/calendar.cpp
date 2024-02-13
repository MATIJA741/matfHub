#include "../include/calendar.h"
#include "../include/config.hpp"
#include "../ui_mainwindow.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

#include <QDate>
#include <QDateEdit>
#include <QFile>

#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG (qDebug() << __FILENAME__ << ":" << __LINE__ << ":\t")

Calendar::Calendar(Ui::MainWindow *ui) {
  initializeMap();
  initializeClassMap();
  ui->calendarWidget->setSelectedDate(selectedDate);
  ui->calendarWidget->setVerticalHeaderFormat(
      QCalendarWidget::NoVerticalHeader);
  ui->calendarWidget->setFirstDayOfWeek(Qt::DayOfWeek::Monday);

  ui->dateEdit->QDateEdit::setDate(selectedDate);

  for (auto itemStr : date_to_note[selectedDate]) {
    QListWidgetItem *item = new QListWidgetItem(itemStr, ui->listWidget);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
  }
}

void Calendar::dateChanged(Ui::MainWindow *ui, QDate date) {
  selectedDate = date;
  ui->dateEdit->QDateEdit::setDate(selectedDate);
  ui->calendarWidget->setSelectedDate(date);
  ui->listWidget->clear();

  if (!day_to_class[selectedDate.dayOfWeek()].empty()) {
    for (QString it : day_to_class[selectedDate.dayOfWeek()]) {
      if (date_to_note[selectedDate].contains(it))
        continue;
      else
        date_to_note[selectedDate].append(it);
    }
  }
  date_to_note[selectedDate].sort();
  for (auto itemStr : date_to_note[selectedDate]) {
    if (itemStr != "") {
      QListWidgetItem *item = new QListWidgetItem(itemStr, ui->listWidget);
      item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
  }
}

void Calendar::taskAdded(Ui::MainWindow *ui) {

  if (ui->dateEdit->date() != selectedDate) {
    dateChanged(ui, ui->dateEdit->date());
  }

  QString itemString = ui->timeEdit->time().toString("HH:mm") + " " +
                       ui->newItemEdit->toPlainText();
  date_to_note[selectedDate].append(itemString);
  date_to_note[selectedDate].sort();

  ui->listWidget->clear();

  for (auto itemStr : date_to_note[selectedDate]) {
    if (itemStr != "") {
      QListWidgetItem *item = new QListWidgetItem(itemStr, ui->listWidget);
      item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
  }

  ui->newItemEdit->clear();
  ui->newItemEdit->setFocus();
}

void Calendar::removeTask(Ui::MainWindow *ui) {

  if (ui->listWidget->currentItem()) {
    QString itemString = ui->listWidget->currentItem()->text();
    date_to_note[selectedDate].removeOne(itemString);
    QListWidgetItem *item =
        ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete item;
  }
}

void Calendar::removeAll(Ui::MainWindow *ui) {
  ui->listWidget->clear();
  date_to_note[selectedDate].clear();
}

// Cuvamo mapu u JSON formatu pri izlasku iz aplikacije
void Calendar::saveHistory() {
  QString fileName = Config::getConfig()->getConfigPath() + "/map.txt";

  QFile file(fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    return;
  }

  QTextStream out(&file);

  // Moram da konvertujem QDate u format pogodan za JSON
  QMap<QString, QVariant> jsonMap;
  for (auto it = date_to_note.begin(); it != date_to_note.end(); ++it) {
    QString dateString = it.key().toString("yyyy-MM-dd");
    jsonMap.insert(dateString, it.value().join("\n"));
  }

  QJsonObject jsonObject = QJsonObject::fromVariantMap(jsonMap);
  QJsonDocument jsonDoc(jsonObject);
  QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

  out << jsonString;

  file.close();
}

void Calendar::addCourse(QDate next_d, QString desc) {
  if (date_to_note[next_d].empty()) {
    QList<QString> note = {desc};
    date_to_note.insert(next_d, note);

  } else {
    date_to_note[next_d].append(desc);
  }
}

void Calendar::initializeClassMap() {

  for (auto date : date_to_note.keys()) {
    if (!day_to_class[date.dayOfWeek()].empty()) {
      for (auto course : day_to_class[date.dayOfWeek()]) {
        if (date_to_note[date].contains(course))
          date_to_note[date].removeOne(course);
      }
    }
  }

  if (!day_to_class.isEmpty()) {
    day_to_class.clear();
  }

  QFile schedulePath(Config::getConfig()->getConfigPath() + "/raspored.json");

  if (!schedulePath.open(QIODevice::ReadOnly)) {
    return;
  }

  QByteArray jsonDataSchedule = schedulePath.readAll();
  schedulePath.close();

  QJsonDocument jsonDocSchedule = QJsonDocument::fromJson(jsonDataSchedule);
  if (jsonDocSchedule.isNull()) {
    return;
  }

  QJsonArray coursesArray = jsonDocSchedule.array();

  int i = 0;
  for (const auto &courseData : coursesArray) {
    QJsonObject courseObject = courseData.toObject();

    QString desc = courseObject["description"].toString();

    int start = courseObject["start"].toInt();
    int dur = courseObject["duration"].toInt();
    QString teacher = courseObject["teacher"].toString();
    QString classroom = courseObject["classroom"].toString();
    QString courseType = courseObject["course_type"].toString();

    int d = courseObject["day"].toInt() + 1;

    QString startStr = "";
    if (start < 10) {
      startStr = "0";
    }
    startStr += QString::number(start);

    QString itemStr = startStr + ":15 - " + QString::number(start + dur) +
                      ":00 " + desc + ", " + teacher + " - " + courseType +
                      ", " + classroom;

    if (day_to_class[d].empty()) { // ovo je problem
      day_to_class[d] = {};
    }
    if (!day_to_class[d].contains(itemStr))
      day_to_class[d].append(itemStr);
  }
}

void Calendar::initializeMap() {
  QFile jsonFile(Config::getConfig()->getConfigPath() + "/map.txt");

  if (!jsonFile.open(QIODevice::ReadOnly)) {
    return;
  }

  QByteArray jsonData = jsonFile.readAll();
  jsonFile.close();

  QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
  if (jsonDoc.isNull()) {
    return;
  }

  QJsonObject jsonObject = jsonDoc.object();

  for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
    QString dateString = it.key();
    QDate date = QDate::fromString(dateString,
                                   "yyyy-MM-dd"); // Parse string back to QDate
    QList<QString> note = it.value().toString().split("\n");
    date_to_note.insert(date, note);
  }
}
