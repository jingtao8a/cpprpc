add_executable(test1 test1.cpp)
target_link_libraries(test1 cppweb cppjson)

add_executable(testRpcError testRpcError.cpp)
target_link_libraries(testRpcError cpprpc)

add_executable(testException testException.cpp)
target_link_libraries(testException cpprpc)

add_subdirectory(echo)
add_subdirectory(add)