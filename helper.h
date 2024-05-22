#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <cstdlib>



struct Student {
    std::string name;
    int id;
    int score;
};

void generateStudentRecords(Student **students, int numStudents);
int generateRandomID();

std::string generateRandomName();

int generateRandomScore();

#endif