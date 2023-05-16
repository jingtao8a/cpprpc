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
install(TARGETS cpprpc DESTINATION lib)

set(HEADERS
    utils.hpp
    RpcError.hpp
    Exception.hpp)
install(FILES ${HEADERS} DESTINATION include/cpprpc)

set(HEADERS_SERVER
    server/BaseServer.hpp
    server/NotifyProcedure.hpp
    server/Procedure.hpp
    server/ReturnProcedure.hpp
    server/RpcServer.hpp
    server/RpcService.hpp)
install(FILES ${HEADERS_SERVER} DESTINATION include/cpprpc/server)

set(HEADERS_CLIENT
    client/BaseClient.hpp)
install(FILES ${HEADERS_CLIENT} DESTINATION include/cpprpc/client)

add_subdirectory(stub)