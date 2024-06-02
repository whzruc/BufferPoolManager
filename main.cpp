#include "bufferManager.h"
#include "helper.h"
#include <iostream>
#include<vector>







int main() {
    const char *filename = "student_records.bin";
    int numStudents = 10000; // 假设生成 100000 条学生记录

    // 生成学生记录
    // Student* students[numStudents];
    Student** students = new Student*[numStudents];
    // std::vector<Student> students;
    BufferPool* bufferPool= new BufferPool(filename);
    // c++ 内部生成目标

    std::cout<<"生成学生数据开始"<<std::endl;
    generateStudentRecords(students, numStudents);
    std::cout<<"生成学生数据完成"<<std::endl;


    // 将学生记录写入二进制文件
    std::cout<<"开始写入学生数据"<<std::endl;
    bufferPool->writeStudentRecords(students,numStudents);
    std::cout<<"写入学生数据完成"<<std::endl;

    // 读取学生记录
    bufferPool->readStudentRecords();

    return 0;
}
