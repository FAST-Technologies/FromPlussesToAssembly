cmake_minimum_required(VERSION 3.29)
project(FromPlusesToAssemblyLab1)

set(CMAKE_CXX_STANDARD 20)

add_executable(FromPlusesToAssemblyLab1 test_main.cpp
        VariableTableV2_0/VariableTableV2_0.cpp
        LexemeV2_0/LexemeV2_0.cpp
        ConstantTableV2_0/ConstantTableV2_0.cpp
        Constants.h
        test_main.cpp)
