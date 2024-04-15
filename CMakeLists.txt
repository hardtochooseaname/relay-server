# 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project("EchoServer")

# 设置构建类型为Debug
set(CMAKE_BUILD_TYPE Debug)

# 添加可执行文件
add_executable(client client.cpp
                        src/Buffer.cpp
                        src/Socket.cpp
                        src/InetAddr.cpp
                        src/utils.cpp)      
                        
add_executable(proxyserv main.cpp
                        src/Buffer.cpp
                        src/Socket.cpp
                        src/InetAddr.cpp
                        src/utils.cpp
                        src/Channel.cpp
                        src/Epoller.cpp
                        src/Eventloop.cpp
                        src/Server.cpp
                        src/Task.cpp
                        src/ProxyTask.cpp)   
                        
target_compile_definitions(proxyserv PRIVATE ISPROXY)

add_executable(echoserv main.cpp
                        src/Buffer.cpp
                        src/Socket.cpp
                        src/InetAddr.cpp
                        src/utils.cpp
                        src/Channel.cpp
                        src/Epoller.cpp
                        src/Eventloop.cpp
                        src/Server.cpp
                        src/Task.cpp
                        src/EchoTask.cpp) 

target_compile_definitions(echoserv PRIVATE ISECHO)


# 指定头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)