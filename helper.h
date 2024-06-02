#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <cstdlib>
#include<iostream>
#include <cstring>



class Student {
    public:
    char* name;
    int id;
    int score;
    // 默认构造函数
    Student() {
        name = new char[9]; // 分配固定长度为8的字符串，加上终止符'\0'
        memset(name, 0, 9); // 初始化字符串
    }

    // 析构函数
    ~Student() {
        delete[] name; // 释放分配的内存
    }



    // 重载输出流操作符
    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os.write(student.name, sizeof(student.name)); // 写入固定长度的字符串
        os.write(reinterpret_cast<const char*>(&student.id), sizeof(student.id));
        os.write(reinterpret_cast<const char*>(&student.score), sizeof(student.score));
        return os;
    }

    // 重载输入流操作符
    friend std::istream& operator>>(std::istream& is, Student& student) {
        is.read(student.name, sizeof(student.name)); // 读取固定长度的字符串
        is.read(reinterpret_cast<char*>(&student.id), sizeof(student.id));
        is.read(reinterpret_cast<char*>(&student.score), sizeof(student.score));
        return is;
    }
};


void generateStudentRecords(Student** students, int numStudents); 
// void generateStudentRecords(std::vector<Student> &students , int numStudents);
int generateRandomID();

char* generateRandomName();

int generateRandomScore();

#endif