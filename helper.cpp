#include "helper.h"

#include <random>
#include <sstream>

// 随机生成学生姓名
std::string generateRandomName() {
    std::string name = "";
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const int nameLength = 8;
    for (int i = 0; i < nameLength; ++i) {
        name += alphabet[rand() % (sizeof(alphabet) - 1)];
    }
    return name;
}

// 随机生成学生学号
int generateRandomID() {
    return rand() % 10000; // 假设学号为 0~9999 的随机数
}

// 随机生成学生成绩
int generateRandomScore() {
    return rand() % 101; // 成绩为 0~100 的随机数
}

// 随机生成学生记录的函数
void generateStudentRecords(Student **students, int numStudents) {
    // 设置随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < numStudents; ++i) {
        students[i]->name = generateRandomName();
        students[i]->id = generateRandomID();
        students[i]->score = generateRandomScore();
    }
}
