cmake_minimum_required(VERSION 3.10)

project(EmployeeValidation)

set(CMAKE_CXX_STANDARD 17)

# Build each file as a separate executable
add_executable(EmployeeValidation employeValidation.cpp)
add_executable(IsCitizen isCitizen.cpp)
add_executable(Learning learning.cpp)
add_executable(Learning2 learning_2.cpp)
add_executable(MultiThreading multiThreading.cpp)


