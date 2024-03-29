#include "../include/mainwindow.hpp"
#include "../ui_mainwindow.h"
#include "../include/schedule.h"
#include "../include/schedulegenerator.h"
#include "../include/course.h"
#include "../include/config.hpp"

#include <QObject>
#include <QCheckBox>
#include <QString>
#include <vector>
#include <string>
#include <iostream>
#include <set>


Schedule::Schedule(){
    kmiljanScraper = std::make_unique<KmiljanScraper>();
    filePath = Config::getConfig()->getConfigPath() + "/raspored.json";
}

void Schedule::changeModule(Ui::MainWindow *ui, int index){

    clearTable(ui);
    selectedCourses.clear();
    selectedModule = modules[index];
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    std::set<std::string> courseSet;
    for(const auto& pair: moduleCourseTypeTermMap[selectedModule]) {
        courseSet.insert(pair.first); // ovo je course name
    }
    for(const auto& course : courseSet) {
        QCheckBox *checkBox = new QCheckBox(QString::fromStdString(course));
        scrollLayout->addWidget(checkBox);
        QObject::connect(checkBox, &QCheckBox::clicked, [=](bool checked) {
            ui->rasporedStartButton->setFocus();
            if (checked) {
                selectedCourses.insert(course);
//                std::cerr << "Selected " << course << std::endl;
            } else {
                selectedCourses.erase(course);
//                std::cerr << "Deselected " << course << std::endl;
            }
        });
    }
    delete ui->examArea->takeWidget(); // fix memory leak
    ui->examArea->setWidget(scrollContent);
};

void Schedule::clearTable(Ui::MainWindow *ui) {
    ui->scheduleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->scheduleTable->setFocusPolicy(Qt::NoFocus);
    ui->scheduleTable->setSelectionMode(QAbstractItemView::NoSelection);
    for(int r = 0; r < ui->scheduleTable->rowCount(); r++){
        for(int c = 0; c < ui->scheduleTable->columnCount(); c++){
            ui->scheduleTable->setSpan(r, c, 1, 1);
            auto item = ui->scheduleTable->takeItem(r, c);
            delete item;
        }
    }
}

void Schedule::findSchedule(Ui::MainWindow *ui){
    clearTable(ui);
    brojRasporeda = 0;
    while(!m_gen.schedules.empty()){
        m_gen.schedules.erase(m_gen.schedules.begin());
    }
    QString oldText = ui->rasporedStartButton->text();
    ui->rasporedStartButton->setText("...");
    StrMap<StrMap<CourseSet>> reducedMap;
    for (auto& pair : moduleCourseTypeTermMap[selectedModule]){
        auto courseName = pair.first;
        auto courseMap = pair.second;
        if (selectedCourses.find(courseName) != selectedCourses.end()) {
            reducedMap[courseName] = courseMap;
        }
    }
    m_gen.find(reducedMap);
    m_gen.displaySchedule(ui->scheduleTable, brojRasporeda);
    ui->rasporedStartButton->setText(oldText);
}

void Schedule::downloadSchedule(Ui::MainWindow *ui){
    std::unordered_map<std::string, CourseSet> moduleCourseMap = kmiljanScraper->download();
    modules.clear();
    for (const auto& pair : moduleCourseMap) {
        modules.push_back(pair.first);
    }
    ui->smerBox->setPlaceholderText("Izaberi smer");
    ui->smerBox->clear();
    for(auto& module : modules){
        ui->smerBox->addItem(QString::fromStdString(module));
    }
//    moduleCourseTermMap;
    for(const auto& pair: moduleCourseMap) {
        auto module = pair.first;
        auto courses = pair.second;
        for(const auto& course : courses){
            moduleCourseTypeTermMap[module][course.description][course.course_type].insert(course);
        }
    }
};

void Schedule::scrapeSchedule(Ui::MainWindow *ui){
    QString oldText = ui->scrapeButton->text();
    ui->scrapeButton->setText("Ucitavanje...");
    downloadSchedule(ui);
    ui->scrapeButton->setText(oldText);
    ui->rasporedStartButton->setEnabled(true);
    ui->scheduleTable->setEnabled(true);
    ui->smerBox->setEnabled(true);
    ui->examArea->setEnabled(true);
    ui->SacuvajButton->setEnabled(true);
    ui->sledeciButton->setEnabled(true);
    ui->prethodniButton->setEnabled(true);
    ui->scrapeButton->setEnabled(false);
}

void Schedule::nextSchedule(Ui::MainWindow *ui){
    clearTable(ui);
    brojRasporeda++;

    if(brojRasporeda == m_gen.schedules.size()){
        brojRasporeda = 0;
    }

    m_gen.displaySchedule(ui->scheduleTable, brojRasporeda);
}

void Schedule::prevSchedule(Ui::MainWindow *ui){
    clearTable(ui);
    brojRasporeda--;

    if(brojRasporeda == -1){
        brojRasporeda = m_gen.schedules.size() - 1;
    }

    m_gen.displaySchedule(ui->scheduleTable, brojRasporeda);
}


void Schedule::saveSchedule(Ui::MainWindow *ui){
    m_gen.saveCoursesToJson(filePath);

}

void Schedule::loadSchedule(Ui::MainWindow *ui){
    clearTable(ui);
    Generator tmp_gen;
    std::vector<Course> load = tmp_gen.loadCoursesFromJson(filePath);
    tmp_gen.schedules.push_back(load);
    int n = tmp_gen.schedules.size() - 1;
    tmp_gen.displaySchedule(ui->scheduleTable,n);
    tmp_gen.schedules.pop_back();
}
