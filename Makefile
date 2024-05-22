# 编译器设置
CXX = g++
CXXFLAGS = -std=c++14 -Wall 

# 源文件列表
SRCS = main.cpp bufferManager.cpp helper.cpp

# 目标文件列表
OBJS = $(SRCS:.cpp=.o)

# 可执行文件名称
TARGET = my_program

# 默认目标，生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 依赖关系
main.o: main.cpp bufferManager.h helper.h
bufferManager.o: bufferManager.cpp bufferManager.h helper.h
helper.o: helper.cpp helper.h

# 编译规则
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -O3 $< -o $@

# 清理规则
clean:
	rm -f $(TARGET) $(OBJS)

