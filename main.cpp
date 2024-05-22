#include "bufferManager.h"
#include "helper.h"






int main() {
    const char *filename = "student_records.bin";
    int numStudents = 100; // 假设生成 100000 条学生记录

    // 生成学生记录
    Student* students[numStudents];
    BufferPool* bufferPool= new BufferPool(filename);
    // c++ 内部生成目标
    generateStudentRecords(students, numStudents);


    // 将学生记录写入二进制文件
    bufferPool->writeStudentRecords(students,numStudents);

    // 读取学生记录
    bufferPool->readStudentRecords();

    // 在文件头写入学生记录的条数和文件大小
    // 注意：在写入文件头之前，需要先获取文件的大小
    // 这里暂时用0表示文件大小
    // writeHeader(filename, numStudents, 0);

    // 读取二进制文件
    // readStudentRecords(filename);

    return 0;
}
