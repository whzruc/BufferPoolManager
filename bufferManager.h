#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include "helper.h"
#include <cstring>
#include <map>




const int PAGE_SIZE = 4096; // 页大小为 4096 字节
const int BUFFER_SIZE = 4;   // 缓冲区大小为 4 页
const int fileOffset=96;// 会给header分配96字节的空间存储 记录数量和需要的空间大小

// 存储一页数据的结构体
class Page {
    private:
    char data[PAGE_SIZE];
    int dirty; // dirty 表示该页是否已被修改
    int pageNumber; // 页号，用于 LRU 算法
    int offset;
    int full;

    public:
    Page(int pageNumber):pageNumber(pageNumber){
        dirty=0;
        memset(data,0,PAGE_SIZE);
        offset=0;
        full=0;
    }
    // 一次性写一页数据

    // 写入数据 
    bool writeData(Student* student){
        dirty=1;
        if(offset+sizeof(Student)>PAGE_SIZE){
            full=1;
            return false;
        }
        memcpy(data+offset,student,sizeof(Student));
        offset+=sizeof(Student);
        return true;
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
    char* filename;
    int global_last_used;
    std::map<int, std::pair<int,int>> page_index_map; // 页面索引和缓冲区索引的映射，用于LRU算法
    // lru_queue



public:
    BufferPool(const char* filename) {
        current = 0;
        currentPageNumber=0;
        filename=filename;
        global_last_used=0;
        // 先用新页将缓冲区填满
        for(int i=0;i<BUFFER_SIZE;i++){
            buffer[i]=new Page(currentPageNumber++);
            page_index_map.insert({i,{global_last_used,buffer[i]->getDirty()}});
        }
    }
    // 将整个Student* 写入
    void writeStudentRecords(Student** students,int numStudents);
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