#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "helper.h"
#include "bufferManager.h"
#include<algorithm>
// #include<iostream>
#include<vector>
// using namespace std;

void BufferPool::writeStudentRecords(Student** student,int numStudents){
    // if(buffer[currentSize])
    for(int i=0;i<numStudents;i++){
        if(buffer[current]->writeData(student[i])){
            // 写入成功 
            continue;

            
        }else{
            // 设置上一页的last_used
            // global_last_used++并且标记已满
            // page_index_map.at(current)={global_last_used++,buffer[current]->getDirty()};
            buffer[current]->last_used=global_last_used++;
            current=findFreePage();
            std::cout<<"当前页:"<<current<<std::endl;
            buffer[current]->writeData(student[i]);
        }
    }
    // 写文件头部
    writeHeader(numStudents,PAGE_SIZE*currentPageNumber+buffer[current]->getOffset());
    std::cout<<"写入完成 页总数为:"<<currentPageNumber<<std::endl;
    // 将最后一个页数据写入磁盘
    flushPageToFile(buffer[current]);

}

int BufferPool::findFreePage(){
    // 检查buffer中最小的last_used
    int min_last_used=99999;
    int page_to_replace=0;
    for(int i=0;i<BUFFER_SIZE;i++){
        // 未满
        if(buffer[i]->last_used==0){
            return i;
        }
        // 调用lru
        if(buffer[i]->last_used<min_last_used){
            min_last_used=buffer[i]->last_used;
            page_to_replace=i;
        }
    }
    flushPageToFile(buffer[page_to_replace]);
    buffer[page_to_replace]=new Page(currentPageNumber++);
    return page_to_replace;
}

bool BufferPool::flushPageToFile(Page* page) {
    // std::cout<<"页"<<page->getPageNumber()<<"完成写入"<<std::endl;
    // totalPageNumber++;
    // 尝试以读写模式打开文件
    const char* filename=getFileName();
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    
    // 检查文件是否成功打开
    if (!file) {
        // 如果文件未成功打开，尝试以写入模式（创建文件）打开
        std::ofstream createFile(filename, std::ios::binary | std::ios::out);
        if (!createFile) {
            std::cerr << "Error: Unable to create file." << std::endl;
            return false;
        }
        // 创建成功后关闭文件
        createFile.close();
        
        // 重新以读写模式打开文件
        file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
        if (!file) {
            std::cerr << "Error: Unable to open file after creation." << std::endl;
            return false;
        }
    }

    // 定位文件指针到正确的位置
    file.seekp(page->getPageNumber() * PAGE_SIZE + fileOffset);
    
    // 写入数据
    file.write(page->getData(), PAGE_SIZE);
    
    // 检查写操作是否成功
    if (!file) {
        std::cerr << "Error: Writing to file failed." << std::endl;
        return false;
    }

    // 关闭文件
    file.close();
    return true;
}






// // // 将学生记录写入二进制文件的函数
// void writeStudentRecords(const char *filename, Student *students, int numStudents) {
//     // 在这里实现将学生记录写入二进制文件的逻辑

// }

void BufferPool::writeHeader(int numStudents, int fileSize) {
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "Error: Unable to open file for writing header." << std::endl;
        return;
    }

    // 移动到文件开始位置
    file.seekp(0);

    // 写入学生记录的条数
    file.write(reinterpret_cast<char*>(&numStudents), sizeof(int));

    // 写入文件大小
    file.write(reinterpret_cast<char*>(&fileSize), sizeof(int));

    // 保证写入了前 96 个字节
    // char padding[8 - 2 * sizeof(int)] = {0};
    // file.write(padding, sizeof(padding));

    if (!file) {
        std::cerr << "Error: Writing header to file failed." << std::endl;
    }

    file.close();
}

void BufferPool::readStudentRecords() {
    std::fstream file(filename, std::ios::binary | std::ios::in);
    if (!file) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return;
    }

    // 移动到文件开始位置#include <fstream>
    file.seekg(0);

    // 读取学生记录的条数
    int numStudents;
    file.read(reinterpret_cast<char*>(&numStudents), sizeof(int));

    // 读取文件大小
    int fileSize;
    file.read(reinterpret_cast<char*>(&fileSize), sizeof(int));

    // 打印读取的值
    std::cout << "Number of students: " << numStudents << std::endl;
    std::cout << "File size: " << fileSize << std::endl;
    // 根据读取的文件大小分配足够的缓冲空间 缓存区不够时 使用换页
    int needPageNumber=fileSize/PAGE_SIZE+1;
    // if(need)
    // file.re

    // file.seekg(8);
    // char* buffer_= new char[sizeof(Student)];
    // Student *student=new Student();
    
    // file.read(buffer_,sizeof(Student));
    // memcpy(student,buffer_,sizeof(Student));
    //                 std::cout << "Name: " << student->name 
    //                     << ", Student ID: " << student->id 
    //                     << ", Age: " << student->score << std::endl;

    global_last_used=0;
    for(int i=0;i<needPageNumber;i++){
        // if(i>=BUFFER_SIZE){}
        current=findFreePage();
        buffer[current]->readData(fileOffset+i*PAGE_SIZE,PAGE_SIZE,&file,true);
        buffer[current]->last_used=global_last_used++;
        // page_index_map.at(current)={global_last_used++,buffer[current]->getDirty()};
    }

    if (!file) {
        std::cerr << "Error: Reading header from file failed." << std::endl;
    }

    file.close();
}



