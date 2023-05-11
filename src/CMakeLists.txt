add_library(cpprpc 
    RpcError.cpp)
target_link_libraries(cpprpc cppweb cppjson)