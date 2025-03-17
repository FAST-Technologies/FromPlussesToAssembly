cmake_minimum_required(VERSION 3.29)
project(FromPlusesToAssemblyLab1)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -IC:/msys64/mingw64/include")

# Find libxlsxwriter (CMake should be able to locate it automatically)
# Include directories
include_directories("C:/msys64/mingw64/include")

# Link directories
link_directories("C:/msys64/mingw64/lib")

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES: ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES}")
message("CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES: ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES}")

#message(STATUS "PATH: ${env.PATH}")
#message(STATUS "MSYSTEM: ${env.MSYSTEM}")

add_executable(FromPlusesToAssemblyLab1 test_main.cpp
        VariableTableV2_0/VariableTableV2_0.cpp
        ConstantTableV2_0/ConstantTableV2_0.cpp
        Constants.h
        test_main.cpp
        OutputModuleV2_0/OutputModuleV2_0.cpp
        Scanner/Scanner.cpp)

target_link_libraries(FromPlusesToAssemblyLab1 xlsxwriter)

## Link against the library
#if(libxlsxwriter_FOUND)
#    message(STATUS "libxlsxwriter found at ${libxlsxwriter_DIR}") # Debugging
#    target_link_libraries(tableToExcel libxlsxwriter::xlsxwriter)  # Link against alias
#else()
#    message(FATAL_ERROR "libxlsxwriter not found.  Please install it and set CMAKE_PREFIX_PATH correctly.")
#endif()
