cmake_minimum_required(VERSION 3.21)

project(e++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
message("C++ Standard: ${CMAKE_CXX_STANDRAD}")
file(GLOB SRC_FILES "epp.cpp", "lexer/eplex.cpp", "parser/east.cpp", "eppack/error/epperr.cpp", "eppack/var/var.cpp", "eppack/calc/calcvisitor.cpp", "eppack/calc/calc.cpp", "parser/parser.cpp",
        "eppack/function/func.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES} )
