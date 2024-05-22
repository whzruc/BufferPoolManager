#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "helper.h"
#include "bufferManager.h"
#include<algorithm>

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
            page_index_map.at(current)={global_last_used++,buffer[current]->getDirty()};
            
            current=findFreePage();
            buffer[current]->writeData(student[i]);
        }
    }
    // 写文件头部
    writeHeader(numStudents,PAGE_SIZE*currentPageNumber+buffer[current]->getOffset());

}

int BufferPool::findFreePage(){
    // 先检查是否有未满的页
    auto is_not_full = [](const auto& pair) { return pair.second.second == 0; };
    auto not_full_page = std::find_if(page_index_map.begin(), page_index_map.end(), is_not_full);
    if (not_full_page != page_index_map.end()) {
        // 返回第一个未满的页
        return not_full_page->first;
    }

    // 没有未满的页，执行LRU替换
    int page_to_replace = -1;
    auto min_last_used_page = std::min_element(page_index_map.begin(), page_index_map.end(),
        [](const auto& a, const auto& b) { return a.second.first < b.second.first; });
    page_to_replace = min_last_used_page->first;

    // 执行LRU替换
    // 分配一个新页，将旧页写入磁盘
    flushPageToFile(buffer[current]);

    buffer[page_to_replace] = new Page(currentPageNumber++);
    page_index_map[page_to_replace] = { global_last_used, 0 }; // 更新页面映射
    return page_to_replace;
}


bool BufferPool::flushPageToFile(Page* page){
        std::ofstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
        if (!file) {
            std::cerr << "Error: Unable to open file." << std::endl;
            return false;
        }
        file.seekp(page->getPageNumber() * PAGE_SIZE + fileOffset);
        file.write(page->getData(), PAGE_SIZE);
        file.close();
        return true;
}






// // 将学生记录写入二进制文件的函数
// void writeStudentRecords(const char *filename, Student *students, int numStudents) {
//     // 在这里实现将学生记录写入二进制文件的逻辑
// }

// // 读取二进制文件的函数
// void readStudentRecords(const char *filename) {
//     // 在这里实现读取二进制文件的逻辑
// }

// // 在文件头写入学生记录的条数和文件大小的函数
// void writeHeader(const char *filename, int numStudents, int fileSize) {
//     // 在这里实现在文件头写入学生记录的条数和文件大小的逻辑

// }



