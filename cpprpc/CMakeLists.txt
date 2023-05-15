add_library(cpprpc STATIC
    RpcError.cpp
    server/BaseServer.cpp
    server/NotifyProcedure.cpp
    server/Procedure.cpp
    server/ReturnProcedure.cpp
    server/RpcServer.cpp
    server/RpcService.cpp
    client/BaseClient.cpp)

target_link_libraries(cpprpc cppweb cppjson)

add_subdirectory(stub)