add_executable(crackle_tests
        main.cpp
        io_test.hpp
        DriverTest.cpp DriverTest.hpp)
target_link_libraries(crackle_tests crackle_lib)
#set_property(TARGET crackle_tests PROPERTY CXX_STANDARD 17)
