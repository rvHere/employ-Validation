cmake_minimum_required(VERSION 3.10)
project(EmployeeValidation)

set(CMAKE_CXX_STANDARD 17)

# third party library
add_subdirectory(third_party/fmt)

add_executable(EmployeeValidation
        employeValidation.cpp
        isCitizen.cpp
        learning.cpp
        learning_2.cpp
        multiThreading.cpp
)

# Linking  third-party library to our program
target_link_libraries(EmployeeValidation PRIVATE fmt::fmt)
