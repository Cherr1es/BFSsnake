cmake_minimum_required(VERSION 3.26)
project(BFSsnake)

set(CMAKE_CXX_STANDARD 17)
set(EasyX_INC easyx\\include)
set(EasyX_LINK easyx\\lib64)
include_directories(${EasyX_INC})
link_directories(${EasyX_LINK})
link_libraries(libeasyx.a)

add_executable(BFSsnake main.cpp)
