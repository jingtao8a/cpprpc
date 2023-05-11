add_executable(test1 test1.cpp)
target_link_libraries(test1 cppweb cppjson)

add_executable(testRpcError testRpcError.cpp)
target_link_libraries(testRpcError cpprpc cppweb cppjson)