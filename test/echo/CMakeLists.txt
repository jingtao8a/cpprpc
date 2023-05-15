add_executable(EchoClient EchoClient.cpp)
target_link_libraries(EchoClient cpprpc)

add_executable(EchoService EchoService.cpp)
target_link_libraries(EchoService cpprpc)