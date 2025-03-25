cmake_minimum_required(VERSION 3.29)
project(FromPlusesToAssemblyLab1)

set(CMAKE_CXX_STANDARD 20)

# Указываем, что используем MinGW
if (NOT MINGW)
    message(FATAL_ERROR "This project requires MinGW. Please use a MinGW toolchain.")
endif()

# Найти libxlsxwriter
find_library(XLSXWRITER_LIBRARY NAMES xlsxwriter libxlsxwriter HINTS "C:/msys64/mingw64/lib")
find_path(XLSXWRITER_INCLUDE_DIR xlsxwriter.h HINTS "C:/msys64/mingw64/include")

if (NOT XLSXWRITER_LIBRARY OR NOT XLSXWRITER_INCLUDE_DIR)
    message(FATAL_ERROR "libxlsxwriter not found. Please install it using 'pacman -S mingw-w64-x86_64-libxlsxwriter' in MSYS2.")
endif()

include_directories(${XLSXWRITER_INCLUDE_DIR})
link_directories(${XLSXWRITER_LIBRARY})

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES: ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES}")
message("CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES: ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES}")

add_executable(FromPlusesToAssemblyLab1
        test_main.cpp
        VariableTableV2_0/VariableTableV2_0.cpp
        ConstantTableV2_0/ConstantTableV2_0.cpp
        Constants.h
        OutputModuleV2_0/OutputModuleV2_0.cpp
        Scanner/Scanner.cpp
        Scanner/Scanner.cpp
)

target_link_libraries(FromPlusesToAssemblyLab1 ${XLSXWRITER_LIBRARY})

## Link against the library
#if(libxlsxwriter_FOUND)
#    message(STATUS "libxlsxwriter found at ${libxlsxwriter_DIR}") # Debugging
#    target_link_libraries(tableToExcel libxlsxwriter::xlsxwriter)  # Link against alias
#else()
#    message(FATAL_ERROR "libxlsxwriter not found.  Please install it and set CMAKE_PREFIX_PATH correctly.")
#endif()
