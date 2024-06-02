#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include "helper.h"
#include <cstring>
#include <map>
#include<vector>
#include <fstream>
#include <string>
#include <sstream>



const int PAGE_SIZE = 4096; // 页大小为 4096 字节
const int BUFFER_SIZE = 4;   // 缓冲区大小为 4 页
const int fileOffset=8;// 会给header分配96字节的空间存储 记录数量和需要的空间大小

// 存储一页数据的结构体
class Page {
    private:
    char data[PAGE_SIZE];
    int dirty; // dirty 表示该页是否已被修改
    int pageNumber; // 页号，用于 LRU 算法
    int offset;
    int full;

    public:
    int last_used;
    Page(int pageNumber):pageNumber(pageNumber){
        dirty=0;
        memset(data,0,PAGE_SIZE);
        offset=0;
        full=0;
        last_used=0;
    }
    // 一次性写一页数据

    // 写入数据
    bool writeData(Student* student) {
        dirty = 1;
        if (offset + sizeof(Student) > PAGE_SIZE) {
            full = 1;
            return false;
        }
        std::stringstream ss;
        ss << *student;
        std::string studentStr = ss.str();
        const char* studentData = studentStr.c_str();
        memcpy(data + offset, studentData, studentStr.size());
        offset += sizeof(Student);
        return true;
    }

    // 读取数据函数
    void readData(int offset, int size, std::fstream* file, bool is_print) {
        if (!file->is_open()) {
            std::cerr << "File is not open!" << std::endl;
            return;
        }

        // 将文件指针移动到指定的偏移位置
        file->seekg(offset, std::ios::beg);
        std::cout<<"当前偏移量为:"<<offset<<std::endl;

        // 分配缓冲区以读取数据
        // char* buffer = new char[size];
        file->read(data, size);

        // 判断是否需要打印读取到的学生信息
        if (is_print) {
            int num_students = size / sizeof(Student);
            // std::cout<<"当前页总共有:"<<num_students<<"条学生记录"<<std::endl;
            for (int i = 0; i < num_students; ++i) {
                Student* student=new Student();
                std::istringstream ss(std::string(data + i * sizeof(Student), sizeof(Student)));
                ss >> *student;
                    // file->read(data+i*sizeof(Student),sizeof(Student));
                    // memcpy(student->name,data+i*sizeof(Student),8);
                    // memcpy(student->id,data+i*sizeof(Student)+8,sizeof(int));
                    if(i<3){
                        std::cout << "Name: " << student->name 
                            << ", Student ID: " << student->id 
                            << ", Age: " << student->score << std::endl;
                    }

            }
        }

    }

    int getPageNumber(){
        return pageNumber;
    }

    char* getData(){
        return data;
    }
    int getDirty(){
        return dirty;
    }
    int getOffset(){
        return offset;
    }
};

// 缓冲区类
class BufferPool {
private:
    Page* buffer[BUFFER_SIZE]; // 缓冲区包含的页数组
    int current; // 当前缓冲区中的页数
    int currentPageNumber;
    int totalPageNumber;
    char* filename;
    int global_last_used;
    std::map<int, std::pair<int,int>> page_index_map; // 页面索引和缓冲区索引的映射，用于LRU算法
    // lru_queue



public:
    char* getFileName(){
        return filename;
    }
    // 构造函数
    BufferPool(const char* fname) {
        current = 0;
        currentPageNumber = 0;
        global_last_used = 0;
        totalPageNumber=0;

        // 分配内存并复制文件名
        filename = new char[strlen(fname) + 1];
        strcpy(filename, fname);

        // 先用新页将缓冲区填满
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = new Page(currentPageNumber++);
            page_index_map.insert({i, {global_last_used, buffer[i]->getDirty()}});
        }
    }

    // 析构函数
    ~BufferPool() {
        // 释放缓冲区中的页内存
        for (int i = 0; i < BUFFER_SIZE; i++) {
            delete buffer[i];
        }
        // 释放文件名内存
        delete[] filename;
    }
    // 将整个Student数组 写入
    void writeStudentRecords(Student** student,int numStudents);
    // 写文件头
    void writeHeader(int numStudents,int size);
    // 读取整个文件
    void readStudentRecords();


    // 将页面数据入磁盘

    bool flushPageToFile(Page* page);


    // 查找空闲页或执行LRU算法腾出空间
    int findFreePage();

    
};


void writeStudentRecords(const char *filename, Student *students, int numStudents) ;

void readStudentRecords(const char *filename);

void writeHeader(const char *filename, int numStudents, int fileSize);


#endif