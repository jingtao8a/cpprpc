add_executable(stub_generator
    main.cpp
    StubGenerator.cpp
    ServiceStubGenerator.cpp)

target_link_libraries(stub_generator cpprpc)