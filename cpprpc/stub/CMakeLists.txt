add_executable(stub_generator
    main.cpp
    StubGenerator.cpp
    ServiceStubGenerator.cpp
    ClientStubGenerator.cpp)

target_link_libraries(stub_generator cpprpc)

install(TARGETS stub_generator DESTINATION bin)