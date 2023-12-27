#include "../include/schedulegenerator.h"
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <QDebug>
#include <QTableWidget>
#include <iostream>

#define ROWS 5
#define COLUMNS 13

Generator::Generator() {
    bitmap.resize(ROWS, std::vector<bool>(COLUMNS, 0));
}

bool Generator::scheduleFilter(const std::vector<Course>& schedule) {
    size_t n = schedule.size();

    auto classroomGroup = [](const Course& course) -> int {
        if (course.classroom.empty()) return 0;
        if (course.classroom[0] == 'J') return 1;
        if (course.classroom[0] == 'N') return 2;
        return 0;
    };

    for (size_t i = 0; i < n - 1; ++i) {
        if (schedule[i].day == schedule[i + 1].day) {
            if (classroomGroup(schedule[i]) != classroomGroup(schedule[i + 1])) {
                if (schedule[i + 1].start - schedule[i].end < 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Generator::find(StrMap<StrMap<CourseSet>> courseTypeTermMap) {
    int stop = 0;
    // koliko stvari treba da stavimo
    for(const auto& pair : courseTypeTermMap) {
        StrMap<CourseSet> types = pair.second;
        stop += types.size();
    }
    std::vector<Course> placedList;
    _find(courseTypeTermMap, placedList, 0, stop);
    std::sort(schedules.begin(), schedules.end(), [](const auto& lhs, const auto& rhs) {
        return std::accumulate(lhs.begin(), lhs.end(), 0,
                               [](int sum, const Course& course) { return sum + course.start; }) <
               std::accumulate(rhs.begin(), rhs.end(), 0,
                   [](int sum, const Course& course) { return sum + course.start; });
    });
    std::cout << "Schedules found: " << schedules.size() << std::endl;
}

void Generator::displaySchedule(QTableWidget* tableWidget) {
    if (!schedules.empty()) {
        const std::vector<Course>& schedule = schedules.front();
        for (const auto& course : schedule) {
            int day = course.day;
            int start = course.start;
            int end = course.end;

            for (int i = start; i < end; ++i) {
                QTableWidgetItem* subItem = new QTableWidgetItem(QString::fromStdString(course.description + '\n' + course.course_type +
                                                                                        '\n' + course.classroom));
                tableWidget->setItem(day, i, subItem);
            }
        }
    }
}

void Generator::_find(StrMap<StrMap<CourseSet>>& courseTypeTermMap, std::vector<Course>& placedList, int i, int stop) {
    if (i == stop) {
        std::cerr << "Shouldnt be reachable" << std::endl;
        return;
    }

    for (const auto& pair : courseTypeTermMap) {
        std::string course = pair.first;
        StrMap<CourseSet> typeTermMap = pair.second;
        for (const auto& pair : typeTermMap) {
            std::string type = pair.first;
            CourseSet terms = pair.second;
            for (const auto& term : terms) {
                bool notPlacedYet = placed.find(term.description + term.course_type) == placed.end();
                if (schedules.size() < 200 && notPlacedYet && !conflict(term)) {
                    place(term);
                    placedList.push_back(term);
                    if(placedList.size() == stop) {
                        schedules.push_back(placedList);
                    }
                    else {
                        _find(courseTypeTermMap, placedList, i + 1, stop);
                    }
                    remove(term);
                    placedList.pop_back();
                }
            }
        }
    }
    return;
}

bool Generator::conflict(const Course& term) {
    for (int i = term.start; i < term.end; ++i) {
        if (bitmap[term.day][i] == true) {
            return true;
        }
    }
    return false;
}

void Generator::place(const Course& term) {
    for (int i = term.start; i < term.end; ++i) {
        bitmap[term.day][i] = true;
    }
    placed.insert(term.description + term.course_type);
}

void Generator::remove(const Course& term) {
    for (int i = term.start; i < term.end; ++i) {
        bitmap[term.day][i] = false;
    }
    placed.erase(term.description + term.course_type);
}