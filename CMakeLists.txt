cmake_minimum_required(VERSION 3.10)

project(
    HeaderParserLib
    VERSION 1.0
    LANGUAGES CXX
)

#Library sources
add_subdirectory(src)

#Tests
add_subdirectory(tests)