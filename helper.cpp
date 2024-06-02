#include "helper.h"
#include <random>
#include <sstream>
#include <string>
#include <iostream>


// 随机生成学生姓名
char* generateRandomName() {
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const int nameLength = 8;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphabet) - 2);

    char* name = new char[nameLength + 1]; // +1 用于存储字符串的终止符 '\0'
    for (int i = 0; i < nameLength; ++i) {
        name[i] = alphabet[dis(gen)];
    }
    name[nameLength] = '\0'; // 添加字符串的终止符 '\0'
    return name;
}

// 随机生成学生学号
int generateRandomID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9999);
    
    return dis(gen);
}

// 随机生成学生成绩
int generateRandomScore() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);
    
    return dis(gen);
}

// 随机生成学生记录的函数
void generateStudentRecords(Student** students, int numStudents) {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < numStudents; ++i) {
        students[i] = new Student;  // 确保已分配内存
        students[i]->name = generateRandomName();
        students[i]->id = generateRandomID();
        students[i]->score = generateRandomScore();
    }
}


