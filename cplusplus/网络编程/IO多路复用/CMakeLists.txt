cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(TcpServerClient)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 添加可执行文件
add_executable(tcp_server epoll.cpp)
add_executable(tcp_client client.cpp)