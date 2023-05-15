cmake_minimum_required(VERSION 3.7)
project("cpprpc")

add_definitions("-std=c++14 -g")#定义编译器参数

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 包含第三方库的头文件路径
include_directories(thirdparty/cppweb)
include_directories(thirdparty/cppjson)
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(cpprpc)
add_subdirectory(test)

#第三方库
add_subdirectory(thirdparty/cppweb)
add_subdirectory(thirdparty/cppjson)
